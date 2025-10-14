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

void printLinearSortingOptions()
{
    cout << "Do you want to sort array/linked list before linear search:\n";
    cout << "1. Yes\n";
    cout << "2. No\n";
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

void printMatchedJobsLinkedList(JobDescriptionNode *matchedJobsHead, const int jobMatchCount)
{
    cout << "Matched Jobs:\n";
    JobDescriptionNode *temp = matchedJobsHead;
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
        cout << "Match Percentage: " << temp->matchPercentage << "%\n";
        cout << endl;
        temp = temp->next;
        cout << "---------------------\n"
             << endl;
    }
    cout << "Total matched jobs: " << jobMatchCount << endl;
}

void findMatchingJobsLinkedList(
    const int sortChoice,
    const int searchChoice,
    const int linearSortChoice,
    SkillNode *skillHead,
    const int count,
    size_t *fetchDataMemory_ptr,
    size_t *matchingMemory_ptr)
{
    cout << "Finding matching jobs...\n";

    // Measure fetch time
    auto fetchStart = high_resolution_clock::now();
    JobDescriptionNode *jobs = fetchJobs();
    auto fetchEnd = high_resolution_clock::now();
    auto fetchDuration = duration_cast<milliseconds>(fetchEnd - fetchStart);

    JobDescriptionNode *currentJob = jobs;
    JobDescriptionNode *matchedJobsHead = nullptr;
    JobDescriptionNode *matchedJobsTail = nullptr;
    int jobsMatchCount = 0;

    *fetchDataMemory_ptr = 0;
    *matchingMemory_ptr = 0;

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
        else if (sortChoice == 2 && linearSortChoice == 1)
        {
            SkillNode *tail = getTail(jobSkill);
            SkillNode *afterSort = quickSort(jobSkill, getTail(jobSkill));
            currentJob->skillLinkedListHead = afterSort;
        }
        *fetchDataMemory_ptr += sizeof(JobDescriptionNode) + currentJob->jobTitle.capacity();
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
        else if (searchChoice == 1)
        {
            SkillNode *userSkill = skillHead;
            while (userSkill)
            {
                if (linearSearchSkill(jobSkill, userSkill->skillName))
                {
                    skillMatchCount++;
                }
                userSkill = userSkill->next;
            }
        }

        double matchPercentage = (double)skillMatchCount / count * 100.0;
        if (matchPercentage >= 50.0)
        {
            JobDescriptionNode *newMatch = new JobDescriptionNode();
            newMatch->jobID = currentJob->jobID;
            newMatch->jobTitle = currentJob->jobTitle;
            newMatch->skillLinkedListHead = currentJob->skillLinkedListHead;
            newMatch->matchPercentage = matchPercentage;
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
            *matchingMemory_ptr += sizeof(JobDescriptionNode);
            jobsMatchCount++;
        }
        currentJob = currentJob->next;
    }
    auto matchEnd = high_resolution_clock::now();
    auto matchDuration = duration_cast<milliseconds>(matchEnd - matchStart);

    long long additionalSortDuration;

    if (sortChoice == 1)
    {
        auto sortResultStart = high_resolution_clock::now();
        matchedJobsHead = mergeSortJob(matchedJobsHead);
        auto sortResultEnd = high_resolution_clock::now();
        auto sortResultDuration = duration_cast<milliseconds>(sortResultEnd - sortResultStart);
        additionalSortDuration = sortResultDuration.count();
    }
    else if (sortChoice == 2)
    {
        auto sortResultStart = high_resolution_clock::now();
        JobDescriptionNode *tail = getTail(matchedJobsHead);
        matchedJobsHead = quickSort(matchedJobsHead, tail);
        auto sortResultEnd = high_resolution_clock::now();
        auto sortResultDuration = duration_cast<milliseconds>(sortResultEnd - sortResultStart);
        additionalSortDuration = sortResultDuration.count();
    }

    printMatchedJobsLinkedList(matchedJobsHead, jobsMatchCount);

    cout << "Fetch Time: " << fetchDuration.count() << " ms\n";
    cout << "Sort Time: " << sortDuration.count() << " ms\n";
    cout << "Match Time: " << matchDuration.count() << " ms\n";
}

