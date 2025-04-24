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
};

class Candidate : public Person {
private:
    int party_id;
    int election_id;
    int votes;

public:
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

bool add_voters()
{
        system("cls");
        string name,gender,address, cnic;
        int age;

        while (true) {
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);
            bool valid = true;
            for (char c : name) {
                if (!isalpha(c) && c != ' ') {
                    cout << "Name can only contain alphabets and spaces!\n";
                    valid = false;
                    break;
                }
            }
            if (valid) break;
        }

        cout << "Enter age: ";
        while (!(cin >> age)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Enter an integer: ";
        }

        cin.ignore();
        cout << "Enter CNIC (13 digits, no dashes): ";
        getline(cin, cnic);

        int attempts = 0;
        while (attempts<3)
        {
            bool isValid = true;
            for (char c : cnic) {
                if (!isdigit(c)) {
                    isValid = false;
                    break;
                }
            }

            if(cnic.length() != 14 || !isValid)
            {
                    cout << "Invalid CNIC!\n";
                    cout << "Try again: ";
                    getline(cin, cnic);
                    attempts++;
            }
        }
   
    string code = cnic.substr(0, 5);
    string line, ccode, na_cons,pa_cons;

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

    Voter(++auto_id,name,gender,age,address,cnic,na_cons,pa_cons);

    ofstream can("voters.csv",ios::app);
    if(can.is_open()) {
        can << auto_id << "," << name << "," << gender << "," << age << "," << address << "," << cnic <<"\n";
        can.close();
    }
    else
    {
        cout<<"Failed to open voters.csv file!"<<endl;
        return false;
    }
    return true;
}

bool add_candidates() {
    system("cls");

    int age=0, party_id, election_id,votes=0;   //Data to save
    string name, gender, address;

    bool valid_name = false;
    while (!valid_name) {
        cout << "Enter candidate name: ";
        cin.ignore();
        getline(cin, name);
        valid_name = true;
        for (char c : name) {
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

    while (age<18 || age > 100) {
        cout<<"Enter age:";
        cin>>age;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid age! Must be a number between 18 and 100: ";
    }

    cin.ignore();
    cout << "Enter address: ";
    getline(cin, address);


    while (party_id<1 || party_id>25) {
        cin.ignore(10000, '\n');
        cout << "Invalid input! Enter a valid integer: ";
    }

    cout << "Enter election ID: ";
    while (!(cin >> election_id)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input! Enter a valid integer: ";
    }

    Candidate(++auto_id,name,gender,age,address,party_id,election_id,votes);

    ofstream can("candidates.csv",ios::app);
    if(can.is_open()) {
        can << auto_id << "," << name << "," << gender << "," << age << "," << address << "," << party_id << ","
            << election_id << "," << votes << "\n";
        can.close();
    }
    else
    {
        cout<<"Failed to open candidate.csv file!"<<endl;
        return false;
    }
    return true;
}


void admin_menu() {
    cout << "Welcome to admin (Establishment) menu!" << endl;
    cout << "Select-->\n";
    cout << "1) Add voters\n2) Add candidates\n3) View statistics\n4) Remove candidates\n";
    int opt;
    cin >> opt;
    switch (opt) {
        case 1:
            if (add_voters())
                cout << "Successfully completed!" << endl;
            else
                cout << "Couldn't complete action!" << endl;
            break;
        case 2:
            if (add_candidates())
                cout << "Successfully completed!" << endl;
            else
                cout << "Couldn't complete action!" << endl;
            break;
        case 3:
            show_stats();
            break;
        case 4:
            if (remove_candidates())
                cout << "Successfully completed!" << endl;
            else
                cout << "Couldn't complete action!" << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
    }
}

bool admin_login() {
    string username, password;

    cout << "=== Admin Login ===\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (username == "admin" && password == "admin") {
        cout << "Login successful!\n";
        admin_menu();
        return true;
    } else {
        cout << "Invalid credentials. Access denied.\n";
        return false;
    }
}

void voter_menu()
{

}


void voter_login() {
    bool flag = true;
    int cnt = 3;
    int count = 0;
    system("cls");
    string line;
    string user;
    string pass;
    string s_user;
    string s_pass;
    do
    {
        cout<<endl;
        cout << "\t||====+---> Enter Your Login Info  <---+====||" << endl;
        cout << "\t--> Username(0 to go back): ";
        if (count == 0) {
            cin.ignore();
            count++;
        }
        getline(cin, user);
        cout << "\t--> Password(0 to go back): ";
        getline(cin, pass);
        ifstream Data("login_info.txt");
        if (Data.is_open()) {
            while (getline(Data, line)) {
                s_user = line;
                if (getline(Data, line)) {
                    s_pass = line;
                    if (user == s_user && pass == s_pass) {
                        flag = true;
                        cout << "\t:::: Login Successful! ::::" << endl;
                        cout << "\t-_-_-_-_-_-_-_-_-_-_-_-_-_-" << endl;
                        Sleep(1000);
                        voter_menu();
                        flag = false;
                        break;
                    } else if (user == "0" && pass == "0") {
                        flag = false;

                    }
                }
            }
            if (user=="0" && pass=="0")
                break;
            if (user != s_user || pass != s_pass) {
                cout << "\t||====+===>           ERROR         <===+====||" << endl;
                cout << "\t||--->   Incorrect Username Or Password  <---||" << endl;
                cout << "\t||====+===>     Please Try Again    <===+====||" << endl;
                cout << "\t||====+===>  Remaining Attempts " << cnt << "   <===+====||" << endl;
                cnt--;
                //Sleep(2000);
                flag = true;

            }

        }
    }while(flag && cnt>0);
}


int main() {
    cout << "Election Commision" << endl;
    cout << "Select-->\n";
    cout << "1) Admin Menu\n2) Voter Menu\n3) Candidate Menu\n4) Exit\n";
    int opt;
    cin >> opt;
    switch (opt) {
        case 1:
            admin_login();
            break;
        case 2:
            voter_login();
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
    return 0;
}