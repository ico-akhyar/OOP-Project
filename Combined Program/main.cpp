#include <iostream>
#include "sstream"
#include "fstream"
#include "windows.h"
using namespace std;

static int auto_id = 0;
static int voterCount = 0;
static int electionId=0;

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

class Candidate : public Person {
private:
    string electionType;
    int party_id;
    int election_id;
    int votes;
    int cons;

public:
    Candidate():votes(0){}
    Candidate(int id, string name, string gender, int age, string address,
              int party_id, int election_id,string type,
              int votes = 0, int choice = 0)
            : Person(id, name, gender, age, address),
              party_id(party_id), election_id(election_id),electionType(type),votes(votes), cons(choice) {}

    void display() const override {
        cout << "Candidate -> ";
        Person::display();
        cout << "Party ID: " << party_id << ", Election ID: " << election_id
             << ", Votes: " << votes << endl;
    }

    void saveToFile() {
        ofstream file("candidates.csv", ios::app);
        if (file.is_open()) {
            file << id << "," << name << "," << gender << "," << age << ","
                 << address << "," << party_id << "," << election_id << ","
                 << electionType << "," << cons << "," << votes << "\n";
            file.close();
        }
    }
    static void loadAllByElection(Candidate candidates[], int& count, string type) {
        ifstream file("candidates.csv");
        string line;
        while (getline(file, line) && count < 100) {
            stringstream ss(line);
            string id, name, electionType, cons, votes;
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, electionType, ',');
            getline(ss, cons, ',');
            getline(ss, votes, ',');

            if (electionType == type) {
                candidates[count].id = stoi(id);
                candidates[count].name = name;
                candidates[count].electionType = electionType;
                candidates[count].cons = stoi(cons);
                candidates[count].votes = stoi(votes);
                count++;
            }
        }
    }

    int getID() const { return id; }
    string getName() const { return name; }
};


class Constituency {
public:
    string District;
    string Area;
    int no;
    string type;
    Candidate candidates[100];
    int candidateCount = 0;

    Constituency() {}
    Constituency(string cname, string area, int num) : District(cname), Area(area), no(num) {}
    void saveToFile() {
        ofstream file("constituencies.csv", ios::app);
        if (file.is_open()) {
            file << District << "," << Area << "," << type << "," << no << "\n";
            file.close();
        }
    }
    static void loadAll(Constituency constituencies[], int& count) {
        ifstream file("constituencies.csv");
        string line;
        while (getline(file, line) && count < 100) {
            stringstream ss(line);
            string name, type, numStr;
            getline(ss, name, ',');
            getline(ss, type, ',');
            getline(ss, numStr, ',');
            constituencies[count++] = Constituency(name, type, stoi(numStr));
        }
    }

    void addCandidate(const Candidate& c);

    void show_candidates(string na_const);
};
// Constiurncy class functions
void Constituency::addCandidate(const Candidate& c) {
    if (candidateCount < 100) {
        candidates[candidateCount++] = c;
    }
    else {
        cout << "Max candidates limit reached!\n";
    }
}
void Constituency::show_candidates(string na_const) {
    ifstream file("constituencies.csv");
    if (!file.is_open()) {
        cout << "Cannot open constituencies.csv file!\n";
        return;
    }

    cout << "Candidates in Constituency: " << na_const << endl;
    string line;
    bool found = false;
    while (getline(file, line)) {
        stringstream ss(line);
        string cname, candName, candID, partyID;
        getline(ss, cname, ',');
        getline(ss, candName, ',');
        getline(ss, candID, ',');
        getline(ss, partyID, ',');

        if (cname == na_const) {
            cout << "Name: " << candName << ", ID: " << candID << ", Party ID: " << partyID << endl;
            found = true;
        }
    }
    file.close();

    if (!found) {
        cout << "No candidates found in this constituency.\n";
    }
}

class Voter : public Person {

    string password;
    string cnic;
    string na_cons;
    string pa_cons;
    int voted_na;
    int voted_pa;
public:
    Voter() {}
    Voter(int id, string name, string gender, int age, string address, string cnic, string pass, string na_cons, string pa_cons, int v_n = 0, int v_p = 0)
            : Person(id, name, gender, age, address), password(pass), cnic(cnic), na_cons(na_cons), pa_cons(pa_cons), voted_na(v_n), voted_pa(v_p) {}
    void display() const override {
        cout << "Voter -> ";
        Person::display();
        cout << "CNIC: " << cnic << endl;
    }

