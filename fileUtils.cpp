#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "fileUtils.hpp"
#include "linkedList.hpp"
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
    string jobTitle = line.substr(1, i);
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

string toLower(const string &s)
{
    string out = s;
    for (char &c : out)
        c = tolower((unsigned char)c);
    return out;
}

// Array utility functions
Job *createJobStruct(const int id,const string &line)
{
    Job *job = new Job();
    job->initSkills();

    job->id = id;

    // Extract job title
    job->role = extractJobTitle(line);

    // Extract skills string
    string skillsStr = extractSkillsString(line);

    // Tokenise skills and add to job struct skills array
    stringstream ss(skillsStr);
    string skill;
    while (getline(ss, skill, ','))
    {
        // Trim whitespace
        skill = skill.erase(0, skill.find_first_not_of(" "));
        skill = skill.erase(skill.find_last_not_of(" ") + 1);

        // Skip if the skill is not capitalized
        if (skill.empty() || !isCapital(skill[0]))
        {
            continue;
        }

        job->addSkill(skill);
    }

    return job;
}

Candidate *createCandidateStruct(const int id,const string &line)
{
    Candidate *cand = new Candidate();
    cand->initSkills();

    cand->id = id;

    // Extract skills string
    string skillsStr = extractSkillsString(line);

    // Tokenise skills and add to candidate struct skills array
    stringstream ss(skillsStr);
    string skill;
    while (getline(ss, skill, ','))
    {
        // Trim whitespace
        skill = skill.erase(0, skill.find_first_not_of(" "));
        skill = skill.erase(skill.find_last_not_of(" ") + 1);

        // Skip if the skill is not capitalized
        if (skill.empty() || !isCapital(skill[0]))
        {
            continue;
        }

        cand->addSkill(skill);
    }

    return cand;
}

JobArray *fetchJobsArray()
{
    ifstream file(JOB_DESCRIPTION_PATH);
    string line;
    int index = 0;
    JobArray *jobArray = new JobArray();

    if (!file.is_open())
    {
        cerr << "Error opening file: " << JOB_DESCRIPTION_PATH << endl;
        return nullptr;
    }

    // skip header line
    getline(file, line);

    while (getline(file, line))
    {
        Job *newJob = createJobStruct(index, line);
        jobArray->addJob(*newJob);
        index++;
    }

    file.close();
    return jobArray;
}

CandidateArray *fetchCandidatesArray()
{
    ifstream file(RESUME_PATH);
    string line;
    int index = 0;
    CandidateArray *candArray = new CandidateArray();

    if (!file.is_open())
    {
        cerr << "Error opening file: " << RESUME_PATH << endl;
        return nullptr;
    }

    // skip header line
    getline(file, line);

    while (getline(file, line))
    {
        Candidate *newCand = createCandidateStruct(index, line);
        candArray->addCandidate(*newCand);
        index++;
    }

    file.close();
    return candArray;
}

// Linked list utility functions
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

JobDescriptionNode *createJobDescription(const int index, const string &line)
{
    JobDescriptionNode *jobDesc = new JobDescriptionNode();
    jobDesc->jobID = index;
    jobDesc->jobTitle = extractJobTitle(line);
    string skillsStr = extractSkillsString(line);
    SkillNode *skillLinkedListHead = createSkillLinkedList(skillsStr);
    jobDesc->skillLinkedListHead = skillLinkedListHead;
    return jobDesc;
}

JobDescriptionNode *fetchJobs()
{
    ifstream file(JOB_DESCRIPTION_PATH);
    string line;
    int index = 0;
    JobDescriptionNode *head = nullptr;
    JobDescriptionNode *current = nullptr;

    if (!file.is_open())
    {
        cerr << "Error opening file: " << JOB_DESCRIPTION_PATH << endl;
        return nullptr;
    }

    // skip header line
    getline(file, line);

    while (getline(file, line))
    {
        JobDescriptionNode *newJobDesc = createJobDescription(index, line);
        JobDescriptionNode *newJobNode = new JobDescriptionNode();
        *newJobNode = *newJobDesc;
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

ResumeNode *createResume(const int index, const string &line)
{
    ResumeNode *resume = new ResumeNode();
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
        ResumeNode *newResume = createResume(index, line);
        ResumeNode *newResumeNode = new ResumeNode();
        *newResumeNode = *newResume;
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
