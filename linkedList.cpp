#include "linkedList.hpp"

// Use fast and slow pointer to find the middle node
SkillNode *getMiddleSkillNodeForSort(SkillNode *head)
{
    // If the list is empty or has only one element return head
    if (!head || !head->next)
        return head;

    SkillNode *slow = head;
    SkillNode *fast = head->next;

    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

// Use fast and slow pointer to find the middle node for search
SkillNode *getMiddleSkillNodeForSearch(SkillNode *start, SkillNode *end)
{
    if (!start)
        return nullptr;

    SkillNode *slow = start;
    SkillNode *fast = start;

    // move until fast or fast->next reaches end
    while (fast->next != end && fast->next && fast->next->next != end)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

ResumeNode *getMiddleResumeNodeForSort(ResumeNode *head)
{
    if (!head || !head->next)
        return head;

    ResumeNode *slow = head;
    ResumeNode *fast = head->next;

    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

JobDescriptionNode *getMiddleJobNodeForSort(JobDescriptionNode *head)
{
    if (!head || !head->next)
        return head;

    JobDescriptionNode *slow = head;
    JobDescriptionNode *fast = head->next;

    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}
