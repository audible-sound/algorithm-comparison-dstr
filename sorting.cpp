#include "sorting.hpp"

// Array sorting functions
// Merge sort implementation for string array
string *mergeTwoSortedStringArrays(string *left, int leftSize, string *right, int rightSize)
{
    string *merged = new string[leftSize + rightSize];
    int i = 0, j = 0, k = 0;
    while (i < leftSize && j < rightSize)
    {
        if (left[i] <= right[j])
        {
            merged[k++] = left[i++];
        }
        else
        {
            merged[k++] = right[j++];
        }
    }
    while (i < leftSize)
    {
        merged[k++] = left[i++];
    }
    while (j < rightSize)
    {
        merged[k++] = right[j++];
    }
    return merged;
}

string *mergeSortStringArray(string *arr, int size)
{
    if (size <= 1)
        return arr;
    int mid = size / 2;
    string *left = mergeSortStringArray(arr, mid);
    string *right = mergeSortStringArray(arr + mid, size - mid);
    return mergeTwoSortedStringArrays(left, mid, right, size - mid);
}

ResultCandidate *mergeCandidate(ResultCandidate *left, int leftSize, ResultCandidate *right, int rightSize)
{
    ResultCandidate *merged = new ResultCandidate[leftSize + rightSize];
    int i = 0, j = 0, k = 0;
    while (i < leftSize && j < rightSize)
    {
        if (left[i].matchPercentage <= right[j].matchPercentage)
        {
            merged[k++] = left[i++];
        }
        else
        {
            merged[k++] = right[j++];
        }
    }
    while (i < leftSize)
    {
        merged[k++] = left[i++];
    }
    while (j < rightSize)
    {
        merged[k++] = right[j++];
    }
    return merged;
}

ResultCandidate *mergeSortCandidate(ResultCandidate *arr, int size)
{
    if (size <= 1)
        return arr;
    int mid = size / 2;
    ResultCandidate *left = mergeSortCandidate(arr, mid);
    ResultCandidate *right = mergeSortCandidate(arr + mid, size - mid);
    return mergeCandidate(left, mid, right, size - mid);
}

ResultJob *mergeJob(ResultJob *left, int leftSize, ResultJob *right, int rightSize)
{
    ResultJob *merged = new ResultJob[leftSize + rightSize];
    int i = 0, j = 0, k = 0;
    while (i < leftSize && j < rightSize)
    {
        if (left[i].matchPercentage <= right[j].matchPercentage)
        {
            merged[k++] = left[i++];
        }
        else
        {
            merged[k++] = right[j++];
        }
    }
    while (i < leftSize)
    {
        merged[k++] = left[i++];
    }
    while (j < rightSize)
    {
        merged[k++] = right[j++];
    }
    return merged;
}

ResultJob *mergeSortJob(ResultJob *arr, int size)
{
    if (size <= 1)
        return arr;
    int mid = size / 2;
    ResultJob *left = mergeSortJob(arr, mid);
    ResultJob *right = mergeSortJob(arr + mid, size - mid);
    return mergeJob(left, mid, right, size - mid);
}

// Linked list sorting functions
// Recursively merge two sorted linked lists
SkillNode *mergeTwoSortedSkillLists(SkillNode *left, SkillNode *right)
{
    // If list is empty return the other list
    if (!left)
        return right;
    if (!right)
        return left;

    SkillNode *result = nullptr;
    if (left->skillName <= right->skillName)
    {
        result = left;
        result->next = mergeTwoSortedSkillLists(left->next, right);
    }
    else
    {
        result = right;
        result->next = mergeTwoSortedSkillLists(left, right->next);
    }
    return result;
}

// Merge sort implementation for linked list
SkillNode *mergeSortSkillLinkedList(SkillNode *head)
{
    if (!head || !head->next)
        return head;

    SkillNode *mid = getMiddleSkillNodeForSort(head);
    SkillNode *secondHalf = mid->next;
    mid->next = nullptr;

    SkillNode *left = mergeSortSkillLinkedList(head);
    SkillNode *right = mergeSortSkillLinkedList(secondHalf);

    return mergeTwoSortedSkillLists(left, right);
}

// Partition the list around pivot (last element)
SkillNode *partition(SkillNode *head, SkillNode *end, SkillNode **newHead, SkillNode **newEnd)
{
    SkillNode *pivot = end;
    SkillNode *prev = nullptr;
    SkillNode *cur = head;   // moveable pointer from head
    SkillNode *tail = pivot; // moveable pointer from tail

    // During partition, newHead and newEnd will be updated
    while (cur != pivot)
    {
        if (cur->skillName < pivot->skillName)
        {
            // First SkillNode with value < pivot becomes new head
            if (!(*newHead))
                *newHead = cur;

            prev = cur;
            cur = cur->next;
        }
        else
        {
            // Move SkillNode to end
            if (prev)
                prev->next = cur->next;
            SkillNode *tmp = cur->next;
            cur->next = nullptr;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }

    // If pivot is the smallest element, it becomes head
    if (!(*newHead))
        *newHead = pivot;

    *newEnd = tail;

    return pivot;
}

// Get the tail of jobSkill linked list
SkillNode *getTail(SkillNode *tail)
{
    while (tail && tail->next)
        tail = tail->next;
    return tail;
};

// Quicksort
SkillNode *quickSort(SkillNode *head, SkillNode *end)
{
    if (!head || head == end)
    {
        return head;
    }

    SkillNode *newHead = nullptr;
    SkillNode *newEnd = nullptr;

    // Partition the list
    SkillNode *pivot = partition(head, end, &newHead, &newEnd);

    // If pivot is not the smallest element, sort left part
    if (newHead != pivot)
    {
        // Find SkillNode before pivot
        SkillNode *tmp = newHead;
        while (tmp->next != pivot)
            tmp = tmp->next;
        tmp->next = nullptr;

        // Recur on left list
        newHead = quickSort(newHead, tmp);

        // Connect left part with pivot
        tmp = newHead;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = pivot;
    }

    // Recur on right part
    pivot->next = quickSort(pivot->next, newEnd);

    return newHead;
}