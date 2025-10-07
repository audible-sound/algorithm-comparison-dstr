// JobExtract.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <chrono>

using namespace std;

// Helper functions
// ..................
static inline void trim(string& s) {
    size_t start = 0;
    while (start < s.size() && isspace((unsigned char)s[start])) start++;
    size_t end = s.size();
    while (end > start && isspace((unsigned char)s[end - 1])) end--;
    s = s.substr(start, end - start);
}
// Case-insensitive compare
static string toLower(const string& s) {
    string out = s;
    for (char& c : out) c = tolower((unsigned char)c);
    return out;
}

// Job structure: [ID, Role, Skills...]
// ...................
struct Job {
    int jobID;
    string role;
    string* skills;
    int skillCount;
    int skillCapacity;

    Job() : jobID(0), role(""), skills(nullptr), skillCount(0), skillCapacity(0) {}

    void initSkills(int cap = 8) {
        skills = new string[cap];
        skillCapacity = cap;
        skillCount = 0;
    }

    void addSkill(const string& sk) {
        if (skills == nullptr) initSkills();
        if (skillCount >= skillCapacity) {
            int newCap = skillCapacity * 2;
            string* tmp = new string[newCap];
            for (int i = 0; i < skillCount; i++) tmp[i] = skills[i];
            delete[] skills;
            skills = tmp;
            skillCapacity = newCap;
        }
        skills[skillCount++] = sk;
    }

    //Copy constructor
    Job(const Job& other) {
        jobID = other.jobID;
        role = other.role;
        skillCount = other.skillCount;
        skillCapacity = other.skillCapacity;
        if (other.skills) {
            skills = new string[skillCapacity];
            for (int i = 0; i < skillCount; i++) {
                skills[i] = other.skills[i];
            }
        }
        else {
            skills = nullptr;
        }
    }

    //Assignment operator 
    Job& operator=(const Job& other) {
        if (this == &other) return *this; // self-assign check
        delete[] skills;
        jobID = other.jobID;
        role = other.role;
        skillCount = other.skillCount;
        skillCapacity = other.skillCapacity;
        if (other.skills) {
            skills = new string[skillCapacity];
            for (int i = 0; i < skillCount; i++) {
                skills[i] = other.skills[i];
            }
        }
        else {
            skills = nullptr;
        }
        return *this;
    }

    //Destructor
    ~Job() {
        delete[] skills;
    }
};

// Candidate structure: [ID, Skills...]
// ...................
struct Candidate {
    int candidateID;
    string* skills;
    int skillCount;
    int skillCapacity;

    Candidate() : candidateID(0), skills(nullptr), skillCount(0), skillCapacity(0) {}

    void initSkills(int cap = 8) {
        skills = new string[cap];
        skillCapacity = cap;
        skillCount = 0;
    }

    void addSkill(const string& sk) {
        if (!skills) initSkills();
        if (skillCount >= skillCapacity) {
            int newCap = skillCapacity * 2;
            string* tmp = new string[newCap];
            for (int i = 0; i < skillCount; i++) tmp[i] = skills[i];
            delete[] skills;
            skills = tmp;
            skillCapacity = newCap;
        }
        skills[skillCount++] = sk;
    }

    // Copy constructor
    Candidate(const Candidate& other) {
        candidateID = other.candidateID;
        skillCount = other.skillCount;
        skillCapacity = other.skillCapacity;
        if (other.skills) {
            skills = new string[skillCapacity];
            for (int i = 0; i < skillCount; i++) skills[i] = other.skills[i];
        }
        else skills = nullptr;
    }

    // Assignment operator
    Candidate& operator=(const Candidate& other) {
        if (this == &other) return *this;
        delete[] skills;
        candidateID = other.candidateID;
        skillCount = other.skillCount;
        skillCapacity = other.skillCapacity;
        if (other.skills) {
            skills = new string[skillCapacity];
            for (int i = 0; i < skillCount; i++) skills[i] = other.skills[i];
        }
        else skills = nullptr;
        return *this;
    }