    static int voter_login(Voter voters[]);
    string get_na() { return na_cons; }
    string get_pa() { return pa_cons; }
    void view_pofile() {
        cout << "-------> Your profile <-------\n";
        cout << "-----> NA Constituency: " << get_na() << endl;
        cout << "-----> PA Constituency: " << get_pa() << endl;
        cout << "-------------------\n";
    }


    bool cast_NA_vote();
    bool cast_PA_vote();
};bool Voter::cast_PA_vote() {
    Constituency paConstituencies[100];
    int totalPA = 0;
    Constituency::loadAll(paConstituencies, totalPA);

    // Load candidates for PA
    Candidate paCandidates[100];
    int totalCandidates = 0;
    Candidate::loadAllByElection(paCandidates, totalCandidates, "PA");

    // Check if already voted
    if (voted_pa == 1) {
        cout << "You have already voted in PA elections!" << endl;
        return false;
    }

    // Find voter's constituency
    string constituency;
    ifstream pa_con("PA_const.csv");
    string cn = cnic.substr(0, 5);
    string line;

    if (!pa_con.is_open()) {
        cout << "Failed to open PA_const.csv file!" << endl;
        return false;
    }

    while (getline(pa_con, line)) {
        stringstream ss(line);
        string f_cnic, f_const;
        getline(ss, f_cnic, ',');
        getline(ss, f_const);

        if (f_cnic == cn) {
            constituency = f_const;
            break;
        }
    }
    pa_con.close();

    if (constituency.empty()) {
        cout << "No matching constituency found for your CNIC!" << endl;
        return false;
    }

    // Show candidates for this constituency
    cout << "\nCandidates for PA Constituency: " << constituency << endl;
    cout << "----------------------------------------\n";

    ifstream cand_file("candidates.csv");
    if (!cand_file.is_open()) {
        cout << "Failed to open candidates file!" << endl;
        return false;
    }

    bool candidates_found = false;
    Candidate candidates[100];
    int candidate_count = 0;
    while (getline(cand_file, line)) {
        stringstream ss(line);
        string id, name, gender, age, address, party_id, election_id, electionType, cons, votes;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, gender, ',');
        getline(ss, age, ',');
        getline(ss, address, ',');
        getline(ss, party_id, ',');
        getline(ss, election_id, ',');
        getline(ss, electionType, ',');
        getline(ss, cons, ',');
        getline(ss, votes);

        if (electionType == "PA" && cons == constituency) {
            candidates[candidate_count++] = Candidate(
                    stoi(id), name, gender, stoi(age), address,
                    stoi(party_id), stoi(election_id), electionType,
                    stoi(votes), stoi(cons)
            );
            cout << "ID: " << id << " | Name: " << name << " | Party: " << party_id << endl;
            candidates_found = true;
        }
    }
    cand_file.close();

    if (!candidates_found) {
        cout << "No candidates found for your constituency!" << endl;
        return false;
    }

    // Get voter's choice
    int candidate_id;
    cout << "\nEnter the ID of candidate you want to vote for: ";
    cin >> candidate_id;

    // Verify candidate exists
    bool valid_candidate = false;
    for (int i = 0; i < candidate_count; i++) {
        if (candidates[i].getID() == candidate_id) {
            valid_candidate = true;
            break;
        }
    }

    if (!valid_candidate) {
        cout << "Invalid candidate ID!" << endl;
        return false;
    }

    // Update PA_Results.csv
    bool vote_recorded = false;
    ifstream results_in("PA_Results.csv");
    ofstream results_out("temp_PA_Results.csv");

    // If file doesn't exist, create headers
    if (!results_in.is_open()) {
        results_out << "CandidateID,Name,Votes,Constituency\n";
    } else {
        // Copy existing results and update the voted candidate
        string header;
        getline(results_in, header); // Skip header
        results_out << header << "\n";

        while (getline(results_in, line)) {
            stringstream ss(line);
            string c_id, c_name, c_votes, c_const;

            getline(ss, c_id, ',');
            getline(ss, c_name, ',');
            getline(ss, c_votes, ',');
            getline(ss, c_const);

            if (stoi(c_id) == candidate_id) {
                // Update vote count for this candidate
                results_out << c_id << "," << c_name << ","
                            << (stoi(c_votes) + 1) << "," << c_const << "\n";
                vote_recorded = true;
            } else {
                results_out << line << "\n";
            }
        }
        results_in.close();
    }

    // If candidate wasn't found in existing results, add new entry
    if (!vote_recorded) {
        string c_name;
        for (int i = 0; i < candidate_count; i++) {
            if (candidates[i].getID() == candidate_id) {
                c_name = candidates[i].getName();
                break;
            }
        }
        results_out << candidate_id << "," << c_name << "," << 1 << "," << constituency << "\n";
    }
    results_out.close();

    // Replace old file with updated one
    remove("PA_Results.csv");
    rename("temp_PA_Results.csv", "PA_Results.csv");

    // Update voter's record
    voted_pa = 1;
    ifstream voter_file("voters.csv");
    ofstream temp_voter("temp_voters.csv");

    if (voter_file.is_open() && temp_voter.is_open()) {
        getline(voter_file, line); // Skip header if exists
        temp_voter << "ID,Name,Gender,Age,Address,CNIC,Password,NA_Constituency,PA_Constituency,Voted_NA,Voted_PA\n";

        while (getline(voter_file, line)) {
            stringstream ss(line);
            string v_id, v_name, v_gender, v_age, v_address, v_cnic, v_password, v_na_const, v_pa_const, v_voted_na, v_voted_pa;

            getline(ss, v_id, ',');
            getline(ss, v_name, ',');
            getline(ss, v_gender, ',');
            getline(ss, v_age, ',');
            getline(ss, v_address, ',');
            getline(ss, v_cnic, ',');
            getline(ss, v_password, ',');
            getline(ss, v_na_const, ',');
            getline(ss, v_pa_const, ',');
            getline(ss, v_voted_na, ',');
            getline(ss, v_voted_pa);

            if (stoi(v_id) == id) {
                // Update this voter's record
                temp_voter << id << "," << name << "," << gender << "," << age << ","
                           << address << "," << cnic << "," << password << ","
                           << na_cons << "," << pa_cons << "," << voted_na << "," << 1 << "\n";
            } else {
                temp_voter << line << "\n";
            }
        }
        voter_file.close();
        temp_voter.close();

        remove("voters.csv");
        rename("temp_voters.csv", "voters.csv");
    } else {
        cout << "Error updating voter record!" << endl;
        return false;
    }

    cout << "\nVote cast successfully for candidate " << candidate_id << endl;
    return true;
}

