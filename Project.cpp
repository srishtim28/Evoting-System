#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

// Structures
struct Voter {
    int voterID;
    string name;
    int age;
    string pin;
    bool hasVoted = false;
};

struct Candidate {
    int candidateID;
    string name;
    int votes = 0;
};

// Databases
vector<Voter> voters; 
vector<Candidate> candidates;

// Function declarations
void loadVoters();
void saveVoters();
void loadCandidates();
void saveCandidates();

void registerVoter();
void addCandidate();
void castVote();
void displayResult();
bool authenticateVoter(int voterID, string pin);
Voter* findVoter(int voterID);
Candidate* findCandidate(int candidateID);

int main() {
    loadVoters();
    loadCandidates();

    int choice;
    while (true) {
        cout << "\n=== E-Voting System ===\n";
        cout << "1. Register Voter\n";
        cout << "2. Add Candidate\n";
        cout << "3. Cast Vote\n";
        cout << "4. Show Result\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: registerVoter(); break;
            case 2: addCandidate(); break;
            case 3: castVote(); break;
            case 4: displayResult(); break;
            case 5: {
                saveVoters();
                saveCandidates();
                cout << "Data saved. Exiting...\n";
                return 0;
            }
            default: cout << "Invalid choice! Try again.\n";
        }
    }
}

// Load voters from file
void loadVoters() {
    ifstream file("voters.txt");
    if (!file) return;

    Voter v;
    while (file >> v.voterID >> v.name >> v.age >> v.pin >> v.hasVoted) {
        voters.push_back(v);
    }
    file.close();
}

// Save voters to file
void saveVoters() {
    ofstream file("voters.txt");
    for (auto& v : voters) {
        file << v.voterID << " " << v.name << " " << v.age << " " << v.pin << " " << v.hasVoted << "\n";
    }
    file.close();
}

// Load candidates from file
void loadCandidates() {
    ifstream file("candidates.txt");
    if (!file) return;

    Candidate c;
    while (file >> c.candidateID >> c.name >> c.votes) {
        candidates.push_back(c);
    }
    file.close();
}

// Save candidates to file
void saveCandidates() {
    ofstream file("candidates.txt");
    for (auto& c : candidates) {
        file << c.candidateID << " " << c.name << " " << c.votes << "\n";
    }
    file.close();
}

// Register a voter
void registerVoter() {
    Voter v;
    cout << "Enter Voter ID: ";
    cin >> v.voterID;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, v.name);
    cout << "Enter Age: ";
    cin >> v.age;

    if (v.age < 18) {
        cout << "Sorry, age must be 18+ to vote.\n";
        return;
    }

    cout << "Set a PIN: ";
    cin >> v.pin;

    voters.push_back(v);
    saveVoters();
    cout << "Voter registered successfully.\n";
}

// Add a candidate
void addCandidate() {
    Candidate c;
    cout << "Enter Candidate ID: ";
    cin >> c.candidateID;
    cout << "Enter Candidate Name: ";
    cin.ignore();
    getline(cin, c.name);

    candidates.push_back(c);
    saveCandidates();
    cout << "Candidate added successfully.\n";
}

// Cast a vote
void castVote() {
    int voterID;
    string pin;

    cout << "Enter your Voter ID: ";
    cin >> voterID;
    cout << "Enter your PIN: ";
    cin >> pin;

    if (!authenticateVoter(voterID, pin)) {
        cout << "Authentication failed or you have already voted.\n";
        return;
    }

    cout << "\nCandidates:\n";
    for (const auto& c : candidates) {
        cout << "ID: " << c.candidateID << " | Name: " << c.name << "\n";
    }

    int candidateID;
    cout << "Enter Candidate ID to vote: ";
    cin >> candidateID;

    Candidate* selected = findCandidate(candidateID);
    if (selected == nullptr) {
        cout << "Invalid Candidate ID.\n";
        return;
    }

    selected->votes++;
    findVoter(voterID)->hasVoted = true;

    saveVoters();
    saveCandidates();

    cout << "Vote cast successfully.\n";

    displayResult();
}

// Show result
void displayResult() {
    cout << "\n=== Current Results ===\n";

    if (candidates.empty()) {
        cout << "No candidates available.\n";
        return;
    }

    vector<Candidate> sorted = candidates;
    sort(sorted.begin(), sorted.end(), [](Candidate a, Candidate b) {
        return a.votes > b.votes;
    });

    for (const auto& c : sorted) {
        cout << "Candidate: " << c.name << " | Votes: " << c.votes << "\n";
    }

    cout << "========================\n";
}

// Authenticate voter
bool authenticateVoter(int voterID, string pin) {
    for (auto& v : voters) {
        if (v.voterID == voterID && v.pin == pin && !v.hasVoted) {
            return true;
        }
    }
    return false;
}

// Find voter by ID
Voter* findVoter(int voterID) {
    for (auto& v : voters) {
        if (v.voterID == voterID) {
            return &v;
        }
    }
    return nullptr;
}

// Find candidate by ID
Candidate* findCandidate(int candidateID) {
    for (auto& c : candidates) {
        if (c.candidateID == candidateID) {
            return &c;
        }
    }
    return nullptr;
}