    ~Candidate() { delete[] skills; }
};


// Job Array container
// ......................
class JobArray {
private:
    Job* arr;
    int size;
    int capacity;

    void ensureCapacity(int minCap) {
        if (capacity >= minCap) return;
        int newCap = capacity == 0 ? 8 : capacity * 2;
        while (newCap < minCap) newCap *= 2;
        Job* tmp = new Job[newCap];
        for (int i = 0; i < size; i++) tmp[i] = arr[i];
        delete[] arr;
        arr = tmp;
        capacity = newCap;
    }

public:
    JobArray() {
        arr = nullptr;
        size = 0;
        capacity = 0;
    }

    ~JobArray() {
        delete[] arr;
    }

    void addJob(const Job& j) {
        ensureCapacity(size + 1);
        arr[size++] = j;
    }

    int getSize() const { return size; }
    Job& get(int idx) { return arr[idx]; }

    // Parse CSV line -> Job object
    static Job parseLine(const string& line) {
        Job job;
        string desc = line;

        // Role = before "needed"
        size_t posNeeded = desc.find("needed");
        if (posNeeded != string::npos) {
            job.role = desc.substr(0, posNeeded);
        }
        else {
            job.role = "Unknown Role";
        }
        trim(job.role);

        // remove leading and trailing quotes if any
        if (!job.role.empty() && job.role.front() == '"')
            job.role.erase(0, 1);
        if (!job.role.empty() && job.role.back() == '"')
            job.role.pop_back();

        // Skills = after "experience in "
        size_t posExp = desc.find("experience in ");
        string skillsPart;
        if (posExp != string::npos) {
            skillsPart = desc.substr(posExp + 13); // skip "experience in "
        }
        // Cut at "."
        size_t dot = skillsPart.find(".");
        if (dot != string::npos) {
            skillsPart = skillsPart.substr(0, dot);
        }

        // Split by commas into skills[]
        string token;
        stringstream ss(skillsPart);
        job.initSkills();
        while (getline(ss, token, ',')) {
            trim(token);
            if (!token.empty()) job.addSkill(token);
        }

        return job;
    }

    // Load CSV 
    // .........
    bool loadCSV(const string& filename) {
        ifstream fin(filename.c_str());
        if (!fin.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return false;
        }
        string line;
        bool firstLine = true;
        int rowCounter = 0;

        while (getline(fin, line)) {
            trim(line);
            if (firstLine) { // skip header
                firstLine = false;
                continue;
            }
            if (line.empty()) continue;
            rowCounter++; //increment per valid line
            Job j = parseLine(line);
            j.jobID = rowCounter; // assign job ID here
            addJob(j);
        }
        fin.close();
        cout << "Total jobs loaded: " << rowCounter << endl; //optional immediate feedback
        return true;
    }

};

// Candidate Array container
// ..........................
class CandidateArray {
private:
    Candidate* arr;
    int size;
    int capacity;

    void ensureCapacity(int minCap) {
        if (capacity >= minCap) return;
        int newCap = capacity == 0 ? 8 : capacity * 2;
        while (newCap < minCap) newCap *= 2;
        Candidate* tmp = new Candidate[newCap];
        for (int i = 0; i < size; i++) tmp[i] = arr[i];
        delete[] arr;
        arr = tmp;
        capacity = newCap;
    }

public:
    CandidateArray() : arr(nullptr), size(0), capacity(0) {}
    ~CandidateArray() { delete[] arr; }

    void addCandidate(const Candidate& c) { ensureCapacity(size + 1); arr[size++] = c; }
    int getSize() const { return size; }
    Candidate& get(int idx) { return arr[idx]; }

    static Candidate parseLine(const string& line) {
        Candidate cand;
        string desc = line;
        size_t posSkills = desc.find("skilled in ");
        string skillsPart = (posSkills != string::npos) ? desc.substr(posSkills + 10) : "";
        size_t dot = skillsPart.find(".");
        if (dot != string::npos) skillsPart = skillsPart.substr(0, dot);

        string token;
        stringstream ss(skillsPart);
        cand.initSkills();
        while (getline(ss, token, ',')) {
            trim(token);
            if (!token.empty()) cand.addSkill(token);
        }
        return cand;
    }

