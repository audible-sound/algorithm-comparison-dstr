#include <string>
#include <iostream>
#include <chrono>
#include "fileUtils.hpp"
#include "linkedList.hpp"
#include "searching.hpp"
#include "sorting.hpp"

using namespace std;
using namespace std::chrono;

void printTaskOptions()
{
    cout << "Select an option:\n";
    cout << "1. Find matching jobs by skill\n";
    cout << "2. Find matching resumes by skill\n";
    cout << "3. Exit\n";
}

void printDSOptions()
{
    cout << "Select a data structure:\n";
    cout << "1. Array\n";
    cout << "2. Linked List\n";
}

void printSortingOptions()
{
    cout << "Select a sorting algorithm:\n";
    cout << "1. Merge Sort\n";
    cout << "2. Quick Sort\n";
}

void printSearchingOptions()
{
    cout << "Select a searching algorithm:\n";
    cout << "1. Linear Search\n";
    cout << "2. Binary Search\n";
}

void getSkillsInputLinkedList(int *count, SkillNode *&head, size_t *memoryUsage)
{
    cout << "Enter skills (type 'done' to finish):\n";
    string skill;
    SkillNode *tail = nullptr;
    *memoryUsage = 0;
    while (true)
    {
        cin >> skill;
        if (skill == "done")
            break;
        SkillNode *newNode = new SkillNode();
        newNode->skillName = skill;
        newNode->next = nullptr;

        // update counter
        (*count)++;

        if (!head)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
        *memoryUsage += sizeof(SkillNode) + skill.capacity();
    }
    cout << endl;
}

void printMatchedJobs(JobDescription *matchedJobsHead, const int jobMatchCount)
{
    cout << "Matched Jobs:\n";
    JobDescription *temp = matchedJobsHead;
    while (temp)
    {
        cout << "Job ID: " << temp->jobID << ", Title: " << temp->jobTitle << endl;
        cout << "Required Skills: ";
        SkillNode *skillTemp = temp->skillLinkedListHead;
        while (skillTemp)
        {
            cout << skillTemp->skillName;

            if (skillTemp->next)
                cout << ", ";

            skillTemp = skillTemp->next;
        }
        cout << endl;
        temp = temp->next;
        cout << "---------------------\n"
             << endl;
    }
    cout << "Total matched jobs: " << jobMatchCount << endl;
}

