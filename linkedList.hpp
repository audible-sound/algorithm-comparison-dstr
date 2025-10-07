#pragma once
#include <string>
using namespace std;

struct SkillNode
{
    string skillName;
    SkillNode *next;
};

struct JobDescriptionNode
{
    int jobID;
    string jobTitle;
    SkillNode *skillLinkedListHead;
    JobDescriptionNode *next;
};

struct ResumeNode
{
    int resumeID;
    SkillNode *skillLinkedListHead;
    ResumeNode *next;
};


SkillNode* getMiddleSkillNodeForSort(SkillNode *head);
SkillNode* getMiddleSkillNodeForSearch(SkillNode *left, SkillNode *right);