    bool loadCSV(const string& filename) {
        ifstream fin(filename);
        if (!fin.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return false;
        }
        string line;
        bool firstLine = true;
        int rowCounter = 0;
        while (getline(fin, line)) {
            trim(line);
            if (firstLine) { firstLine = false; continue; }
            if (line.empty()) continue;
            rowCounter++;
            Candidate c = parseLine(line);
            c.candidateID = rowCounter;
            addCandidate(c);
        }
        fin.close();
        cout << "Total candidates loaded: " << rowCounter << endl;
        return true;
    }
};



// Linear Search for skills
// ..........................

// linear search for a skill inside one job
int linearSearchSkill(string* skills, int n, const string& target) {
    string tgt = toLower(target); // helper: case insensitive
    for (int i = 0; i < n; i++) {
        if (toLower(skills[i]) == tgt) {
            return i; // found at index i
        }
    }
    return -1; // not found
}


// Quick Sort (jobs by score, descending)
// .......................................

struct Result {
    Job* job; // pointer when result refers to a Job (may be nullptr for candidate results)
    int score;
    int jobID; // ID to use for tie-breaker (jobID or candidateID)
};

bool shouldComeBefore(const Result& a, const Result& b) {
    if (a.score != b.score)
        return a.score > b.score; // higher score first
    return a.jobID < b.jobID;
    //HERE : return a.job->jobID < b.job->jobID; //tie-break: smaller jobID first
}

