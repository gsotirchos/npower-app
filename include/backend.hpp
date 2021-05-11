#pragma once

#include <QObject>
#include <QThread>
#include <qqml.h>
#include <sqlite3.h>

#include <memory>

// npower/mock sensors libraries
#include "npower_sensors.hpp"
#include "fake_sensors.hpp"


#ifndef USE_FAKE_SENSORS
    #define SENSORS npower_sensors
#else
    #define SENSORS fake_sensors
#endif  // USE_FAKE_SENSORS


using std::string;


namespace Backend {

class Challenge;
class BatteryMonitor;
class Leaderboard;

enum class ChallengeType {
    record,
    time_attack
};

class Controller : public QObject {
    Q_OBJECT

    Q_PROPERTY(int chargePercentage
        MEMBER charge_percentage
        NOTIFY chargePercentageChanged
    )
    Q_PROPERTY(int remainingTime
        MEMBER remaining_time
        WRITE setRemainingTime
        NOTIFY remainingTimeChanged
    )
    Q_PROPERTY(int time
        MEMBER time
        NOTIFY timeChanged
        WRITE setTime
    )
    Q_PROPERTY(int steps
        MEMBER steps
        NOTIFY stepsChanged
        WRITE setSteps
    )
    Q_PROPERTY(float speed
        MEMBER speed
        NOTIFY speedChanged
        WRITE setSpeed
    )
    Q_PROPERTY(float maxSpeed
        MEMBER max_speed
        NOTIFY maxSpeedChanged
        WRITE setMaxSpeed
    )
    Q_PROPERTY(float power
        MEMBER power
        NOTIFY powerChanged
        WRITE setPower
    )
    Q_PROPERTY(float energy
        MEMBER energy
        NOTIFY energyChanged
        WRITE setEnergy
    )
    Q_PROPERTY(float energyTarget
        MEMBER energy_target
        NOTIFY energyTargetChanged
        WRITE setEnergyTarget
    )
    Q_PROPERTY(bool challengeIsFinished
        MEMBER challenge_finished
        NOTIFY challengeFinished
    )
    Q_PROPERTY(QString challengeType
        MEMBER challenge_type
        WRITE setChallengeType
    )
    Q_PROPERTY(QVariantList scores
        MEMBER scores
        NOTIFY scoresChanged
    )
    Q_PROPERTY(bool canSaveScore
        MEMBER can_save_score
        NOTIFY canSaveScoreChanged
    )

  public:
    Controller();
    ~Controller();

    static std::unique_ptr<SENSORS::HallSensor> hallSensor;
    static std::unique_ptr<SENSORS::Wattmeter> wattmeter;

    int remaining_time;
    int time;
    int steps;
    float speed;
    float max_speed;
    float power;
    float energy;
    float energy_target;
    bool challenge_finished;

    int charge_percentage;
    QString challenge_type;
    float worst_score;
    QVariantList scores;
    bool can_save_score;

    bool battery_monitor_on;
    bool challenge_monitor_on;

    bool targetReached();
    void setChallengeType(QString value);
    void setRemainingTime(int value);
    void setTime(int value);
    void setSteps(int value);
    void setSpeed(float value);
    void setMaxSpeed(float value);
    void setPower(float value);
    void setEnergy(float value);
    void setEnergyTarget(float value);

  signals:
    void runBatteryMonitor(Controller * controller);
    void runChallenge(Controller * controller);

    void chargePercentageChanged(int value);
    void remainingTimeChanged(int value);
    void scoresChanged(QVariantList scores);
    void canSaveScoreChanged(bool value);
    void timeChanged(int value);
    void stepsChanged(int value);
    void speedChanged(float value);
    void maxSpeedChanged(float value);
    void powerChanged(float value);
    void energyChanged(float value);
    void energyTargetChanged(float value);
    void challengeFinished(bool value);

  public slots:
    void startBatteryMonitor();
    void stopBatteryMonitor();

    void startChallenge();
    void stopChallenge();

    void openLeaderboard();
    void closeLeaderboard();

    void saveScore(QString name = "");
    void deleteDatabases();

  private:
    BatteryMonitor * batteryMonitor;
    QThread * batteryMonitorThread;
    Challenge * challenge;
    QThread * challengeThread;
    std::unique_ptr<Leaderboard> leaderboard;
};  // class Controller


class BatteryMonitor : public QObject {
    Q_OBJECT

  public:
    BatteryMonitor();
    ~BatteryMonitor();

  public slots:
    void start(Controller * controller);
};  // class BatteryMonitor


class Challenge : public QObject {
    Q_OBJECT

  public:
    Challenge();
    ~Challenge();

  public slots:
    void start(Controller * controller);

  private:
    static void monitorStepsPThread(Controller * controller);
    static void monitorPowerPThread(Controller * controller);
};  // class Challenge


class Leaderboard : public QObject {
    Q_OBJECT

  public:
    Leaderboard(Controller * controller);
    ~Leaderboard();

    // function to insert new entry
    int insertRecord(string new_name = "");

  private:
    Controller * controller;
    float score;
    int row;
    string worst;
    string worst_order;

    string db_file;
    sqlite3 * db;
    static int const max_rows;
    static string const query_cmd;
    int failed;
    int status;
    char * messageError;

    static int score_callback(
        void * data, int cols, char * field[], char * col_names[]
    );
    static int contents_callback(
        void * data, int cols, char * field[], char * col_names[]
    );
    static int print_callback(
        void * data, int cols, char * field[], char * col_names[]
    );

    int cleanUpRecords();
    int showContents();
    int readWorstScore();
    int readContents();
};  // class Leaderboard

}  // namespace Backend
