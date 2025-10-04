// JobExtract.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <chrono>

using namespace std;

// Helper functions
static inline void trim(string &s)
{
    size_t start = 0;
    while (start < s.size() && isspace((unsigned char)s[start]))
        start++;
    size_t end = s.size();
    while (end > start && isspace((unsigned char)s[end - 1]))
        end--;
    s = s.substr(start, end - start);
}

static string toLower(const string &s)
{
    string out = s;
    for (char &c : out)
        c = tolower((unsigned char)c);
    return out;
}

bool equalsIgnoreCase(const string &a, const string &b)
{
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); i++)
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i]))
            return false;
    return true;
}

// Job structure
struct Job
{
    int id;
    string role;
    string *skills;
    int skillCount;
    int skillCapacity;

    Job() : id(0), role(""), skills(nullptr), skillCount(0), skillCapacity(0) {}

    void initSkills(int cap = 8)
    {
        skills = new string[cap];
        skillCapacity = cap;
        skillCount = 0;
    }

    void addSkill(const string &sk)
    {
        if (!skills)
            initSkills();
        if (skillCount >= skillCapacity)
        {
            int newCap = skillCapacity * 2;
            string *tmp = new string[newCap];
            for (int i = 0; i < skillCount; i++)
                tmp[i] = skills[i];
            delete[] skills;
            skills = tmp;
            skillCapacity = newCap;
        }
        skills[skillCount++] = sk;
    }

    // Deep copy constructor
    Job(const Job &other)
    {
        id = other.id; // <-- add this
        role = other.role;
        skillCount = other.skillCount;
        skillCapacity = other.skillCapacity;
        if (other.skills)
        {
            skills = new string[skillCapacity];
            for (int i = 0; i < skillCount; i++)
                skills[i] = other.skills[i];
        }
        else
        {
            skills = nullptr;
        }
    }

    // Deep copy assignment
    Job &operator=(const Job &other)
    {
        if (this == &other)
            return *this;
        delete[] skills;
        id = other.id; // <-- add this
        role = other.role;
        skillCount = other.skillCount;
        skillCapacity = other.skillCapacity;
        if (other.skills)
        {
            skills = new string[skillCapacity];
            for (int i = 0; i < skillCount; i++)
                skills[i] = other.skills[i];
        }
        else
        {
            skills = nullptr;
        }
        return *this;
    }

    ~Job() { delete[] skills; }
};

// Candidate structure
struct Candidate
{
    int id;
    string role;
    string *skills;
    int skillCount;
    int skillCapacity;

    Candidate() : id(0), role("Candidate"), skills(nullptr), skillCount(0), skillCapacity(0) {}

    void initSkills(int cap = 8)
    {
        skills = new string[cap];
        skillCapacity = cap;
        skillCount = 0;
    }

    void addSkill(const string &sk)
    {
        if (!skills)
            initSkills();
        if (skillCount >= skillCapacity)
        {
            int newCap = skillCapacity * 2;
            string *tmp = new string[newCap];
            for (int i = 0; i < skillCount; i++)
                tmp[i] = skills[i];
            delete[] skills;
            skills = tmp;
            skillCapacity = newCap;
        }
        skills[skillCount++] = sk;
    }

    Candidate(const Candidate &other)
    {
        id = other.id; // <-- add this
        role = other.role;
        skillCount = other.skillCount;
        skillCapacity = other.skillCapacity;
        if (other.skills)
        {
            skills = new string[skillCapacity];
            for (int i = 0; i < skillCount; i++)
                skills[i] = other.skills[i];
        }
        else
        {
            skills = nullptr;
        }
    }

    Candidate &operator=(const Candidate &other)
    {
        if (this == &other)
            return *this;
        delete[] skills;
        id = other.id; // <-- add this
        role = other.role;
        skillCount = other.skillCount;
        skillCapacity = other.skillCapacity;
        if (other.skills)
        {
            skills = new string[skillCapacity];
            for (int i = 0; i < skillCount; i++)
                skills[i] = other.skills[i];
        }
        else
        {
            skills = nullptr;
        }
        return *this;
    }

    ~Candidate() { delete[] skills; }
};

// JobArray container
class JobArray
{
private:
    Job *arr;
    int size;
    int capacity;

    void ensureCapacity(int minCap)
    {
        if (capacity >= minCap)
            return;
        int newCap = capacity == 0 ? 8 : capacity * 2;
        while (newCap < minCap)
            newCap *= 2;
        Job *tmp = new Job[newCap];
        for (int i = 0; i < size; i++)
            tmp[i] = arr[i];
        delete[] arr;
        arr = tmp;
        capacity = newCap;
    }

public:
    JobArray() : arr(nullptr), size(0), capacity(0) {}
    ~JobArray() { delete[] arr; }

