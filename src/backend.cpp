#include "backend.hpp"

#include <iostream>
#include <algorithm>
#include <thread>


namespace Backend {


//// Controller class ////
Controller::Controller()
    : remaining_time{0},
      time{0},
      steps{0},
      speed{0},
      max_speed{0},
      power{0},
      power_target{0},
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

auto Controller::hallSensor = std::make_unique<SENSORS::HallSensor>();

auto Controller::wattmeter = std::make_unique<SENSORS::Wattmeter>();

bool Controller::targetReached() {
    bool reached;

    if (remaining_time) {
        reached = false;
    } else {
        reached = true;
    }

    if (power_target && (power < power_target)) {
        reached = false;
    } else {
        reached = true;
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

void Controller::setPowerTarget(float value) {
    power_target = value;
    emit powerTargetChanged(power_target);
    std::cout << "- POWER TARGET: " << power_target << std::endl;
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
    leaderboard = new Leaderboard{this};
}

void Controller::closeLeaderboard() {
    if (leaderboard != nullptr) {
        delete leaderboard;
        leaderboard = nullptr;
    }
}

void Controller::saveScore(QString name) {
    leaderboard->insertRecord(name.toUtf8().constData());
    emit scoresChanged(scores);
}

//// Challenge class ////
Challenge::Challenge() {
    std::cout << "- CHALLENGE QTHREAD CREATED" << std::endl;
}

Challenge::~Challenge() {
    std::cout << "- CHALLENGE QTHREAD DESTROYED" << std::endl;
}

void Challenge::start(Controller* controller) {
    static int const delay_s = 1;

    std::cout << "- CHALLENGE STARTED" << std::endl;

    // monitor steps and power on separate Posix threads
    std::thread t_steps(monitorStepsPThread, controller);
    std::thread t_power(monitorPowerPThread, controller);

    // clock
    while (not controller->targetReached()
            && controller->challenge_monitor_on) {
        std::this_thread::sleep_for(std::chrono::seconds(delay_s));

        controller->time++;
        emit controller->timeChanged(controller->time);

        controller->remaining_time--;
        emit controller->remainingTimeChanged(controller->remaining_time);
    }

    // wait for Posix threads to finish
    t_steps.join();
    t_power.join();

    std::cout << "- CHALLENGE FINISHED" << std::endl;
}

// function to monitor steps
void Challenge::monitorStepsPThread(Controller* controller) {
    static int const delay_us = 30;
    int n = 0;
    int new_value = controller->hallSensor->readValue();
    int previous_value = new_value;

    while (not controller->targetReached()
            && controller->challenge_monitor_on) {
        new_value = controller->hallSensor->readValue();

        // register step when sensor reading changes from high to low
        if ((new_value < previous_value)
            && (n*delay_us > 200)) {  // ignore rapid motion/noise
            controller->steps++;
            emit controller->stepsChanged(controller->steps);

            // calculate speed = 1/[duration of  1 step] (rpm)
            controller->speed = 60.0/(n*delay_us/1000.0);
            emit controller->speedChanged(controller->speed);

            if (controller->speed > controller->max_speed) {
                controller->max_speed = controller->speed;
                emit controller->maxSpeedChanged(controller->max_speed);
            }
            n = 0;
        } else {
            n++;
        }

        previous_value = new_value;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_us));
    }
}

void Challenge::monitorPowerPThread(Controller* controller) {
    static int const delay_us = 300;

    while (not controller->targetReached()
            && controller->challenge_monitor_on) {
        // register only positive values (generated power)
        float power = controller->wattmeter->power();
        if (power > 0.0) {
            controller->power += controller->wattmeter->power();
            emit controller->powerChanged(controller->power);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(delay_us));
    }
}

//// BatteryMonitor class ////
BatteryMonitor::BatteryMonitor() {
    std::cout << "- BATTERY MONITOR CREATED" << std::endl;
}

BatteryMonitor::~BatteryMonitor() {
    std::cout << "- BATTERY MONITOR DESTROYED" << std::endl;
}

void BatteryMonitor::start(Controller* controller) {
    static int const delay_s = 2;
    static float const max_oc_voltage = 13.0;
    static float const min_oc_voltage = 11.8;
    static float const max_oc_current = 0.7;
    static float const min_oc_current = 0;
    static float voltage;
    static float current;
    static int percentage;

    while (controller->battery_monitor_on == true) {
        current = controller->wattmeter->current();
        voltage = controller->wattmeter->voltage();

        // check if idling (~open-cirquit)
        if ((current > min_oc_current)
            && (current < max_oc_current)) {
            percentage =
                100*(voltage - min_oc_voltage)
                    /(max_oc_voltage - min_oc_voltage);
            percentage = std::max(0, percentage);
            percentage = std::min(100, percentage);
            controller->charge_percentage = std::max(0, percentage);

            emit controller->chargePercentageChanged(controller->charge_percentage);
            std::cout << "- BATTERY PERCENTAGE: " << controller->charge_percentage
                << "%" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(delay_s));
    }
}

//// Leaderboard class ////
Leaderboard::Leaderboard(Controller * the_controller)
    : controller{the_controller},
      row{0},
      failed{-1},
      status{0}
{
    // determine worst score criteria and database file according to challenge type
    if (controller->challenge_type == "max power") {
        worst = "MIN";
        worst_order = "ASC";
        db_file = "max_power.db";
        score = controller->power;
    } else if (controller->challenge_type == "max speed") {
        worst = "MIN";
        worst_order = "ASC";
        db_file = "max_speed.db";
        score = controller->max_speed;
    } else if (controller->challenge_type == "lightning fast") {
        worst = "MAX";
        worst_order = "DESC";
        db_file = "lightning_fast.db";
        score = controller->time;
    }

    // open database file
    failed = sqlite3_open(db_file.c_str(), &db);
    if (failed) {
        std::cerr << "Failed to open database file: " << sqlite3_errmsg(db) << std::endl;
    }

    // check for existing table with a query
    failed = readContents();
    if (failed) {
        // create the table if it doesn't exists
        std::cerr << "Creating new table in: " << db_file << std::endl;

        string create_table_cmd = (
            "CREATE TABLE players("
                "name  TEXT PRIMARY KEY,"
                "score REAL NOT NULL"
            ");"
        );
        status = sqlite3_exec(db, create_table_cmd.c_str(), nullptr, nullptr, &messageError);

        if (status != SQLITE_OK) {
            std::cerr << "Failed to create table: " << messageError << std::endl;
            sqlite3_free(messageError);
        }
    }

    // read existing worst score
    readWorstScore();

    // set flag variable to enable/disable new records
    if ((row >= max_rows)
        || (score < controller->worst_score)) {
        controller->can_save_score = false;
    } else {
        controller->can_save_score = true;
    }
    emit controller->canSaveScoreChanged(controller->can_save_score);

    std::cout << "LEADERBOARD CREATED" << std::endl;
}

Leaderboard::~Leaderboard() {
    cleanUpRecords();
    sqlite3_close(db);

    std::cout << "LEADERBOARD DESTROYED" << std::endl;
}

int const Leaderboard::max_rows = 20;

string const Leaderboard::query_cmd = "SELECT * FROM players ORDER BY score DESC;";

// callback to get worst score
int Leaderboard::score_callback(
    void * data, int cols, char * field[], char * col_names[]
) {
    Leaderboard * leaderboard = static_cast<Leaderboard *>(data);
    leaderboard->controller->worst_score =
        field[0] ? std::atof(field[0]) : 0;

    static_cast<void>(cols);
    static_cast<void>(col_names);
    return 0;
}

// callback to get contents formatted
int Leaderboard::contents_callback(
    void * data, int cols, char * field[], char * col_names[]
) {
    Leaderboard * leaderboard = static_cast<Leaderboard *>(data);
    leaderboard->row++;
    const char* name = field[0] ? field[0] : "";
    const char* score = field[1] ? field[1] : "";
    leaderboard->controller->scores << QVariant::fromValue(
        QVariantList{std::to_string(leaderboard->row).c_str(), name, score}
    );

    static_cast<void>(cols);
    static_cast<void>(col_names);
    return 0;
}

// callback to print contents
int Leaderboard::print_callback(
    void * data, int cols, char * field[], char * col_names[]
) {
    Leaderboard * leaderboard = static_cast<Leaderboard *>(data);
    leaderboard->row++;
    std::cout << std::to_string(leaderboard->row) + string(": ");
    for (int i = 0; i < cols; i++) {
        std::cout << (field[i] ? field[i] : "NULL") << ' ';
    }
    std::cout << std::endl;

    static_cast<void>(col_names);
    return 0;
}

int Leaderboard::readWorstScore() {
    string worst_score_cmd = "SELECT " + worst + "(score) FROM players;";
    status = sqlite3_exec(db, worst_score_cmd.c_str(), score_callback, this, &messageError);

    if (status != SQLITE_OK) {
        std::cerr << "Failed to read worst score: " << messageError << std::endl;
        sqlite3_free(messageError);

        return -1;
    }

    return 0;
}

int Leaderboard::readContents() {
    controller->scores = QVariantList{};
    row = 0;
    status = sqlite3_exec(db, query_cmd.c_str(), contents_callback, this, &messageError);

    if (status != SQLITE_OK) {
        std::cerr << "Failed to read table contents: " << messageError << std::endl;
        sqlite3_free(messageError);

        return -1;
    }

    return 0;
}

int Leaderboard::showContents() {
    std::cout << "-STATE OF TABLE" << std::endl;
    row = 0;
    status = sqlite3_exec(db, query_cmd.c_str(), print_callback, this, &messageError);

    if (status != SQLITE_OK) {
        std::cerr << "Failed to print table: " << messageError << std::endl;
        sqlite3_free(messageError);

        return -1;
    }

    return 0;
}

int Leaderboard::insertRecord(string new_name) {
    // insert entry
    std::cout << "Inserting: NAME: " << new_name << " SCORE: " << score << endl;
    string insert_cmd = (
        "INSERT INTO players VALUES('" + new_name + "', " + std::to_string(score) + ");"
    );
    status = sqlite3_exec(db, insert_cmd.c_str(), nullptr, nullptr, &messageError);

    if (status != SQLITE_OK) {
        std::cerr << "Failed to insert entry: " << messageError << std::endl;
        sqlite3_free(messageError);

        return -1;
    }

    // update data
    readContents();
    readWorstScore();

    return 0;
}

int Leaderboard::cleanUpRecords() {
    // delete empty entries
    string clean_cmd = "DELETE FROM players WHERE name IS NULL OR trim(name) = '';";
    status = sqlite3_exec(db, clean_cmd.c_str(), nullptr, nullptr, &messageError);

    if (status != SQLITE_OK) {
        std::cerr << "Failed to clean table: " << messageError << std::endl;
        sqlite3_free(messageError);

        return -1;
    }

    // delete worst excess rows
    if (row > max_rows) {
        int excess_rows = row - max_rows;
        string delete_cmd =
            "DELETE FROM players WHERE name IN ("
                "SELECT name FROM players ORDER BY score "
                + worst_order + " LIMIT " + to_string(excess_rows)
            + ");";
        status = sqlite3_exec(db, delete_cmd.c_str(), nullptr, nullptr, &messageError);

        if (status != SQLITE_OK) {
            std::cerr << "Failed to delete entries: " << messageError << std::endl;
            sqlite3_free(messageError);

            return -1;
        }
    }

    // update data
    readContents();
    readWorstScore();

    return 0;
}

}  // namespace Backend
