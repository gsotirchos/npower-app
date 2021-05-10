#include "backend.hpp"

#include <iostream>


namespace Backend {

//// Leaderboard class ////
Leaderboard::Leaderboard(Controller * the_controller)
    : controller{the_controller},
      score{0},
      row{0},
      failed{-1},
      status{0}
{
    // determine worst score criteria and database file according to challenge type
    if (controller->challenge_type == "max power") {
        worst = "MIN";
        worst_order = "ASC";
        db_file = "max_power.db";
        score = controller->energy;
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
    if (score > 0) {
        if (row < max_rows) {
            controller->can_save_score = true;
        } else if (score > controller->worst_score) {
            controller->can_save_score = true;
        } else {
            controller->can_save_score = false;
        }
    } else {
        controller->can_save_score = false;
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
    const char * name = field[0] ? field[0] : "";
    const char * score = field[1] ? field[1] : "";
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
    // format score to single decimal float
    char * str_score;
    std::sprintf(str_score, "%.2f", score);
    std::sscanf(str_score, "%f", &score);

    // insert entry
    std::cout << "Inserting: NAME: " << new_name << " SCORE: " << str_score << endl;
    string insert_cmd = (
        "INSERT INTO players VALUES('" + new_name + "', " + str_score + ");"
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

    // update data
    readContents();
    readWorstScore();

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
