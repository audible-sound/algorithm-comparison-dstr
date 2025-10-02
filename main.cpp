#include <string>
#include <iostream>
#include "fileUtils.hpp"
#include "linkedList.hpp"
#include "searching.hpp"
#include "sorting.hpp"

using namespace std;

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

void getSkillsInputLinkedList(int *count, SkillNode *&head)
{
    cout << "Enter skills (type 'done' to finish):\n";
    string skill;
    SkillNode *tail = nullptr;
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
    }
    cout << endl;
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

    if (dsChoice == 2)
    { // Linked List
        SkillNode *skillHead = nullptr;
        int count = 0;
        int *count_ptr = &count;
        getSkillsInputLinkedList(count_ptr, skillHead);

        if (taskChoice == 1)
        { // Jobs
            cout << "Finding matching jobs...\n";
            JobDescription *jobs = fetchJobs();
            JobDescription *currentJob = jobs;
            JobDescription *matchedJobsHead = nullptr;
            JobDescription *matchedJobsTail = nullptr;

            while (currentJob)
            {
                int matchCount = 0;
                SkillNode *jobSkill = currentJob->skillLinkedListHead;

                // sort skills
                if (sortChoice == 1)
                {
                    jobSkill = mergeSortSkillLinkedList(jobSkill);
                }
                else if (sortChoice == 2)
                {
                    // implement later
                }

                // compare skills
                if (searchChoice == 1)
                {
                    // Implement later
                }
                else if (searchChoice == 2)
                {
                    SkillNode *userSkill = skillHead;
                    while (userSkill)
                    {
                        if (binarySearchSkillList(jobSkill, userSkill->skillName))
                        {
                            matchCount++;
                        }
                        userSkill = userSkill->next;
                    }
                }

                double matchPercentage = (double)matchCount / count * 100.0;
                if (matchPercentage >= 50.0) // threshold
                {
                    // add to matched jobs list
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
                }

                currentJob = currentJob->next;
            }

            // Print matched jobs
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
                cout << "---------------------\n";
            }
        }
        else if (taskChoice == 2)
        { // Resumes
            cout << "Finding matching resumes...\n";
            // Placeholder for resume matching logic
        }
    }
    cout << "Exiting program.\n";
    return 0;
}