int partition(Result arr[], int low, int high) {
    Result pivot = arr[high]; //choose last element as pivot

    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (shouldComeBefore(arr[j], pivot)) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(Result arr[], int low, int high) {
    while (low < high) { // tail recursion optimization
        int pi = partition(arr, low, high);

        // Recur on smaller side first to prevent stack overflow
        if (pi - low < high - pi) {
            quickSort(arr, low, pi - 1);
            low = pi + 1;
        }
        else {
            quickSort(arr, pi + 1, high);
            high = pi - 1;
        }
    }
}

// ------------
// Main
// ------------
int main() {
    cout << "Are you a Job Seeker or Employer? (Enter 'seeker' or 'employer'): ";
    string mode;
    getline(cin, mode);

    if (toLower(mode) == "seeker") {
        // load jobs
        JobArray jobs;
        if (!jobs.loadCSV("job_description.csv")) {
            cout << "Failed to load job_description.csv\n";
            return 1;
        }
        cout << "Enter skills (comma separated): ";
        string userInput;
        getline(cin, userInput);

        // Parse user input skills
        stringstream ss(userInput);
        string token;
        string* userSkills = new string[50]; // support up to 50 user skills
        int userSkillCount = 0;
        while (getline(ss, token, ',')) {
            trim(token);
            if (!token.empty()) userSkills[userSkillCount++] = token;
        }
        // performance timer
        auto start = chrono::high_resolution_clock::now();

        // Compute scores
        int n = jobs.getSize();
        Result* results = new Result[n];
        for (int i = 0; i < n; i++) {
            Job& j = jobs.get(i);
            int matchCount = 0;
            for (int u = 0; u < userSkillCount; u++) {
                if (linearSearchSkill(j.skills, j.skillCount, userSkills[u]) != -1) {
                    matchCount++;
                }
            }
            // percentage score instead of raw points
            int percent = (j.skillCount > 0) ? (matchCount * 100) / j.skillCount : 0;
            results[i] = { &j, percent, j.jobID };
            // HERE : results[i] = { &j, percent };
        }

        // Quick Sort
        if (n > 0) {
            quickSort(results, 0, n - 1);
        }

        // Display matching threshold
        int threshold = 50;
        cout << "\n===== Matching Jobs (Score > " << threshold << "%) =====\n";
        int displayedCount = 0;

        bool found = false;
        for (int i = 0; i < n; i++) {
            if (results[i].score <= threshold) break;
            found = true;
            cout << "Compatibility: " << results[i].score
                << "% | Job ID: " << results[i].jobID //HERE 
                << " | " << results[i].job->role
                << "\n  Skills: ";
            displayedCount++;
            for (int k = 0; k < results[i].job->skillCount; k++) {
                cout << results[i].job->skills[k];
                if (k < results[i].job->skillCount - 1) cout << ", ";
            }
            cout << "\n\n";
        }
        if (!found) cout << "No jobs matched the threshold.\n";
        // end performance timer
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = end - start;

        cout << "\n===== Performance Evaluation =====\n";
        cout << "\nTotal Jobs Displayed: " << displayedCount << endl;
        cout << "Execution Time: " << duration.count() << " ms\n";

        // Memory estimate
        size_t totalMemory = 0;
        for (int i = 0; i < jobs.getSize(); i++) {
            Job& j = jobs.get(i);
            totalMemory += sizeof(Job);
            totalMemory += j.role.capacity();
            for (int k = 0; k < j.skillCount; k++) {
                totalMemory += j.skills[k].capacity();
            }
        }
        cout << "Estimated Memory Usage: " << totalMemory / 1024.0 << " KB\n";
        delete[] userSkills;
        delete[] results;
    }
    else if (toLower(mode) == "employer") {
        CandidateArray candidates;
        if (!candidates.loadCSV("resume.csv")) return 1;

        cout << "Enter required skills (comma separated): ";
        string userInput;
        getline(cin, userInput);

        stringstream ss(userInput);
        string token;
        string* jobSkills = new string[50];
        int jobSkillCount = 0;
        while (getline(ss, token, ',')) {
            trim(token);
            if (!token.empty()) jobSkills[jobSkillCount++] = token;
        }
        //start performance timer
        auto start = chrono::high_resolution_clock::now();

        int n = candidates.getSize();
        Result* results = new Result[n];
        for (int i = 0; i < n; i++) {
            Candidate& c = candidates.get(i);
            int matchCount = 0;
            for (int j = 0; j < jobSkillCount; j++) {
                if (linearSearchSkill(c.skills, c.skillCount, jobSkills[j]) != -1) matchCount++;
            }
            int percent = (jobSkillCount > 0) ? (matchCount * 100) / jobSkillCount : 0;
            // reuse Result struct, but store candidate ID for display
            results[i] = { nullptr, percent, c.candidateID };
        }

        quickSort(results, 0, n - 1);
        // display
        cout << "\n===== Matching Candidates (Score > 50%) =====\n";
        int displayedCount = 0;
        for (int i = 0; i < n; i++) {
            if (results[i].score <= 50) break;
            displayedCount++;
            cout << "Compatibility: " << results[i].score
                << "% | Candidate ID: " << results[i].jobID << "\n  Skills: ";

            Candidate& c = candidates.get(results[i].jobID - 1); // since ID starts from 1
            for (int k = 0; k < c.skillCount; ++k) {
                cout << c.skills[k];
                if (k < c.skillCount - 1) cout << ", ";
            }
            cout << "\n\n";
        }
        // end performance timer
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = end - start;

        // Memory estimate
        size_t totalMemory = 0;
        for (int i = 0; i < candidates.getSize(); ++i) {
            Candidate& c = candidates.get(i);
            totalMemory += sizeof(Candidate);
            if (c.skills != nullptr && c.skillCount > 0) {
                for (int k = 0; k < c.skillCount; ++k) {
                    totalMemory += c.skills[k].capacity();
                }
            }
        }
        cout << "\nTotal Candidates Displayed: " << displayedCount << endl;
        cout << "Execution Time: " << duration.count() << " ms\n";
        cout << "Estimated Memory Usage: " << totalMemory / 1024.0 << " KB\n";

        delete[] jobSkills;
        delete[] results;
    }


    return 0;
}