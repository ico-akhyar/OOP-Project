#include <iostream>
#include "sstream"
#include "fstream"
#include "windows.h"
using namespace std;

static int auto_id=0;

class Person {
protected:
    int id;
    string name;
    string gender;
    int age;
    string address;

public:
    Person() = default;

    Person(int id, string name, string gender, int age, string address)
            : id(id), name(name), gender(gender), age(age), address(address) {}

    virtual void display() const {
        cout << "Name: " << name << ", Gender: " << gender
             << ", Age: " << age << ", Address: " << address << endl;
    }
};

class Voter : public Person {
private:
    string pass;
    string cnic;
    string na_cons;
    string pa_cons;
public:
    Voter(int id, string name, string gender, int age, string address, string cnic,string na_cons,string pa_cons)
            : Person(id, name, gender, age, address), cnic(cnic),na_cons(na_cons),pa_cons(pa_cons) {}

    void display() const override {
        cout << "Voter -> ";
        Person::display();
        cout << "CNIC: " << cnic << endl;
    }

    static bool voter_login()
    {
        string cnic, password;
        cout << "Enter your CNIC: ";
        cin >> cnic;
        cout << "Enter your password: ";
        cin >> password;

        ifstream file("voters.csv");

        if (!file.is_open()) {
            cout << "Error opening the file!" << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, name, gender, age, address, file_cnic, file_password, na_const, pa_const;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, gender, ',');
            getline(ss, age, ',');
            getline(ss, address, ',');
            getline(ss, file_cnic, ',');
            getline(ss, file_password, ',');
            getline(ss, na_const, ',');
            getline(ss, pa_const, ',');

            if (cnic == file_cnic && password == file_password) {
                cout << "Login successful!" << endl;
                file.close();
                return true;
            }
        }

        cout << "Invalid CNIC or password!" << endl;
        file.close();
        return false;
    }
};

class Candidate : public Person {
private:
    int party_id;
    int election_id;
    int votes;

public:
    Candidate() {}
    Candidate(int id, string name, string gender, int age, string address,
              int party_id, int election_id, int votes = 0)
            : Person(id, name, gender, age, address),
              party_id(party_id), election_id(election_id), votes(votes) {}

    void display() const override {
        cout << "Candidate -> ";
        Person::display();
        cout << "Party ID: " << party_id << ", Election ID: " << election_id
             << ", Votes: " << votes << endl;
    }

    int getID() const { return id; }
    string getName() const { return name; }
};


class Constituency {
public:
    string District;
    string Area;
    int no;
    Candidate candidates[100];
    int candidateCount = 0;

    Constituency() {}
    Constituency(string cname,string area,int num) : District(cname),Area(area),no(num) {}

    void addCandidate(const Candidate& c) {
        if (candidateCount < 100) {
            candidates[candidateCount++] = c;
        } else {
            cout << "Max candidates limit reached!\n";
        }
    }
};

class Party {
private:
    int party_id;
    string name;
    string symbol;
    string founder;
    string nominated_pm;

public:
    Party(int id, string name, string symbol, string founder, string pm)
            : party_id(id), name(name), symbol(symbol), founder(founder), nominated_pm(pm) {}

    void display() const {
        cout << "Party: " << name << " | Symbol: " << symbol
             << " | Founder: " << founder << " | Nominated PM: " << nominated_pm << endl;
    }
};

class Election {
public:
    string type;
    string date;
    Constituency constituencies[100];
    int conCount = 0;

    Election() {}
    Election(string t, string d) : type(t), date(d) {}


    void addConstituency(const Constituency& c) {
        if (conCount < 100) {
            constituencies[conCount++] = c;
        } else {
            cout << "Max constituencies limit reached!\n";
        }
    }

    void printAllCandidates() {
        for (int i = 0; i < conCount; i++) {
            cout << "Constituency: " << constituencies[i].no << endl;
            for (int j = 0; j < constituencies[i].candidateCount; j++) {
                cout << "  - " << constituencies[i].candidates[j].getName()
                     << " (ID: " << constituencies[i].candidates[j].getID() << ")\n";
            }
        }
    }
};