int Voter::voter_login(Voter voters[]) {
    string cnic_input, password_input;
    cout << "Enter your CNIC: ";
    cin >> cnic_input;
    cout << "Enter your password: ";
    cin >> password_input;


    for (int i = 0; i < voterCount; i++) {
        if (voters[i].cnic == cnic_input && voters[i].password == password_input) {
            cout << "Login successful! Welcome " << voters[i].name << endl;
            return i;
        }
    }
    cout << "Invalid CNIC or password!" << endl;
    return -1;
}

bool Voter::cast_NA_vote() {
    Constituency naConstituencies[100];
    int totalNA = 0;
    Constituency::loadAll(naConstituencies, totalNA);

    // Load candidates for NA
    Candidate naCandidates[100];
    int totalCandidates = 0;
    Candidate::loadAllByElection(naCandidates, totalCandidates, "NA");
    // Check if already voted
    if (voted_na == 1) {
        cout << "You have already voted in NA elections!" << endl;
        return false;
    }

    // Find voter's constituency
    string constituency;
    ifstream na_con("NA_const.csv");
    string cn = cnic.substr(0, 5);
    string line;

    if (!na_con.is_open()) {
        cout << "Failed to open NA_const.csv file!" << endl;
        return false;
    }

    while (getline(na_con, line)) {
        stringstream ss(line);
        string f_cnic, f_const;
        getline(ss, f_cnic, ',');
        getline(ss, f_const);

        if (f_cnic == cn) {
            constituency = f_const;
            break;
        }
    }
    na_con.close();

    if (constituency.empty()) {
        cout << "No matching constituency found for your CNIC!" << endl;
        return false;
    }

    // Show candidates for this constituency
    cout << "\nCandidates for NA Constituency: " << constituency << endl;
    cout << "----------------------------------------\n";

    ifstream cand_file("candidates.csv");
    if (!cand_file.is_open()) {
        cout << "Failed to open candidates file!" << endl;
        return false;
    }

    bool candidates_found = false;
    Candidate candidates[100];
    int candidate_count = 0;
    while (getline(cand_file, line)) {
        stringstream ss(line);
        string id, name, gender, age, address, party_id, election_id, electionType, cons, votes;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, gender, ',');
        getline(ss, age, ',');
        getline(ss, address, ',');
        getline(ss, party_id, ',');
        getline(ss, election_id, ',');
        getline(ss, electionType, ',');
        getline(ss, cons, ',');
        getline(ss, votes);

        if (electionType == "NA" && cons == constituency) {
            candidates[candidate_count++] = Candidate(
                    stoi(id), name, gender, stoi(age), address,
                    stoi(party_id), stoi(election_id), electionType,
                    stoi(votes), stoi(cons)
            );
            cout << "ID: " << id << " | Name: " << name << " | Party: " << party_id << endl;
            candidates_found = true;
        }
    }

    // Get voter's choice
    int candidate_id;
    cout << "\nEnter the ID of candidate you want to vote for: ";
    cin >> candidate_id;

    // Verify candidate exists
    bool valid_candidate = false;
    for (int i = 0; i < candidate_count; i++) {
        if (candidates[i].getID() == candidate_id) {
            valid_candidate = true;
            break;
        }
    }

    if (!valid_candidate) {
        cout << "Invalid candidate ID!" << endl;
        return false;
    }

    // Update NA_Results.csv
    bool vote_recorded = false;
    ifstream results_in("NA_Results.csv");
    ofstream results_out("temp_NA_Results.csv");

    // If file doesn't exist, create headers
    if (!results_in.is_open()) {
        results_out << "CandidateID,Name,Votes,Constituency\n";
    } else {
        // Copy existing results and update the voted candidate
        string header;
        getline(results_in, header); // Skip header
        results_out << header << "\n";

        while (getline(results_in, line)) {
            stringstream ss(line);
            string c_id, c_name, c_votes, c_const;

            getline(ss, c_id, ',');
            getline(ss, c_name, ',');
            getline(ss, c_votes, ',');
            getline(ss, c_const);

            if (stoi(c_id) == candidate_id) {
                // Update vote count for this candidate
                results_out << c_id << "," << c_name << ","
                            << (stoi(c_votes) + 1) << "," << c_const << "\n";
                vote_recorded = true;
            } else {
                results_out << line << "\n";
            }
        }
        results_in.close();
    }

    // If candidate wasn't found in existing results, add new entry
    if (!vote_recorded) {
        string c_name;
        for (int i = 0; i < candidate_count; i++) {
            if (candidates[i].getID() == candidate_id) {
                c_name = candidates[i].getName();
                break;
            }
        }
        results_out << candidate_id << "," << c_name << "," << 1 << "," << constituency << "\n";
    }
    results_out.close();

    // Replace old file with updated one
    remove("NA_Results.csv");
    rename("temp_NA_Results.csv", "NA_Results.csv");

    // Update voter's record
    voted_na = 1;
    ifstream voter_file("voters.csv");
    ofstream temp_voter("temp_voters.csv");

    if (voter_file.is_open() && temp_voter.is_open()) {
        getline(voter_file, line); // Skip header if exists
        temp_voter << "ID,Name,Gender,Age,Address,CNIC,Password,NA_Constituency,PA_Constituency,Voted_NA,Voted_PA\n";

        while (getline(voter_file, line)) {
            stringstream ss(line);
            string v_id;
            getline(ss, v_id, ',');

            if (stoi(v_id) == id) {
                // Update this voter's record
                temp_voter << id << "," << name << "," << gender << "," << age << ","
                           << address << "," << cnic << "," << password << ","
                           << na_cons << "," << pa_cons << "," << 1 << "," << voted_pa << "\n";
            } else {
                temp_voter << line << "\n";
            }
        }
        voter_file.close();
        temp_voter.close();

        remove("voters.csv");
        rename("temp_voters.csv", "voters.csv");
    } else {
        cout << "Error updating voter record!" << endl;
        return false;
    }

    cout << "\nVote cast successfully for candidate " << candidate_id << endl;
    return true;
}




