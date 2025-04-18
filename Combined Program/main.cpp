#include<iostream>
#include"cstring"
#include"string"
#include"sqlite3.h"
#include"cctype"
#include"iomanip"
#include"cstdlib"
using namespace std;

class person {
protected:
	string name;
	int age;
	string cnic;
public:
	person(string n = " ", int a = 00, string c = " ") :name(n), age(a), cnic(c) {}
	void setName(string n) { name = n; }
	string getName() { return name; }

	void setAge(int a) { age = a; }
	int getAge() { return age; }

	void setCNIC(string c) { cnic = c; }
	string getCNIC() { return cnic; }

};
class voter :public person {
	string district;
	bool voted;
public:
	voter(string name,int age,string cnic):person(name,age,cnic){}
	
	bool cast_vote();
};
class candidate :public person {
	string party;
	string symbol;
	int votes;
public:

};
void ins_voter(sqlite3* db, const string& tableName, const string& name, const string& cnic, int a) {

	char* errMsg;
	string ins_query = "INSERT INTO " + tableName + " (name,cnic,age) VALUES ('" + name + "','" + cnic + "',"+ to_string(a) + "); ";

	int rc = sqlite3_exec(db, ins_query.c_str(), NULL, 0, &errMsg);
	if (rc != 0) {
		cout << "Insert failed: " << errMsg << endl;
		sqlite3_free(errMsg);
	}
	else
		cout << "Inserted successfully.\n";
}


bool add_voters(sqlite3* db) {
	system("cls");
	string name;
	int age;
	string cnic;
	int d_id;
	bool name_flag = false;
	while (!name_flag) {
		cout << "Enter name: ";
		cin.ignore();
		getline(cin, name);
		for (char c : name) {
			if (!isalpha(c)) {
				cout << "Name can only contain alphabhet!" << endl;
				name_flag = false;
				break;
			}
		}
		name_flag = true;
	}
	cout << "Enter age: ";
	// Validating input ta k pchli dfa jesa na ho
	while (!(cin >> age)) {
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "\nInvalid input! Enter an integer: ";
	}

	cout << "Enter CNIC (14 characters long and no dashes): ";
	cin.ignore();
	getline(cin, cnic);
	while (cnic.length() != 14) {
		cerr << "CNIC must be 14 characters long!" << endl;
		cout << "Enter CNIC:  ";
		cin.ignore();
		getline(cin, cnic);
	}
	bool flag = true;
	for (char c : cnic) {
		if (c < 0 || c>9) {
			flag = false;
			break;
		}

	}
	int opt;

	while (!flag) {
		bool n_flag = true;
		cout << "Press 1 to try again or any other key to quit!" << endl;
		cin >> opt;
		switch (opt) {
		case 1:
			cout << "You have one more attempt\nEnter CNIC: ";
			cin.ignore();
			getline(cin, cnic);
			if (cnic.length() != 14) {
				return false;
			}
			else
				for (char c : cnic) {
					if (c < 0 || c>9) {
						n_flag = false;
						return false;
					}
				}
			break;
		case !1:
			return false;

		}
	}

	cout << "Voter added successfully!" << endl;
	 
	return true;
}
void add_candidates() {

}
void print_menu() {

	cout << endl;
	cout << "                          __________      |||                   |||          ||||            " << endl;
	cout << "                         |||========      ||| |||           ||| |||       |||    |||      " << endl;
	cout << "                         |||              |||  |||         |||  |||      |||             " << endl;
	cout << "                         |||________      |||    |||     |||    |||       |||            " << endl;
	cout << "                         |||========      |||      ||| |||      |||          |||            " << endl;
	cout << "                         |||              |||         ||        |||             |||     " << endl;
	cout << "                         |||              |||                   |||              |||     " << endl;
	cout << "                         |||________      |||                   |||       |||    |||     " << endl;
	cout << "                         |||========      |||                   |||          ||||           " << endl;
	cout << endl;
	cout << "  |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| " << endl;
	cout << "  >>>>>>>>>>>>>>>>>>>>>>>>>>   E L E C T I O N      M A N A G E M E N T     S Y S T E M   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< " << endl;
	cout << "  |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| " << endl;
	cout << endl;
}

int main() {
	print_menu();
	sqlite3* db;
	string dist_table="CREATE TABLE IF NOT EXISTS DISTRICTS(ID INT PRIMARY KEY AUTOINCREMENT, District VARCHAR(10) NOT NULL )"
	string voter_table = "Create TABLE IF NOT EXISTS VOTERS_DATA(ID INT PRIMARY KEY AUTOINCREMENT, Name TEXT NOT NULL,CNIC VARCHAR(20) NOT NULL UNIQUE,Age INT NOT NULL,District_id int NOT NULL,FOREIGN KEY(District_id) REFRENCES DISTRICTS(ID))";

}