#include <iostream>
#include "db.h"

using namespace std;

sqlite3* DB;


int main() {
    if (initializeDatabase("Database.db",DB)) {
        cout<<"Database initialized !"<<endl;
    } else {
        cout << "DataBase initialization failed!" << endl;
    }

    cout << "Election Commision" << endl;
    cout << "Select-->\n";
    cout << "1) Admin Menu\n2) Voter Menu\n3) Candidate Menu\n4) Exit\n";
    int opt;
    cin >> opt;
    switch (opt) {
        case 1:
            admin_menu();
            break;
        case 2:
            voter_menu();
            break;
        case 3:
            candidate_menu();
            break;
        case 4:
            cout << "Exiting!" << endl;
            break;
        default:
            cout << "Invalid Option!" << endl;
    }
    sqlite3_close(DB);
    return 0;
}