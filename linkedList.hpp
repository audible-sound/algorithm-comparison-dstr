#pragma once
#include <string>
using namespace std;

struct SkillNode
{
    string skillName;
    SkillNode *next;
};

struct JobDescription
{
    int jobID;
    string jobTitle;
    SkillNode *skillLinkedListHead;
    JobDescription *next;
};

struct Resume
{
    int resumeID;
    SkillNode *skillLinkedListHead;
    Resume *next;
};