class Admin {
private:
    string user;
    string pass;
public:
    Admin(string a, string b)
            : user(a), pass(b) {}

    bool is_valid(const string &pass) {
        if (pass.length() < 8) return false;

        bool hasAlpha = false, hasDigit = false, hasSpecial = false;
        string specialChars = "!@#$&";

        for (char ch: pass) {
            if (isalpha(ch)) hasAlpha = true;
            else if (isdigit(ch)) hasDigit = true;
            else if (specialChars.find(ch) != string::npos) hasSpecial = true;
        }

        return hasAlpha && hasDigit && hasSpecial;
    }


    bool del_cand() {
        system("cls");
        int del_id;
        string id;
        cout << "\t--> Enter Candidate ID to delete: ";
        cin >> del_id;

        ifstream in_file("candidates.csv");
        ofstream tmp_file("temp_candidates.csv");

        string line;
        bool found = false;

        if (in_file.is_open() && tmp_file.is_open()) {
            while (getline(in_file, line)) {
                stringstream ss(line);
                getline(ss, id, ',');

                if (stoi(id) != del_id) {
                    tmp_file << line << endl;
                } else {
                    found = true;
                    cout << "\n\t||====+===>     CANDIDATE DELETED     <===+====||\n";
                    Sleep(1000);
                }
            }

            in_file.close();
            tmp_file.close();

            remove("candidates.csv");
            rename("temp_candidates.csv", "candidates.csv");

            if (!found) {
                cout << "\n\t--> No candidate found with ID: " << del_id << endl;
            } else
                return true;
        } else {
            cout << "\t--> Error opening file!" << endl;
        }
        return false;
    }

    bool add_voters() {
        system("cls");
        string name, gender, address, cnic, pass, p1, p2;
        int age;

        while (true) {
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);
            bool valid = true;
            for (char c: name) {
                if (!isalpha(c) && c != ' ') {
                    cout << "Name can only contain alphabets and spaces!\n";
                    valid = false;
                    break;
                }
            }
            if (valid) break;
        }

        cout << "Enter age: ";
        while (!(cin >> age) || (age < 18 || age > 80)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Enter an integer: ";
        }

        cin.ignore();
        while (true) {
            cout << "Enter gender (Male/Female/Other): ";
            getline(cin, gender);
            if (gender == "Male" || gender == "Female" || gender == "Other") break;
            cout << "Invalid gender! Type exactly: Male, Female, or Other\n";
        }

        cout << "Enter address: ";
        getline(cin, address);

        cout << "Enter CNIC (13 digits, no dashes): ";
        getline(cin, cnic);

        int attempts = 0;
        while (attempts < 3) {
            bool isValid = true;
            for (char c: cnic) {
                if (!isdigit(c)) {
                    isValid = false;
                    break;
                }
            }
            if (cnic.length() != 13 || !isValid) {
                cout << "Invalid CNIC!\n";
                cout << "Try again: ";
                getline(cin, cnic);
            } else
                break;
            attempts++;
        }

        string code = cnic.substr(0, 5);
        string line, ccode, na_cons, pa_cons;

        ifstream na_file("NA_const.csv");
        if (na_file.is_open()) {
            bool found = false;
            while (getline(na_file, line)) {
                stringstream ss(line);
                getline(ss, ccode, ',');
                getline(ss, na_cons);

                if (ccode == code) {
                    found = true;
                    break;
                }

            }
            na_file.close();
        }

        ifstream pa_file("PA_const.csv");
        if (pa_file.is_open()) {
            bool found = false;

            while (getline(pa_file, line)) {
                stringstream ss(line);
                getline(ss, ccode, ',');
                getline(ss, pa_cons);

                if (ccode == code) {
                    found = true;
                    break;
                }

            }
            pa_file.close();
        }