void printMatchedResumesLinkedList(ResumeNode *matchedResumesHead, const int resumeMatchCount)
{
    cout << "Matched Resumes:\n";
    ResumeNode *temp = matchedResumesHead;
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
        cout << "Match Percentage: " << temp->matchPercentage << "%\n";
        cout << endl;
        temp = temp->next;
        cout << "---------------------\n"
             << endl;
    }
    cout << "Total matched resumes: " << resumeMatchCount << endl;
}

void findMatchingResumesLinkedList(
    const int sortChoice,
    const int searchChoice,
    const int linearSortChoice,
    SkillNode *skillHead,
    const int count,
    size_t *fetchDataMemory_ptr,
    size_t *matchingMemory_ptr)
{
    cout << "Finding matching resumes...\n";

    // Measure fetch time
    auto fetchStart = high_resolution_clock::now();
    ResumeNode *resumes = fetchResumes();
    auto fetchEnd = high_resolution_clock::now();
    auto fetchDuration = duration_cast<milliseconds>(fetchEnd - fetchStart);

    ResumeNode *currentResume = resumes;
    ResumeNode *matchedResumesHead = nullptr;
    ResumeNode *matchedResumesTail = nullptr;
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
        else if (sortChoice == 2 && linearSortChoice == 1)
        {
            SkillNode *tail = getTail(resumeSkill);
            SkillNode *afterSort = quickSort(resumeSkill, getTail(resumeSkill));
            currentResume->skillLinkedListHead = afterSort;
        }
        *fetchDataMemory_ptr += sizeof(ResumeNode) + sizeof(int);
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
        else if (searchChoice == 1)
        {
            SkillNode *userSkill = skillHead;
            while (userSkill)
            {
                if (linearSearchSkill(resumeSkill, userSkill->skillName))
                {
                    skillMatchCount++;
                }
                userSkill = userSkill->next;
            }
        }

        double matchPercentage = (double)skillMatchCount / count * 100.0;
        if (matchPercentage >= 50.0)
        {
            ResumeNode *newMatch = new ResumeNode();
            newMatch->resumeID = currentResume->resumeID;
            newMatch->skillLinkedListHead = currentResume->skillLinkedListHead;
            newMatch->matchPercentage = matchPercentage;
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
            *matchingMemory_ptr += sizeof(ResumeNode) + sizeof(int);
            resumeMatchCount++;
        }
        currentResume = currentResume->next;
    }
    auto matchEnd = high_resolution_clock::now();
    auto matchDuration = duration_cast<milliseconds>(matchEnd - matchStart);

    long long additionalSortDuration;

    if (sortChoice == 1)
    {
        auto sortResultStart = high_resolution_clock::now();
        matchedResumesHead = mergeSortResume(matchedResumesHead);
        auto sortResultEnd = high_resolution_clock::now();
        auto sortResultDuration = duration_cast<milliseconds>(sortResultEnd - sortResultStart);
        additionalSortDuration = sortResultDuration.count();
    }
    else if (sortChoice == 2)
    {
        auto sortResultStart = high_resolution_clock::now();
        ResumeNode *tail = getTail(matchedResumesHead);
        matchedResumesHead = quickSort(matchedResumesHead, tail);
        auto sortResultEnd = high_resolution_clock::now();
        auto sortResultDuration = duration_cast<milliseconds>(sortResultEnd - sortResultStart);
        additionalSortDuration = sortResultDuration.count();
    }

    printMatchedResumesLinkedList(matchedResumesHead, resumeMatchCount);

    cout << "Fetch Time: " << fetchDuration.count() << " ms\n";
    cout << "Sort Time: " << sortDuration.count() + additionalSortDuration << " ms\n";
    cout << "Match Time: " << matchDuration.count() << " ms\n";
}

void getSkillsInputArray(int &count, string *&skills, size_t *memoryUsage)
{
    cout << "Enter skills (type 'done' to finish):\n";
    string skill;
    count = 0;
    int capacity = 10;
    skills = new string[capacity];
    *memoryUsage = 0;

    while (true)
    {
        cin >> skill;
        if (skill == "done")
            break;

        if (count >= capacity)
        {
            capacity *= 2;
            string *newSkills = new string[capacity];
            for (int i = 0; i < count; i++)
                newSkills[i] = skills[i];
            delete[] skills;
            skills = newSkills;
        }
        skills[count] = skill;
        count++;
        *memoryUsage += sizeof(string) + skill.capacity();
    }
    cout << endl;
}

