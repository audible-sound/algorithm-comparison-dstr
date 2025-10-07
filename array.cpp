#include <string>
#include "array.hpp"

using namespace std;

// --- Job Struct ---
Job::Job() : id(0), role(""), skills(nullptr), skillCount(0), skillCapacity(0) {}

void Job::initSkills(int cap) {
    skills = new string[cap];
    skillCapacity = cap;
    skillCount = 0;
}

void Job::addSkill(const string &newSkill) {
    if (!skills)
        initSkills();
    if (skillCount >= skillCapacity) {
        int newCap = skillCapacity * 2;
        string *tmp = new string[newCap];
        for (int i = 0; i < skillCount; i++)
            tmp[i] = skills[i];
        delete[] skills;
        skills = tmp;
        skillCapacity = newCap;
    }
    skills[skillCount++] = newSkill;
}

Job::Job(const Job &other) {
    id = other.id;
    role = other.role;
    skillCount = other.skillCount;
    skillCapacity = other.skillCapacity;
    if (other.skills) {
        skills = new string[skillCapacity];
        for (int i = 0; i < skillCount; i++)
            skills[i] = other.skills[i];
    } else {
        skills = nullptr;
    }
}

Job &Job::operator=(const Job &other) {
    if (this == &other)
        return *this;
    delete[] skills;
    id = other.id;
    role = other.role;
    skillCount = other.skillCount;
    skillCapacity = other.skillCapacity;
    if (other.skills) {
        skills = new string[skillCapacity];
        for (int i = 0; i < skillCount; i++)
            skills[i] = other.skills[i];
    } else {
        skills = nullptr;
    }
    return *this;
}

Job::~Job() { delete[] skills; }

// --- JobArray ---
JobArray::JobArray() : arr(nullptr), size(0), capacity(0) {}

JobArray::~JobArray() { delete[] arr; }

void JobArray::ensureCapacity(int minCap) {
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

void JobArray::addJob(const Job &j) {
    ensureCapacity(size + 1);
    arr[size++] = j;
}

int JobArray::getSize() const { return size; }

Job &JobArray::get(int idx) { return arr[idx]; }

// --- Candidate Struct ---
Candidate::Candidate() : id(0), skills(nullptr), skillCount(0), skillCapacity(0) {}

void Candidate::initSkills(int cap) {
    skills = new string[cap];
    skillCapacity = cap;
    skillCount = 0;
}

void Candidate::addSkill(const string &sk) {
    if (!skills)
        initSkills();
    if (skillCount >= skillCapacity) {
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

Candidate::Candidate(const Candidate &other) {
    id = other.id;
    skillCount = other.skillCount;
    skillCapacity = other.skillCapacity;
    if (other.skills) {
        skills = new string[skillCapacity];
        for (int i = 0; i < skillCount; i++)
            skills[i] = other.skills[i];
    } else {
        skills = nullptr;
    }
}

Candidate &Candidate::operator=(const Candidate &other) {
    if (this == &other)
        return *this;
    delete[] skills;
    id = other.id;
    skillCount = other.skillCount;
    skillCapacity = other.skillCapacity;
    if (other.skills) {
        skills = new string[skillCapacity];
        for (int i = 0; i < skillCount; i++)
            skills[i] = other.skills[i];
    } else {
        skills = nullptr;
    }
    return *this;
}

Candidate::~Candidate() { delete[] skills; }

// --- CandidateArray ---
CandidateArray::CandidateArray() : arr(nullptr), size(0), capacity(0) {}

CandidateArray::~CandidateArray() { delete[] arr; }

void CandidateArray::ensureCapacity(int minCap) {
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

void CandidateArray::addCandidate(const Candidate &c) {
    ensureCapacity(size + 1);
    arr[size++] = c;
}

int CandidateArray::getSize() const { return size; }

Candidate &CandidateArray::get(int idx) { return arr[idx]; }