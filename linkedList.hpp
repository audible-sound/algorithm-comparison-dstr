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
    double matchPercentage;
    JobDescriptionNode *next;
};

struct ResumeNode
{
    int resumeID;
    SkillNode *skillLinkedListHead;
    double matchPercentage;
    ResumeNode *next;
};

SkillNode* getMiddleSkillNodeForSort(SkillNode *head);
SkillNode* getMiddleSkillNodeForSearch(SkillNode *left, SkillNode *right);

ResumeNode* getMiddleResumeNodeForSort(ResumeNode *head);
JobDescriptionNode* getMiddleJobNodeForSort(JobDescriptionNode *head);