void printMatchedJobsArray(ResultJob *results, int resultCount)
{
    cout << "Matched Jobs:\n";
    for (int i = 0; i < resultCount; i++)
    {
        cout << "Job ID: " << results[i].job.id << ", Title: " << results[i].job.role << endl;
        cout << "Required Skills: ";
        for (int j = 0; j < results[i].job.skillCount; j++)
        {
            cout << results[i].job.skills[j];
            if (j < results[i].job.skillCount - 1)
                cout << ", ";
        }
        cout << endl;
        cout << "Match Percentage: " << results[i].matchPercentage << "%\n";
        cout << "---------------------\n"
             << endl;
    }
    cout << "Total Matched Jobs: " << resultCount << endl;
}

void findMatchingJobsArray(
    const int sortChoice,
    const int searchChoice,
    const int linearSortChoice,
    string *userSkills,
    const int userSkillCount,
    size_t *fetchDataMemory_ptr,
    size_t *matchingMemory_ptr)
{
    cout << "Finding matching jobs...\n";

    // Load jobs
    auto fetchStart = high_resolution_clock::now();
    JobArray *jobs = fetchJobsArray();
    auto fetchEnd = high_resolution_clock::now();
    auto fetchDuration = duration_cast<milliseconds>(fetchEnd - fetchStart);

    *fetchDataMemory_ptr = 0;
    *matchingMemory_ptr = 0;
    for (int i = 0; i < jobs->getSize(); i++)
    {
        Job &j = jobs->get(i);
        *fetchDataMemory_ptr += sizeof(Job) + j.role.capacity();
        for (int k = 0; k < j.skillCount; k++)
            *fetchDataMemory_ptr += j.skills[k].capacity();
    }

    // Sort job skills if needed
    auto sortStart = high_resolution_clock::now();
    if (sortChoice == 1)
    {
        for (int i = 0; i < jobs->getSize(); i++)
        {
            Job &j = jobs->get(i);
            j.skills = mergeSortStringArray(j.skills, j.skillCount);
        }
    }
    else if (sortChoice == 2 && linearSortChoice == 1)
    {
        // Implement linear sort later
        for (int i = 0; i < jobs->getSize(); i++)
        {
            Job &j = jobs->get(i);
            quickSort(j.skills, 0, j.skillCount - 1);
        }
    }
    auto sortEnd = high_resolution_clock::now();
    auto sortDuration = duration_cast<milliseconds>(sortEnd - sortStart);

    // Find matches
    ResultJob *results = new ResultJob[jobs->getSize()];
    *matchingMemory_ptr = sizeof(ResultJob) * jobs->getSize();
    int resultCount = 0;

    auto searchStart = high_resolution_clock::now();
    for (int i = 0; i < jobs->getSize(); i++)
    {
        Job &j = jobs->get(i);
        int matchCount = 0;

        if (searchChoice == 2)
        {
            for (int u = 0; u < userSkillCount; u++)
                if (binarySearchSkillArray(j.skills, j.skillCount, userSkills[u]))
                    matchCount++;
        }
        else if (searchChoice == 1)
        {
            for (int u = 0; u < userSkillCount; u++)
                if (linerarSearchSkillArray(j.skills, j.skillCount, userSkills[u]))
                    matchCount++;
        }
        double matchPercentage = (double)matchCount / userSkillCount * 100.0;

        if (matchPercentage >= 50.0)
        {
            results[resultCount] = {j, matchPercentage};
            resultCount++;
        }
    }
    auto searchEnd = high_resolution_clock::now();
    auto matchDuration = duration_cast<milliseconds>(searchEnd - searchStart);

    long long additionalSortDuration;

    if (sortChoice == 1)
    {
        auto sortResultStart = high_resolution_clock::now();
        results = mergeSortJob(results, resultCount);
        auto sortResultEnd = high_resolution_clock::now();
        auto sortResultDuration = duration_cast<milliseconds>(sortResultEnd - sortResultStart);
        additionalSortDuration = sortResultDuration.count();
    }
    else if (sortChoice == 2)
    {
        auto sortResultStart = high_resolution_clock::now();
        quickSort(results, 0, resultCount);
        auto sortResultEnd = high_resolution_clock::now();
        auto sortResultDuration = duration_cast<milliseconds>(sortResultEnd - sortResultStart);
        additionalSortDuration = sortResultDuration.count();
    }

    printMatchedJobsArray(results, resultCount);

    cout << "Fetch Time: " << fetchDuration.count() << " ms\n";
    cout << "Sort Time: " << sortDuration.count() + additionalSortDuration << " ms\n";
    cout << "Match Time: " << matchDuration.count() << " ms\n";
}

