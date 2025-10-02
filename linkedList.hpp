#pragma once
#include <string>
using namespace std;

struct SkillNode
{
    string skillName;
    SkillNode *next;
};

class JobDescription
{
public:
    int jobID;
    string jobTitle;
    SkillNode *skillLinkedListHead;
};

class Resume
{
public:
    int resumeID;
    SkillNode *skillLinkedListHead;
};

struct JobNode
{
    JobDescription *jobDesc;
    JobNode *next;
};

struct ResumeNode
{
    Resume *resume;
    ResumeNode *next;
};