    void addJob(const Job &j)
    {
        ensureCapacity(size + 1);
        arr[size++] = j;
    }
    int getSize() const { return size; }
    Job &get(int idx) { return arr[idx]; }

    // Parse CSV line -> Job
    static Job parseLine(const string &line)
    {
        Job job;
        string desc = line;

        // Extract role
        size_t posNeeded = desc.find("needed");
        job.role = (posNeeded != string::npos) ? desc.substr(0, posNeeded) : "Unknown Role";

        // Clean up role (remove spaces and quotes)
        trim(job.role);
        if (!job.role.empty() && job.role.front() == '"')
            job.role.erase(0, 1);
        if (!job.role.empty() && job.role.back() == '"')
            job.role.pop_back();

        // Extract skills
        size_t posExp = desc.find("experience in ");
        string skillsPart;
        if (posExp != string::npos)
            skillsPart = desc.substr(posExp + 13);
        size_t dot = skillsPart.find(".");
        if (dot != string::npos)
            skillsPart = skillsPart.substr(0, dot);

        // Split skills
        string token;
        stringstream ss(skillsPart);
        job.initSkills();
        while (getline(ss, token, ','))
        {
            trim(token);
            if (!token.empty())
                job.addSkill(token);
        }

        return job;
    }

    // Load CSV
    bool loadCSV(const string &filename)
    {
        ifstream fin(filename);
        if (!fin.is_open())
        {
            cerr << "Error: Could not open file " << filename << endl;
            return false;
        }
        string line;
        bool firstLine = true;
        int idCounter = 1;
        while (getline(fin, line))
        {
            trim(line);
            if (firstLine)
            {
                firstLine = false;
                continue;
            }
            if (line.empty())
                continue;
            Job job = parseLine(line);
            job.id = idCounter++;
            addJob(job);
        }
        fin.close();
        return true;
    }
};

// CandidateArray container
class CandidateArray
{
private:
    Candidate *arr;
    int size;
    int capacity;

    void ensureCapacity(int minCap)
    {
        if (capacity >= minCap)
            return;
        int newCap = capacity == 0 ? 8 : capacity * 2;
        while (newCap < minCap)
            newCap *= 2;
        Candidate *tmp = new Candidate[newCap];
        for (int i = 0; i < size; i++)
            tmp[i] = arr[i];
        delete[] arr;
        arr = tmp;
        capacity = newCap;
    }

    static bool isValidSkill(const string &s)
    {
        if (s.empty())
            return false;
        for (char c : s)
            if (isupper((unsigned char)c))
                return true;
        return false;
    }

public:
    CandidateArray() : arr(nullptr), size(0), capacity(0) {}
    ~CandidateArray() { delete[] arr; }

    void addCandidate(const Candidate &c)
    {
        ensureCapacity(size + 1);
        arr[size++] = c;
    }
    int getSize() const { return size; }
    Candidate &get(int idx) { return arr[idx]; }

    static Candidate parseLine(const string &line)
    {
        Candidate cand;
        string desc = line;

        size_t posSkills = desc.find("skilled in ");
        string skillsPart = (posSkills != string::npos) ? desc.substr(posSkills + 10) : "";
        size_t dot = skillsPart.find(".");
        if (dot != string::npos)
            skillsPart = skillsPart.substr(0, dot);

        string token;
        stringstream ss(skillsPart);
        cand.initSkills();
        while (getline(ss, token, ','))
        {
            trim(token);
            if (!token.empty() && isValidSkill(token))
                cand.addSkill(token);
        }
        return cand;
    }

    // Load CSV
    bool loadCSV(const string &filename)
    {
        ifstream fin(filename);
        if (!fin.is_open())
        {
            cerr << "Error: Could not open file " << filename << endl;
            return false;
        }
        string line;
        bool firstLine = true;
        int idCounter = 1;
        while (getline(fin, line))
        {
            trim(line);
            if (firstLine)
            {
                firstLine = false;
                continue;
            }
            if (line.empty())
                continue;
            Candidate c = parseLine(line);
            c.id = idCounter++;
            addCandidate(c);
        }
        fin.close();
        return true;
    }
};

// Binary search for skills
int binarySearchSkill(string *skills, int n, const string &target)
{
    int low = 0, high = n - 1;
    string tgt = toLower(target);
    while (low <= high)
    {
        int mid = (low + high) / 2;
        string midVal = toLower(skills[mid]);
        if (midVal == tgt)
            return mid;
        else if (midVal < tgt)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

// Merge Sort for Results
struct Result
{
    Job *job;
    int score;
};

void merge(Result arr[], int l, int m, int r)
{
    int n1 = m - l + 1, n2 = r - m;
    Result *L = new Result[n1];
    Result *R = new Result[n2];
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i].score >= R[j].score) ? L[i++] : R[j++];
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
    delete[] L;
    delete[] R;
}