void printMatchedResumesArray(ResultCandidate *results, int resultCount)
{
    cout << "Matched Jobs:\n";
    for (int i = 0; i < resultCount; i++)
    {
        cout << "Resume ID: " << results[i].candidate.id << endl;
        cout << "Skills: ";
        for (int j = 0; j < results[i].candidate.skillCount; j++)
        {
            cout << results[i].candidate.skills[j];
            if (j < results[i].candidate.skillCount - 1)
                cout << ", ";
        }
        cout << endl;
        cout << "Match Percentage: " << results[i].matchPercentage << "%\n";
        cout << "---------------------\n"
             << endl;
    }
    cout << "Total Matched Resumes: " << resultCount << endl;
}

void findMatchingResumesArray(
    const int sortChoice,
    const int searchChoice,
    const int linearSortChoice,
    string *userSkills,
    const int userSkillCount,
    size_t *fetchDataMemory_ptr,
    size_t *matchingMemory_ptr)
{
    cout << "Finding matching jobs...\n";

    // Load jobs
    auto fetchStart = high_resolution_clock::now();
    CandidateArray *candidates = fetchCandidatesArray();
    auto fetchEnd = high_resolution_clock::now();
    auto fetchDuration = duration_cast<milliseconds>(fetchEnd - fetchStart);

    *fetchDataMemory_ptr = 0;
    *matchingMemory_ptr = 0;
    for (int i = 0; i < candidates->getSize(); i++)
    {
        Candidate &c = candidates->get(i);
        *fetchDataMemory_ptr += sizeof(Candidate);
        for (int k = 0; k < c.skillCount; k++)
            *fetchDataMemory_ptr += c.skills[k].capacity();
    }

    // Sort job skills if needed
    auto sortStart = high_resolution_clock::now();
    if (sortChoice == 1)
    {
        for (int i = 0; i < candidates->getSize(); i++)
        {
            Candidate &c = candidates->get(i);
            c.skills = mergeSortStringArray(c.skills, c.skillCount);
        }
    }
    else if (sortChoice == 2 && linearSortChoice == 1)
    {
        for (int i = 0; i < candidates->getSize(); i++)
        {
            Candidate &c = candidates->get(i);
            quickSort(c.skills, 0, c.skillCount - 1);
        }
    }
    auto sortEnd = high_resolution_clock::now();
    auto sortDuration = duration_cast<milliseconds>(sortEnd - sortStart);

    // Find matches
    ResultCandidate *results = new ResultCandidate[candidates->getSize()];
    *matchingMemory_ptr = sizeof(ResultCandidate) * candidates->getSize();
    int resultCount = 0;

    auto searchStart = high_resolution_clock::now();
    for (int i = 0; i < candidates->getSize(); i++)
    {
        Candidate &c = candidates->get(i);
        int matchCount = 0;

        if (searchChoice == 2)
        {
            for (int u = 0; u < userSkillCount; u++)
                if (binarySearchSkillArray(c.skills, c.skillCount, userSkills[u]))
                    matchCount++;
        }
        else if (searchChoice == 1)
        {
            // Implement linear search later
            for (int u = 0; u < userSkillCount; u++)
                if (linerarSearchSkillArray(c.skills, c.skillCount, userSkills[u]))
                    matchCount++;
        }
        double matchPercentage = (double)matchCount / userSkillCount * 100.0;

        if (matchPercentage >= 50.0)
        {
            results[resultCount] = {c, matchPercentage};
            resultCount++;
        }
    }
    auto searchEnd = high_resolution_clock::now();
    auto matchDuration = duration_cast<milliseconds>(searchEnd - searchStart);

    long long additionalSortDuration;

    if (sortChoice == 1)
    {
        auto sortResultStart = high_resolution_clock::now();
        results = mergeSortCandidate(results, resultCount);
        auto sortResultEnd = high_resolution_clock::now();
        auto sortResultDuration = duration_cast<milliseconds>(sortResultEnd - sortResultStart);
        additionalSortDuration = sortResultDuration.count();
    }
    else if (sortChoice == 2)
    {
        auto sortResultStart = high_resolution_clock::now();
        quickSort(results, 0, resultCount);
        auto sortResultEnd = high_resolution_clock::now();
        auto sortResultDuration = duration_cast<milliseconds>(sortResultEnd - sortResultStart);
        additionalSortDuration = sortResultDuration.count();
    }

    printMatchedResumesArray(results, resultCount);

    cout << "Fetch Time: " << fetchDuration.count() << " ms\n";
    cout << "Sort Time: " << sortDuration.count() + additionalSortDuration << " ms\n";
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

    int linearSortChoice = 2;
    if (searchChoice == 1)
    {
        printLinearSortingOptions();
        cin >> linearSortChoice;
        cout << endl;
    }

    int totalExecutionTime = 0;
    size_t skillInputMemory;
    size_t *skillInputMemory_ptr = &skillInputMemory;
    size_t fetchDataMemory;
    size_t *fetchDataMemory_ptr = &fetchDataMemory;
    size_t matchingMemory;
    size_t *matchingMemory_ptr = &matchingMemory;

    if (dsChoice == 1)
    { // Array
        string *userSkills = nullptr;
        int userSkillCount = 0;
        getSkillsInputArray(userSkillCount, userSkills, skillInputMemory_ptr);

        auto start = high_resolution_clock::now();
        if (taskChoice == 1) // Find matching jobs
        {
            findMatchingJobsArray(
                sortChoice,
                searchChoice,
                linearSortChoice,
                userSkills,
                userSkillCount,
                fetchDataMemory_ptr,
                matchingMemory_ptr);
        }
        else if (taskChoice == 2)
        { // Find matching resumes
            findMatchingResumesArray(
                sortChoice,
                searchChoice,
                linearSortChoice,
                userSkills,
                userSkillCount,
                fetchDataMemory_ptr,
                matchingMemory_ptr);
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        totalExecutionTime += duration.count();
        delete[] userSkills;
    }

    else if (dsChoice == 2)
    { // Linked List
        SkillNode *skillHead = nullptr;
        int count = 0;
        int *count_ptr = &count;

        getSkillsInputLinkedList(count_ptr, skillHead, skillInputMemory_ptr);

        auto start = high_resolution_clock::now();
        if (taskChoice == 1) // Find matching jobs
        {
            findMatchingJobsLinkedList(
                sortChoice,
                searchChoice,
                linearSortChoice,
                skillHead,
                count,
                fetchDataMemory_ptr,
                matchingMemory_ptr);
        }
        else if (taskChoice == 2)
        { // Find matching resumes
            findMatchingResumesLinkedList(
                sortChoice,
                searchChoice,
                linearSortChoice,
                skillHead,
                count,
                fetchDataMemory_ptr,
                matchingMemory_ptr);
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        totalExecutionTime += duration.count();
    }

    cout << "Total Program Execution Time: " << totalExecutionTime << " ms\n";
    cout << "Estimated Skill Input Memory Usage: " << skillInputMemory / 1024.0 << " KB\n";
    cout << "Estimated Fetch Data Memory Usage: " << fetchDataMemory / 1024.0 << " KB\n";
    cout << "Estimated Matching Memory Usage: " << matchingMemory / 1024.0 << " KB\n";
    cout << "Estimated Total Memory Usage: " << (skillInputMemory + fetchDataMemory + matchingMemory) / 1024.0 << " KB\n";
    cout << "Exiting program.\n";
    return 0;
}