        int chance = 0;
        again:
        cout << "Create New password :";
        getline(cin, p1);
        while (!is_valid(p1)) {
            cout
                    << "1. Password should be of minimum 8 characters.\n2. Password must contain a special character ! @ # $ &.\n3. Password must be a combination of letters and numebrs.\n";
            cout << "Create New password : ";
            getline(cin, p1);
        }
        cout << "Confirm password : ";
        getline(cin, p2);
        if (p1 != p2 && chance < 3) {
            chance++;
            goto again;
        }


        Voter V(++auto_id, name, gender, age, address, cnic, na_cons, pa_cons);

        ofstream can("voters.csv", ios::app);
        if (can.is_open()) {
            can << auto_id << "," << name << "," << gender << "," << age << "," << address << "," << cnic << "," << p1 << "," << na_cons << "," << pa_cons << "\n";
            can.close();
        } else {
            cout << "Failed to open voters.csv file!" << endl;
            return false;
        }
        return true;
    }

    Election declare_election()
    {
        string type,date;
        cout << "Enter Election Type (National/Provincial): ";
        cin.ignore();
        getline(cin, type);
        cout << "Enter Election Date (DD/MM/YYYY): ";
        getline(cin, date);

        Election E(type,date);
        cout<<"Election 'E' Declared!"<<endl;
        return E;
    }

    void create_cons(Election& E)
    {
        string dist,area;
        int no;

        cout << "Enter District: ";
        cin.ignore();
        getline(cin, dist);
        cout << "Enter Area: ";
        getline(cin, area);
        cout << "Enter Constituency No.: ";
        cin>>no;

        Constituency C(dist,area,no);
        E.constituencies[no-1]=C;
        cout<<"New Constituency Created NA-"<<no<< " !"<<endl;
    }


    bool add_candidates(Election& E) {
        system("cls");

        int age = 0, party_id, votes = 0;
        string name, gender, address;

        bool valid_name = false;
        while (!valid_name) {
            cout << "Enter candidate name: ";
            cin.ignore();
            getline(cin, name);
            valid_name = true;
            for (char c: name) {
                if (!isalpha(c) && c != ' ') {
                    cout << "Name must only contain alphabets and spaces!\n";
                    valid_name = false;
                    break;
                }
            }
        }

        while (true) {
            cout << "Enter gender (Male/Female/Other): ";
            getline(cin, gender);
            if (gender == "Male" || gender == "Female" || gender == "Other") break;
            cout << "Invalid gender! Type exactly: Male, Female, or Other\n";
        }

        cout << "Enter age:";
        cin >> age;
        while (age < 18 || age > 100) {
            cin.ignore(10000, '\n');
            cout << "Invalid age! Must be a number between 18 and 100\n";
            cout << "Enter age:";
            cin >> age;
        }

        cin.ignore();
        cout << "Enter address: ";
        getline(cin, address);

        cout << "Enter Party-id:";
        cin >> party_id;
        while (party_id < 1 || party_id > 25) {
            cin.ignore(10000, '\n');
            cout << "Invalid input!";
            cout << " Enter a valid Party-id:";
            cin >> party_id;
        }

        for (int i = 0; i < 100; i++) {
            cout << i + 1 << ". " << E.constituencies[i].District << "-" << E.constituencies[i].Area
                 << " (NA-" << E.constituencies[i].no << ")\n";
        }

        int choice;
        repeat:
        cout << "Choose Constituency Number: ";
        cin >> choice;
        if (choice < 1 || choice > 100) {
            cout << "Invalid choice!\n";
            goto repeat;
        }

        else {
            Candidate X(++auto_id, name, gender, age, address, party_id, votes);
            E.constituencies[choice - 1].addCandidate(X);

            ofstream can("candidates.csv", ios::app);
            if (can.is_open()) {
                can << auto_id << "," << name << "," << gender << "," << age << "," << address << "," << party_id << ","
                    << votes << "\n";
                can.close();
                cout << "New candidate 'X' added" << endl;
            } else {
                cout << "Failed to open candidate.csv file!" << endl;
                return false;
            }
        }
        return true;
    }

    void admin_menu() {
        cout << "Welcome to admin (Establishment) menu!" << endl;
        Election E=declare_election();
        int opt;
        do {
            cout << "1) Add voters\n2) Add candidates\n3) Declare Elections\n4) Remove candidates\n5) Create Constitution\n0) Exit";
            cin >> opt;
            switch (opt) {
                case 0:
                    cout << "Exiting..." << endl;
                    break;
                case 1:
                    if (add_voters())
                        cout << "Successfully completed!" << endl;
                    else
                        cout << "Couldn't complete action!" << endl;
                    break;
                case 2:
                    if (add_candidates(E))
                        cout << "Successfully completed!" << endl;
                    else
                        cout << "Couldn't complete action!" << endl;
                    break;
//                case 3:
////
//                    break;
                case 4:
                    if (del_cand())
                        cout << "Successfully completed!" << endl;
                    else
                        cout << "Couldn't complete action!" << endl;
                    break;
                case 5:
                    create_cons(E);
                    break;
                default:
                    cout << "Invalid choice!" << endl;
            }
        } while (opt != 0);
    }


    static bool admin_login() {
        string username, password;
        int tries = 3;

        do {
            cout << "=== Admin Login ===\n";
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;

            if (username == "admin" && password == "admin") {
                cout << "Login successful!\n";
                Admin a(username, password);
                a.admin_menu();
                return true;
            } else {
                cout << "Invalid credentials. Access denied.\n";
            }
        } while (tries > 0);
        return false;
    }
};