//Class Party
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
        }
        else {
            cout << "Max constituencies limit reached!\n";
        }
    }
    //now the admin don't have to declare election each time he login
    void saveToFile() {
        ofstream file("elections.csv", ios::app);
        if (file.is_open()) {
            file << type << "," << date << "\n";
            file.close();
        }
    }

    static Election loadLatestElection() {
        ifstream file("elections.csv");
        string line, lastType, lastDate;
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, lastType, ',');
            getline(ss, lastDate, ',');
        }
        return Election(lastType, lastDate);
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
    Admin(string a="admin", string b="admin") : user(a), pass(b) {}

    bool is_valid(const string& pass);

    bool del_cand();

    bool add_voters();


    Election declare_election()
    {
        string type, date;
        cout << "Enter Election Type (National/Provincial): ";
        cin.ignore();
        getline(cin, type);
        cout << "Enter Election Date (DD/MM/YYYY): ";
        getline(cin, date);

        Election E(type, date);
        cout << "Election  Declared!" << endl;
        return E;
    }

    void create_cons(Election& E)
    {
        string dist, area;
        int no;

        cout << "Enter District: ";
        cin.ignore();
        getline(cin, dist);
        cout << "Enter Area: ";
        getline(cin, area);
        cout << "Enter Constituency No.: ";
        cin >> no;
        if (E.conCount < 100) {
            E.constituencies[E.conCount++] = Constituency(dist, area, no);
            E.constituencies[E.conCount-1].saveToFile();
        }
        else {
            cout << "Maximum constituencies reached!\n";
        }
        ofstream cons("constituencies.csv",ios::app);
        if(cons.is_open())
        {
            cons<<dist<<","<<area<<","<<no;
            cout << "New Constituency Created NA-" << no << " !" << endl;
            cons.close();
        }
        else
            cout<<"Error: Opening file \'constituencies.csv\'";
    }

    void calculate_results(const string& filename, const string& electionType) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Failed to open " << filename << "\n";
            return;
        }

        const int MAX_CONST = 100;
        const int MAX_CAND = 100;

        string constituencies[MAX_CONST];
        int constCount = 0;

        int candidateIDs[MAX_CONST][MAX_CAND];
        string candidateNames[MAX_CONST][MAX_CAND];
        int candidateVotes[MAX_CONST][MAX_CAND];
        int candidateCount[MAX_CONST] = {0};

        string line;
        getline(file, line); // skip header

        while (getline(file, line)) {
            stringstream ss(line);
            string cidStr, name, votesStr, consti;
            getline(ss, cidStr, ',');
            getline(ss, name, ',');
            getline(ss, votesStr, ',');
            getline(ss, consti);

            int cid = stoi(cidStr);
            int votes = stoi(votesStr);

            // Check if constituency exists
            int cIndex = -1;
            for (int i = 0; i < constCount; ++i) {
                if (constituencies[i] == consti) {
                    cIndex = i;
                    break;
                }
            }

            // If not, add new constituency
            if (cIndex == -1 && constCount < MAX_CONST) {
                cIndex = constCount;
                constituencies[constCount++] = consti;
            }

            // Add candidate data
            int candIdx = candidateCount[cIndex];
            if (candIdx < MAX_CAND) {
                candidateIDs[cIndex][candIdx] = cid;
                candidateNames[cIndex][candIdx] = name;
                candidateVotes[cIndex][candIdx] = votes;
                candidateCount[cIndex]++;
            }
        }

        file.close();

        // Print results
        cout << "\n\n=== " << electionType << " Results ===\n";
        for (int i = 0; i < constCount; ++i) {
            int maxVotes = -1;
            int winnerID = -1;
            string winnerName;

            for (int j = 0; j < candidateCount[i]; ++j) {
                if (candidateVotes[i][j] > maxVotes) {
                    maxVotes = candidateVotes[i][j];
                    winnerID = candidateIDs[i][j];
                    winnerName = candidateNames[i][j];
                }
            }

            cout << "Constituency " << constituencies[i] << " Winner: "
                 << winnerName << " (ID: " << winnerID << ") with "
                 << maxVotes << " votes\n";
        }
    }



    void announce_results() {
        cout << "\n=== Election Results ===";
        calculate_results("NA_Results.csv", "National Assembly");
        calculate_results("PA_Results.csv", "Provincial Assembly");
    }

    bool add_candidates(Election& E);

    void admin_menu();
    static bool admin_login();

};
// Admin functions

