#pragma once
#include <string>
#include "linkedList.hpp"
#include "array.hpp"

using namespace std;

string extractJobTitle(const string &line);
string extractSkillsString(const string &line);
bool isCapital(const char c);
string toLower(const string &s);

// Array utility functions
Job *createJobStruct(const int id,const string &line);
Candidate *createCandidateStruct(const int id,const string &line);

JobArray *fetchJobsArray();
CandidateArray *fetchCandidatesArray();

// Linked list utility functions
SkillNode *createSkillLinkedList(const string &skillsStr);

JobDescriptionNode *createJobDescription(const int index, const string &line);
JobDescriptionNode *fetchJobs();

ResumeNode *createResume(const int index, const string &line);
ResumeNode *fetchResumes();