void voter_menu() {

}


void voter_login() {
    bool flag = true;
    int cnt = 3;
    int count = 0;
    system("cls");
    string line, user, pass, s_user, s_pass, cnic;

    do {
        cout << endl;
        cout << "\t||====+---> Enter Your Login Info  <---+====||" << endl;
        cout << "\t--> Username(0 to go back): ";
        if (count == 0) {
            cin.ignore();
            count++;
        }
        getline(cin, user);
        cout << "\t--> Password(0 to go back): ";
        getline(cin, pass);

        if (user == "0" && pass == "0") {
            flag = false;
            break;
        }

        ifstream Data("voters_login.csv");
        bool loginSuccess = false;

        if (Data.is_open()) {
            while (getline(Data, line)) {
                stringstream ss(line);
                getline(ss, s_user, ',');
                getline(ss, s_pass, ',');
                getline(ss, cnic, ','); // optional use

                if (user == s_user && pass == s_pass) {
                    loginSuccess = true;
                    cout << "\t:::: Login Successful! ::::" << endl;
                    cout << "\t-_-_-_-_-_-_-_-_-_-_-_-_-_-" << endl;
                    Sleep(1000);
                    voter_menu();
                    flag = false;
                    break;
                }
            }
            Data.close();
        }

        if (!loginSuccess) {
            cout << "\t||====+===>           ERROR         <===+====||" << endl;
            cout << "\t||--->   Incorrect Username Or Password  <---||" << endl;
            cout << "\t||====+===>     Please Try Again    <===+====||" << endl;
            cout << "\t||====+===>  Remaining Attempts " << cnt << "   <===+====||" << endl;
            cnt--;
            flag = true;
        }

    } while (flag && cnt > 0);
}


int main() {
    cout << "Election Commision" << endl;
    cout << "Select-->\n";
    cout << "1) Admin Menu\n2) Voter Menu\n3) Exit\n";
    int opt;
    cin >> opt;
    switch (opt) {
        case 1:
            Admin::admin_login();
            break;
        case 2:
            voter_login();
            break;
        case 3:
            cout << "Exiting!" << endl;
            break;
        default:
            cout << "Invalid Option!" << endl;
    }
    return 0;
}