void findMatchingJobsLinkedList(const int sortChoice, const int searchChoice, SkillNode *skillHead, const int count, size_t *fetchDataMemory_ptr, size_t *matchingMemory_ptr)
{
    using namespace std::chrono;
    cout << "Finding matching jobs...\n";

    // Measure fetch time
    auto fetchStart = high_resolution_clock::now();
    JobDescription *jobs = fetchJobs();
    auto fetchEnd = high_resolution_clock::now();
    auto fetchDuration = duration_cast<milliseconds>(fetchEnd - fetchStart);

    JobDescription *currentJob = jobs;
    JobDescription *matchedJobsHead = nullptr;
    JobDescription *matchedJobsTail = nullptr;
    int jobsMatchCount = 0;

    *fetchDataMemory_ptr = 0;

    // Measure sort time
    auto sortStart = high_resolution_clock::now();
    while (currentJob)
    {
        SkillNode *jobSkill = currentJob->skillLinkedListHead;
        if (sortChoice == 1)
        {
            jobSkill = mergeSortSkillLinkedList(jobSkill);
            currentJob->skillLinkedListHead = jobSkill;
        }
        else if (sortChoice == 2)
        {
            // implement later
        }
        *fetchDataMemory_ptr += sizeof(JobDescription) + currentJob->jobTitle.capacity();
        currentJob = currentJob->next;
    }
    auto sortEnd = high_resolution_clock::now();
    auto sortDuration = duration_cast<milliseconds>(sortEnd - sortStart);

    // Measure match time
    auto matchStart = high_resolution_clock::now();
    currentJob = jobs;
    while (currentJob)
    {
        int skillMatchCount = 0;
        SkillNode *jobSkill = currentJob->skillLinkedListHead;

        if (searchChoice == 2)
        {
            SkillNode *userSkill = skillHead;
            while (userSkill)
            {
                if (binarySearchSkillList(jobSkill, userSkill->skillName))
                {
                    skillMatchCount++;
                }
                userSkill = userSkill->next;
            }
        }

        double matchPercentage = (double)skillMatchCount / count * 100.0;
        if (matchPercentage >= 50.0)
        {
            JobDescription *newMatch = new JobDescription();
            newMatch->jobID = currentJob->jobID;
            newMatch->jobTitle = currentJob->jobTitle;
            newMatch->skillLinkedListHead = currentJob->skillLinkedListHead;
            newMatch->next = nullptr;
            if (!matchedJobsHead)
            {
                matchedJobsHead = newMatch;
                matchedJobsTail = newMatch;
            }
            else
            {
                matchedJobsTail->next = newMatch;
                matchedJobsTail = newMatch;
            }
            *matchingMemory_ptr += sizeof(JobDescription) + newMatch->jobTitle.capacity();
            jobsMatchCount++;
        }
        currentJob = currentJob->next;
    }
    auto matchEnd = high_resolution_clock::now();
    auto matchDuration = duration_cast<milliseconds>(matchEnd - matchStart);

    printMatchedJobs(matchedJobsHead, jobsMatchCount);

    cout << "Fetch Time: " << fetchDuration.count() << " ms\n";
    cout << "Sort Time: " << sortDuration.count() << " ms\n";
    cout << "Match Time: " << matchDuration.count() << " ms\n";
}

void printMatchedResumes(Resume *matchedResumesHead, const int resumeMatchCount)
{
    cout << "Matched Resumes:\n";
    Resume *temp = matchedResumesHead;
    while (temp)
    {
        cout << "Resume ID: " << temp->resumeID << endl;
        cout << "Skills: ";
        SkillNode *skillTemp = temp->skillLinkedListHead;
        while (skillTemp)
        {
            cout << skillTemp->skillName;

            if (skillTemp->next)
                cout << ", ";

            skillTemp = skillTemp->next;
        }
        cout << endl;
        temp = temp->next;
        cout << "---------------------\n"
             << endl;
    }
    cout << "Total matched resumes: " << resumeMatchCount << endl;
}