void Admin::admin_menu() {
    cout << "Welcome to admin (Establishment) menu!" << endl;
    Election E = declare_election();
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


bool Admin::admin_login() {
    string username, password;
    int tries = 3;

    do {
        cout << "=== Admin Login ===\n";
        cout << "Username: ";
        cin.ignore();
        cin>> username;
        cout << "Password: ";

        cin>> password;

        if (username == "admin" && password == "admin") {
            cout << "Login successful!\n";
            return true;
        }
        else {
            cout << "Invalid credentials. Access denied.\n";

            tries--;
        }
    } while (tries > 0);
    return false;
}

bool Admin::is_valid(const string& pass) {
    if (pass.length() < 8) return false;


    bool hasAlpha = false, hasDigit = false, hasSpecial = false;
    string specialChars = "!@#$&";

    for (char ch : pass) {
        if (isalpha(ch)) hasAlpha = true;
        else if (isdigit(ch)) hasDigit = true;
        else if (specialChars.find(ch) != string::npos) hasSpecial = true;
    }

    return hasAlpha && hasDigit && hasSpecial;
}

bool Admin::add_candidates(Election& E) {
    system("cls");

    int age = 0, party_id, votes = 0;
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
    // Show available constituencies for the election type
    cout << "\nAvailable " << E.type << " Constituencies:\n";
    for (int i = 0; i < E.conCount; i++) {
        if (E.constituencies[i].type == E.type) {
            cout << i + 1 << ". " << E.constituencies[i].District
                 << " (" << E.type << "-" << E.constituencies[i].no << ")\n";
        }
    }

    // Select constituency
    int choice;
    do {
        cout << "Choose Constituency Number (1-" << E.conCount << "): ";
        cin >> choice;
    } while (choice < 1 || choice > E.conCount);
    Constituency& selectedCons = E.constituencies[choice - 1];


    Candidate X(++auto_id,name,gender,age,address,party_id,++electionId,E.type,0,selectedCons.no);


    selectedCons.addCandidate(X);
    X.saveToFile();

    cout << "New candidate added to " << E.type << "-" << selectedCons.no << endl;
    return true;
}


bool Admin::del_cand() {
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
            }
            else {
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
        }
        else
            return true;
    }
    else {
        cout << "\t--> Error opening file!" << endl;
    }
    return false;
}
bool Admin::add_voters() {
    system("cls");
    string name, gender, address, cnic, pass, p1, p2;
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
        for (char c : cnic) {
            if (!isdigit(c)) {
                isValid = false;
                break;
            }
        }
        if (cnic.length() != 13 || !isValid) {
            cout << "Invalid CNIC!\n";
            cout << "Try again: ";
            getline(cin, cnic);
        }
        else
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


    Voter V(++auto_id, name, gender, age, address, cnic, pass, na_cons, pa_cons,0,0);


    ofstream can("voters.csv", ios::app);
    if (can.is_open()) {
        can << auto_id << "," << name << "," << gender << "," << age << ","
            << address << "," << cnic << "," << p1 << "," << na_cons << ","<< pa_cons <<"0"<<"0"<< "\n";
        can.close();
    }
    else {
        cout << "Failed to open voters.csv file!" << endl;
        return false;
    }
    return true;
}

