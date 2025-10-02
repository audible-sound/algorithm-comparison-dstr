#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "fileUtils.hpp"
#include "sorting.hpp"
const string RESUME_PATH = "./data/resume.csv";
const string JOB_DESCRIPTION_PATH = "./data/job_description.csv";

string extractJobTitle(const string &line)
{
    int i = 0;
    // Stop at the first occurrence of "n"
    while (i < line.length())
    {
        if (line[i] == ' ' && line[i + 1] == 'n')
        {
            break;
        }
        i++;
    }
    string jobTitle = line.substr(0, i);
    return jobTitle;
}

string extractSkillsString(const string &line)
{
    int start = 0;
    int end = 0;

    while (end < line.length() && line[end] != '.')
    {
        if (line[end] == ' ' && line[end + 1] == 'i' && line[end + 2] == 'n' && line[end + 3] == ' ')
        {
            end = end + 4; // Move past " in "
            start = end;
        }
        end++;
    }
    int length = end - start;
    return line.substr(start, length);
}

bool isCapital(char c)
{
    return c >= 'A' && c <= 'Z';
}

SkillNode *createSkillLinkedList(const string &skillsStr)
{
    SkillNode *head = nullptr;
    SkillNode *current = nullptr;
    stringstream ss(skillsStr);
    string skill;

    // Tokenise the skills string by commas
    while (getline(ss, skill, ','))
    {
        // Trim whitespaces
        skill = skill.erase(0, skill.find_first_not_of(" "));
        skill = skill.erase(skill.find_last_not_of(" ") + 1);

        // Skip if the skill is not capitalized
        if (skill.empty() || !isCapital(skill[0]))
        {
            continue;
        }

        SkillNode *newNode = new SkillNode();
        newNode->skillName = skill;
        if (!head)
        {
            head = newNode;
            current = head;
        }
        else
        {
            current->next = newNode;
            current = current->next;
        }
    }

    return head;
}

JobDescription *createJobDescription(const int index, const string &line)
{
    JobDescription *jobDesc = new JobDescription();
    jobDesc->jobID = index;
    jobDesc->jobTitle = extractJobTitle(line);
    string skillsStr = extractSkillsString(line);
    SkillNode *skillLinkedListHead = createSkillLinkedList(skillsStr);
    jobDesc->skillLinkedListHead = skillLinkedListHead;
    return jobDesc;
}

JobNode *fetchJobs()
{
    ifstream file(JOB_DESCRIPTION_PATH);
    string line;
    int index = 0;
    JobNode *head = nullptr;
    JobNode *current = nullptr;

    if (!file.is_open())
    {
        cerr << "Error opening file: " << JOB_DESCRIPTION_PATH << endl;
        return nullptr;
    }

    // skip header line
    getline(file, line);

    while (getline(file, line))
    {
        JobDescription *newJobDesc = createJobDescription(index, line);
        JobNode *newJobNode = new JobNode();
        newJobNode->jobDesc = newJobDesc;
        newJobNode->next = nullptr;

        if (!head)
        {
            head = newJobNode;
            current = head;
        }
        else
        {
            current->next = newJobNode;
            current = current->next;
        }
        index++;
    }

    file.close();
    return head;
}

Resume *createResume(const int index, const string &line)
{
    Resume *resume = new Resume();
    resume->resumeID = index;
    string skillsStr = extractSkillsString(line);
    resume->skillLinkedListHead = createSkillLinkedList(skillsStr);
    return resume;
}

ResumeNode *fetchResumes()
{
    ifstream file(RESUME_PATH);
    string line;
    int index = 0;
    ResumeNode *head = nullptr;
    ResumeNode *current = nullptr;

    if (!file.is_open())
    {
        cerr << "Error opening file: " << RESUME_PATH << endl;
        return nullptr;
    }

    // skip header line
    getline(file, line);

    while (getline(file, line))
    {
        Resume *newResume = createResume(index, line);
        ResumeNode *newResumeNode = new ResumeNode();
        newResumeNode->resume = newResume;
        newResumeNode->next = nullptr;

        if (!head)
        {
            head = newResumeNode;
            current = head;
        }
        else
        {
            current->next = newResumeNode;
            current = current->next;
        }
        index++;
    }

    file.close();
    return head;
}
