#pragma once
#include <string>
using namespace std;

struct Job
{
    int id;
    string role;
    string *skills;
    int skillCount;
    int skillCapacity;
    Job();
    void initSkills(int cap = 8);
    void addSkill(const string &newSkill);
    Job(const Job &other);
    Job &operator=(const Job &other);
    ~Job();
};

class JobArray
{
private:
    Job *arr;
    int size;
    int capacity;
    void ensureCapacity(int minCap);

public:
    JobArray();
    ~JobArray();
    void addJob(const Job &j);
    int getSize() const;
    Job &get(int idx);
};

struct ResultJob
{
    Job job;
    double matchPercentage;
};

struct Candidate
{
    int id;
    string *skills;
    int skillCount;
    int skillCapacity;
    Candidate();
    void initSkills(int cap = 8);
    void addSkill(const string &sk);
    Candidate(const Candidate &other);
    Candidate &operator=(const Candidate &other);
    ~Candidate();
};

class CandidateArray
{
private:
    Candidate *arr;
    int size;
    int capacity;
    void ensureCapacity(int minCap);

public:
    CandidateArray();
    ~CandidateArray();
    void addCandidate(const Candidate &c);
    int getSize() const;
    Candidate &get(int idx);
};

struct ResultCandidate
{
    Candidate candidate;
    double matchPercentage;
};

