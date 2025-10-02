#pragma once
#include <string>
#include "linkedList.hpp"

using namespace std;

string extractJobTitle(const string &line);
string extractSkillsString(const string &line);
SkillNode *createSkillLinkedList(const string &skillsStr);

bool isCapital(const char c);

JobDescription *createJobDescription(const int index, const string &line);
JobDescription *fetchJobs();

Resume *createResume(const int index, const string &line);
Resume *fetchResumes();