void findMatchingResumesLinkedList(const int sortChoice, const int searchChoice, SkillNode *skillHead, const int count, size_t *fetchDataMemory_ptr, size_t *matchingMemory_ptr)
{
    using namespace std::chrono;
    cout << "Finding matching resumes...\n";

    // Measure fetch time
    auto fetchStart = high_resolution_clock::now();
    Resume *resumes = fetchResumes();
    auto fetchEnd = high_resolution_clock::now();
    auto fetchDuration = duration_cast<milliseconds>(fetchEnd - fetchStart);

    Resume *currentResume = resumes;
    Resume *matchedResumesHead = nullptr;
    Resume *matchedResumesTail = nullptr;
    int resumeMatchCount = 0;
    *fetchDataMemory_ptr = 0;
    *matchingMemory_ptr = 0;

    // Measure sort time
    auto sortStart = high_resolution_clock::now();
    while (currentResume)
    {
        SkillNode *resumeSkill = currentResume->skillLinkedListHead;
        if (sortChoice == 1)
        {
            resumeSkill = mergeSortSkillLinkedList(resumeSkill);
            currentResume->skillLinkedListHead = resumeSkill;
        }
        else if (sortChoice == 2)
        {
            // implement later
        }
        *fetchDataMemory_ptr += sizeof(Resume) + sizeof(int);
        currentResume = currentResume->next;
    }
    auto sortEnd = high_resolution_clock::now();
    auto sortDuration = duration_cast<milliseconds>(sortEnd - sortStart);

    // Measure match time
    auto matchStart = high_resolution_clock::now();
    currentResume = resumes;
    while (currentResume)
    {
        int skillMatchCount = 0;
        SkillNode *resumeSkill = currentResume->skillLinkedListHead;

        if (searchChoice == 2)
        {
            SkillNode *userSkill = skillHead;
            while (userSkill)
            {
                if (binarySearchSkillList(resumeSkill, userSkill->skillName))
                {
                    skillMatchCount++;
                }
                userSkill = userSkill->next;
            }
        }

        double matchPercentage = (double)skillMatchCount / count * 100.0;
        if (matchPercentage >= 50.0)
        {
            Resume *newMatch = new Resume();
            newMatch->resumeID = currentResume->resumeID;
            newMatch->skillLinkedListHead = currentResume->skillLinkedListHead;
            newMatch->next = nullptr;
            if (!matchedResumesHead)
            {
                matchedResumesHead = newMatch;
                matchedResumesTail = newMatch;
            }
            else
            {
                matchedResumesTail->next = newMatch;
                matchedResumesTail = newMatch;
            }
            *matchingMemory_ptr += sizeof(Resume) + sizeof(int);
            resumeMatchCount++;
        }
        currentResume = currentResume->next;
    }
    auto matchEnd = high_resolution_clock::now();
    auto matchDuration = duration_cast<milliseconds>(matchEnd - matchStart);

    printMatchedResumes(matchedResumesHead, resumeMatchCount);

    cout << "Fetch Time: " << fetchDuration.count() << " ms\n";
    cout << "Sort Time: " << sortDuration.count() << " ms\n";
    cout << "Match Time: " << matchDuration.count() << " ms\n";
}

int main()
{
    printTaskOptions();
    int taskChoice;
    cin >> taskChoice;
    cout << endl;

    if (taskChoice == 3)
    {
        cout << "Exiting program.\n";
        return 0;
    }

    printDSOptions();
    int dsChoice;
    cin >> dsChoice;
    cout << endl;

    printSortingOptions();
    int sortChoice;
    cin >> sortChoice;
    cout << endl;

    printSearchingOptions();
    int searchChoice;
    cin >> searchChoice;
    cout << endl;

    int totalExecutionTime = 0;
    size_t skillInputMemory;
    size_t *skillInputMemory_ptr = &skillInputMemory;
    size_t fetchDataMemory;
    size_t *fetchDataMemory_ptr = &fetchDataMemory;
    size_t matchingMemory;
    size_t *matchingMemory_ptr = &matchingMemory;

    if (dsChoice == 2)
    { // Linked List
        SkillNode *skillHead = nullptr;
        int count = 0;
        int *count_ptr = &count;

        getSkillsInputLinkedList(count_ptr, skillHead, skillInputMemory_ptr);

        auto start = high_resolution_clock::now();
        if (taskChoice == 1) // Find matching jobs
        {
            findMatchingJobsLinkedList(sortChoice, searchChoice, skillHead, count, fetchDataMemory_ptr, matchingMemory_ptr);
        }
        else if (taskChoice == 2)
        { // Find matching resumes
            findMatchingResumesLinkedList(sortChoice, searchChoice, skillHead, count, fetchDataMemory_ptr, matchingMemory_ptr);
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        totalExecutionTime += duration.count();
    }

    cout << "Total Program Execution Time: " << totalExecutionTime << " ms\n";
    cout << "Estimated Skill Input Memory Usage: " << skillInputMemory << " bytes\n";
    cout << "Estimated Fetch Data Memory Usage: " << fetchDataMemory << " bytes\n";
    cout << "Estimated Matching Memory Usage: " << matchingMemory << " bytes\n";
    cout << "Estimated Total Memory Usage: " << (skillInputMemory + fetchDataMemory + matchingMemory) << " bytes\n";
    cout << "Exiting program.\n";
    return 0;
}