void mergeSort(Result arr[], int l, int r)
{
    if (l < r)
    {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Main Function
int main()
{
    while (true)
    {
        // Ask user type
        cout << "\n----------- Select Your Role ------------\n";
        cout << " 1. Job Seeker\n";
        cout << " 2. Recruiter / Employer\n";
        cout << " 3. Exit\n";
        cout << "-----------------------------------------\n";
        cout << "Enter your choice (1-3): ";

        string userType;
        getline(cin, userType);

        if (userType == "1")
        {
            // Load jobs
            JobArray jobs;
            if (!jobs.loadCSV("./data/job_description.csv"))
                return 1;

            cout << "\n-------------- Job Seeker ---------------\n";
            cout << "Enter your skills (separated by commas): ";
            string userInput;
            getline(cin, userInput);

            // Start Parse timer
            auto startParseTimer = chrono::high_resolution_clock::now();

            // Parse user skills (with duplicate removal)
            stringstream ss(userInput);
            string token;
            string *userSkills = new string[50];
            int userSkillCount = 0;

            while (getline(ss, token, ','))
            {
                trim(token);
                if (!token.empty())
                {
                    // Check for duplicates before adding
                    bool exists = false;
                    for (int i = 0; i < userSkillCount; i++)
                    {
                        // Case-insensitive check
                        if (equalsIgnoreCase(userSkills[i], token))
                        {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists)
                        userSkills[userSkillCount++] = token;
                }
            }

            // End Parse timer
            auto endParseTimer = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> parseDuration = endParseTimer - startParseTimer;
            // Start Search and Matching timer
            auto startSearchTimer = chrono::high_resolution_clock::now();

            // Compute matching scores
            int n = jobs.getSize();
            Result *results = new Result[n];
            for (int i = 0; i < n; i++)
            {
                Job &j = jobs.get(i);
                int matchCount = 0;
                for (int u = 0; u < userSkillCount; u++)
                    if (binarySearchSkill(j.skills, j.skillCount, userSkills[u]) != -1)
                        matchCount++;
                results[i] = {&j, (j.skillCount > 0 ? (matchCount * 100) / j.skillCount : 0)};
            }

            // End Search timer
            auto endSearchTimer = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> searchDuration = endSearchTimer - startSearchTimer;
            // Start Sort timer
            auto startSortTimer = chrono::high_resolution_clock::now();

            mergeSort(results, 0, n - 1);

            // End Sort timer
            auto endSortTimer = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> sortDuration = endSortTimer - startSortTimer;
            // Start Display timer
            auto startDisplayTimer = chrono::high_resolution_clock::now();

            // Display matching jobs
            cout << "\n------------- Matching Jobs -------------\n";
            int count = 0;
            for (int i = 0; i < n; i++)
            {
                if (results[i].score < 50)
                    break;

                count++;

                cout << "-----------------------------------------\n";
                cout << " Job ID         : " << results[i].job->id << "\n";
                cout << " Job Title      : " << results[i].job->role << "\n";
                cout << " Compatibility  : " << results[i].score << "%\n";
                cout << " Required Skills: ";

                for (int k = 0; k < results[i].job->skillCount; k++)
                {
                    cout << results[i].job->skills[k];
                    if (k < results[i].job->skillCount - 1)
                        cout << ", ";
                }
                cout << "\n-----------------------------------------\n\n";
            }

            if (count > 1)
            {
                cout << count << " Jobs Loaded";
            }

            else if (count == 1)
            {
                cout << "1 Job Loaded";
            }

            else if (count == 0)
            {
                cout << "No Job Matches";
            }

            // End timer
            auto endDisplayTimer = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> displayDuration = endDisplayTimer - startDisplayTimer;

            // Memory estimate
            size_t totalMemory = 0;
            for (int i = 0; i < jobs.getSize(); i++)
            {
                Job &j = jobs.get(i);
                totalMemory += sizeof(Job);
                totalMemory += j.role.capacity();
                for (int k = 0; k < j.skillCount; k++)
                    totalMemory += j.skills[k].capacity();
            }

            // Display performance
            cout << "\n\n-------- Performance Evaluation ---------\n";
            cout << "Parse Time: " << parseDuration.count() << " ms\n";
            cout << "Search and Matching Time: " << searchDuration.count() << " ms\n";
            cout << "Sort Time: " << sortDuration.count() << " ms\n";
            cout << "Display Time: " << displayDuration.count() << " ms\n";
            cout << "Total Execution Time: " << parseDuration.count() + searchDuration.count() + sortDuration.count() + displayDuration.count() << " ms\n";
            cout << "Estimated Memory Usage: " << totalMemory / 1024.0 << " KB\n";

            delete[] userSkills;
            delete[] results;
        }

        else if (userType == "2")
        {
            // Load candidates
            CandidateArray candidates;
            if (!candidates.loadCSV("./data/resume.csv"))
                return 1;

            cout << "\n-------------- Recruiter ----------------\n";
            cout << "Enter required skills (separated by commas): ";
            string userInput;
            getline(cin, userInput);

            // Start Parse timer
            auto startParseTimer = chrono::high_resolution_clock::now();

            // Parse required skills with duplicate removal
            stringstream ss(userInput);
            string token;
            string *jobSkills = new string[50];
            int jobSkillCount = 0;

            while (getline(ss, token, ','))
            {
                trim(token);
                if (!token.empty())
                {
                    // Check for duplicates before adding
                    bool exists = false;
                    for (int i = 0; i < jobSkillCount; i++)
                    {
                        if (equalsIgnoreCase(jobSkills[i], token))
                        {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists)
                        jobSkills[jobSkillCount++] = token;
                }
            }

            // End Parse timer
            auto endParseTimer = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> parseDuration = endParseTimer - startParseTimer;
            // Start Search and Matching timer
            auto startSearchTimer = chrono::high_resolution_clock::now();

            // Compute matching scores
            int n = candidates.getSize();
            Result *results = new Result[n];
            for (int i = 0; i < n; i++)
            {
                Candidate &c = candidates.get(i);
                int matchCount = 0;
                for (int j = 0; j < jobSkillCount; j++)
                    if (binarySearchSkill(c.skills, c.skillCount, jobSkills[j]) != -1)
                        matchCount++;
                results[i] = {(Job *)&c, (jobSkillCount > 0 ? (matchCount * 100) / jobSkillCount : 0)};
            }

            // End Search timer
            auto endSearchTimer = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> searchDuration = endSearchTimer - startSearchTimer;
            // Start Sort timer
            auto startSortTimer = chrono::high_resolution_clock::now();

            mergeSort(results, 0, n - 1);

            // End Sort timer
            auto endSortTimer = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> sortDuration = endSortTimer - startSortTimer;
            // Start Display timer
            auto startDisplayTimer = chrono::high_resolution_clock::now();

            // Display matching candidates
            cout << "\n--------- Matching Candidates -----------\n";
            int count = 0;
            for (int i = 0; i < n; i++)
            {
                if (results[i].score < 50)
                    break;
                Candidate *c = (Candidate *)results[i].job;

                count++;
                cout << "-----------------------------------------\n";
                cout << " Candidate ID   : " << c->id << "\n";
                cout << " Compatibility  : " << results[i].score << "%\n";
                cout << " Skills         : ";

                for (int k = 0; k < c->skillCount; k++)
                {
                    cout << c->skills[k];
                    if (k < c->skillCount - 1)
                        cout << ", ";
                }
                cout << "\n-----------------------------------------\n\n";
            }

            if (count > 1)
            {
                cout << count << " Candidates Loaded";
            }

            else if (count == 1)
            {
                cout << "1 Candidate Loaded";
            }

            else if (count == 0)
            {
                cout << "No Candidate Matches";
            }

            // End timer
            auto endDisplayTimer = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> displayDuration = endDisplayTimer - startDisplayTimer;

            // Memory estimate
            size_t totalMemory = 0;
            for (int i = 0; i < candidates.getSize(); i++)
            {
                Candidate &c = candidates.get(i);
                totalMemory += sizeof(Candidate);
                totalMemory += c.role.capacity();
                for (int k = 0; k < c.skillCount; k++)
                    totalMemory += c.skills[k].capacity();
            }

            // Display performance
            cout << "\n\n-------- Performance Evaluation ---------\n";
            cout << "Parse Time: " << parseDuration.count() << " ms\n";
            cout << "Search and Matching Time: " << searchDuration.count() << " ms\n";
            cout << "Sort Time: " << sortDuration.count() << " ms\n";
            cout << "Display Time: " << displayDuration.count() << " ms\n";
            cout << "Total Execution Time: " << parseDuration.count() + searchDuration.count() + sortDuration.count() + displayDuration.count() << " ms\n";
            cout << "Estimated Memory Usage: " << totalMemory / 1024.0 << " KB\n";

            delete[] jobSkills;
            delete[] results;
        }

        else if (userType == "3")
        {
            cout << "\nExiting program...\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }
    }
    return 0;
}
