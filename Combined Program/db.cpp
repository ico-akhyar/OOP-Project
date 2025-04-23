#include "db.h"

using namespace std;

bool executeQuery(sqlite3* db_name,const char* sql)
{
    char* errMsg;
    int out = sqlite3_exec(db_name, sql, nullptr, nullptr, &errMsg);
    if (out != SQLITE_OK) {
        cout << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}


const char* party_table = R"(
    CREATE TABLE IF NOT EXISTS parties (
        party_id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        symbol TEXT NOT NULL,
        founder TEXT,
        nominated_pm TEXT
    );
)";

const char* candidate_table = R"(
    CREATE TABLE IF NOT EXISTS candidates (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        int constitution,
        election_id INTEGER,
        party_id INTEGER,
        name TEXT NOT NULL,
        votes INTEGER NOT NULL DEFAULT 0,
        FOREIGN KEY(election_id) REFERENCES elections(id),
        FOREIGN KEY(party_id) REFERENCES parties(party_id)
    );
)";

const char* voter_table = R"(
    CREATE TABLE IF NOT EXISTS Voter (
        voter_id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        cnic TEXT UNIQUE NOT NULL,
        gender TEXT,
        age INTEGER,
        address TEXT
    );
)";

const char* vote_table = R"(
    CREATE TABLE IF NOT EXISTS Vote (
        vote_id INTEGER PRIMARY KEY AUTOINCREMENT,
        voter_id INTEGER,
        election_id INTEGER,
        candidate_id INTEGER,
        vote_time TEXT,
        FOREIGN KEY(voter_id) REFERENCES Voter(voter_id),
        FOREIGN KEY(election_id) REFERENCES Election(election_id),
        FOREIGN KEY(candidate_id) REFERENCES Candidate(candidate_id)
    );
)";

const char* result_table = R"(
    CREATE TABLE IF NOT EXISTS Result (
        result_id INTEGER PRIMARY KEY AUTOINCREMENT,
        election_id INTEGER,
        candidate_id INTEGER,
        total_votes INTEGER DEFAULT 0,
        FOREIGN KEY(election_id) REFERENCES Election(election_id),
        FOREIGN KEY(candidate_id) REFERENCES Candidate(candidate_id)
    );
)";


bool initializeDatabase(const string& db_name,sqlite3* db) {
    int exit = sqlite3_open(db_name.c_str(), &db);
    if (exit) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    if (!executeQuery(db,voter_table)) return false;
    if (!executeQuery(db,candidate_table)) return false;
    if (!executeQuery(db,party_table)) return false;
    if (!executeQuery(db,result_table)) return false;
    if (!executeQuery(db,vote_table)) return false;

    cout << "All tables created successfully!" << endl;
    return true;
}



