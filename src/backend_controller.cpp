#include "backend.hpp"

#include <iostream>
#include <vector>
#include <filesystem>


namespace Backend {

//// Controller class ////
Controller::Controller()
    : remaining_time{0},
      time{0},
      steps{0},
      speed{0},
      max_speed{0},
      power{0},
      energy_target{0},
      challenge_finished{false},
      charge_percentage{50},
      challenge_type{""},
      worst_score{0},
      can_save_score{false},
      battery_monitor_on{true},
      challenge_monitor_on{true}
{
    // create QThread for challenge
    challengeThread = new QThread;

    // create a QThread for the battery monitor
    batteryMonitorThread = new QThread;

    std::cout << "- CONTROLLER CREATED" << std::endl;
}

Controller::~Controller() {
    closeLeaderboard();
    stopChallenge();
    stopBatteryMonitor();

    std::cout << "- CONTROLLER DESTROYED" << std::endl;
}

std::unique_ptr<SENSORS::HallSensor> Controller::hallSensor =
    std::make_unique<SENSORS::HallSensor>("/dev/gpiochip0", 24);

std::unique_ptr<SENSORS::Wattmeter> Controller::wattmeter =
    std::make_unique<SENSORS::Wattmeter>();

bool Controller::targetReached() {
    bool reached;

    if (remaining_time) {
        reached = false;
    } else {
        std::cout << "- TIME TARGET REACHED" << std::endl;
        reached = true;
    }

    if (energy_target) {
        if (energy < energy_target) {
            reached = false;
        } else {
            std::cout << "- POWER TARGET REACHED" << std::endl;
            reached = true;
        }
    }

    return reached;
}

void Controller::setChallengeType(QString value) {
    challenge_type = value;
    std::cout << "- CHALLENGE TYPE: " << challenge_type.toUtf8().constData() << std::endl;
}

void Controller::setRemainingTime(int value) {
    remaining_time = value;
    emit remainingTimeChanged(remaining_time);
    std::cout << "- REMAINING TIME: " << remaining_time << std::endl;
}

void Controller::setTime(int value) {
    time = value;
    emit timeChanged(time);
    std::cout << "- TIME: " << time << std::endl;
}

void Controller::setSteps(int value) {
    steps = value;
    emit stepsChanged(steps);
    std::cout << "- STEPS: " << steps << std::endl;
}

void Controller::setSpeed(float value) {
    speed = value;
    emit speedChanged(speed);
    std::cout << "- SPEED: " << speed << std::endl;
}

void Controller::setMaxSpeed(float value) {
    max_speed = value;
    emit maxSpeedChanged(max_speed);
    std::cout << "- MAX SPEED: " << max_speed << std::endl;
}

void Controller::setPower(float value) {
    power = value;
    emit powerChanged(power);
    std::cout << "- POWER: " << power << std::endl;
}

void Controller::setEnergy(float value) {
    energy = value;
    emit energyChanged(energy);
    std::cout << "- ENERGY: " << energy << std::endl;
}

void Controller::setEnergyTarget(float value) {
    energy_target = value;
    emit energyTargetChanged(energy_target);
    std::cout << "- POWER TARGET: " << energy_target << std::endl;
}

void Controller::startBatteryMonitor() {
    // create a battery monitor QThread
    batteryMonitor = new BatteryMonitor;
    batteryMonitor->moveToThread(batteryMonitorThread);
    connect(
        this, &Controller::runBatteryMonitor,
        batteryMonitor, &BatteryMonitor::start
    );
    connect(
        batteryMonitorThread, &QThread::finished,
        batteryMonitor, &QObject::deleteLater
    );

    // enable monitors, start the thread, and signal battery monitor to run
    battery_monitor_on = true;
    batteryMonitorThread->start();
    emit runBatteryMonitor(this);
}

void Controller::stopBatteryMonitor() {
    // disable monitors and quit & wait for battery monitor QThread
    battery_monitor_on = false;
    batteryMonitorThread->quit();
    batteryMonitorThread->wait();
}

void Controller::startChallenge() {
    // create a challenge QThread
    challenge = new Challenge;
    challenge->moveToThread(challengeThread);
    connect(
        this, &Controller::runChallenge,
        challenge, &Challenge::start
    );
    connect(
        challengeThread, &QThread::finished,
        challenge, &QObject::deleteLater
    );

    // enable monitors, start the thread, and signal challenge to run
    challenge_monitor_on = true;
    challengeThread->start();
    emit runChallenge(this);
}

void Controller::stopChallenge() {
    if (challengeThread->isRunning()) {
        // disable monitors and quit & wait for challenge QThread
        challenge_monitor_on = false;
        challengeThread->quit();
        challengeThread->wait();
    }
}

void Controller::openLeaderboard() {
    // create a leaderboard
    leaderboard = std::make_unique<Leaderboard>(this);
}

void Controller::closeLeaderboard() {
    if (leaderboard) {
        leaderboard.reset();
    }
}

void Controller::saveScore(QString name) {
    leaderboard->insertRecord(string{name.toUtf8().constData()});
    emit scoresChanged(scores);
}

void Controller::deleteDatabases() {
    vector<string> files{
        "max_power.db",
        "max_speed.db",
        "lightning_fast.db"
    };

    for (auto & file : files) {
        try {
            if (std::filesystem::remove(file)) {
                std::cout << "- DELETED: " << file << std::endl;
            } else {
                std::cout << "- NOT DELETED: " << file << std::endl;
            }
        }
        catch(const std::filesystem::filesystem_error & err) {
            std::cerr << "Filesystem error: " << err.what() << std::endl;
        }
    }
}

}  // namespace Backend
