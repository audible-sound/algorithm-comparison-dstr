#include <string>
#include <iostream>
#include "fileUtils.hpp"
#include "linkedList.hpp"
#include "searching.hpp"
#include "sorting.hpp"

using namespace std;
int main()
{
    string jobdesc = "Data Analyst needed with experience in SQL, Excel, Power BI, Reporting, Data Cleaning, Tableau. Anything real big. Suggest edge happen available treat somebody.";
    JobDescription* job = createJobDescription(1, jobdesc);
    job->skillLinkedListHead = mergeSortSkillLinkedList(job->skillLinkedListHead);

    cout << "Job ID: " << job->jobID << endl;
    cout << "Job Title: " << job->jobTitle << endl;
    cout << "Skills: ";
    SkillNode* current = job->skillLinkedListHead;
    while (current) {
        cout << current->skillName;
        if (current->next) cout << ", ";
        current = current->next;
    }
    cout << endl;

    string searchSkill = "Excel";
    bool found = binarySearchSkillList(job->skillLinkedListHead, searchSkill);
    cout << "Skill '" << searchSkill << "' found: " << (found ? "Yes" : "No") << endl;
    return 0;
}