//not
void load_voters(Voter voters[]) {
    ifstream file("voters.csv");
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, gender, age, address, cnic, password, na_const, pa_const, voted_na, voted_pa;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, gender, ',');
        getline(ss, age, ',');
        getline(ss, address, ',');
        getline(ss, cnic, ',');
        getline(ss, password, ',');
        getline(ss, na_const, ',');
        getline(ss, pa_const, ',');
        getline(ss, voted_na, ',');
        getline(ss, voted_pa, ',');

        voters[voterCount] = Voter(stoi(id), name, gender, stoi(age), address, cnic, password, na_const, pa_const, stoi(voted_na), stoi(voted_pa));
        voterCount++;
    }
    file.close();
}
int main() {
    int opt;
    do{
        cout << "Welcome to Election Commision" << endl;
        cout << "Select-->\n";
        cout << "1) Admin Menu\n2) Voter Menu\n3) Exit\n";
        cin >> opt;
        switch (opt) {
            case 1: {
                if (Admin::admin_login()) {
                    Admin a("admin","admin");
                    a.admin_menu();
                    break;
                }
            }
                break;
            case 2:
            {
                Voter voters[10];
                load_voters(voters);
                int choice;
                int voter_index=Voter::voter_login(voters);
                if (voter_index != -1) {
                    do {

                        cout << "-->Select: \n1)Cast NA Vote\n2)Cast PA Vote\n3)View profile\n4)Log Out" << endl;
                        cin >> choice;
                        switch (choice) {
                            case 1:
                                voters[voter_index].cast_NA_vote();
                                break;
                            case 2:
                                voters[voter_index].cast_PA_vote();
                                break;
                            case 3:
                                break;
                            case 4:
                                cout << "Logging Out" << endl;
                                break;
                            default:
                                cout << "Invalid Choice!\n";
                        }
                    } while (choice != 4);
                }
                break;
            }
            case 3:
                cout << "Exiting!" << endl;
                break;
            default:
                cout << "Invalid Option!" << endl;
        }
    } while (opt!=3);
    return 0;
}