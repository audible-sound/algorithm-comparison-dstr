#include "quickSortLinkedList.hpp"
#include <iostream>
using namespace std;

// Partition the list around pivot (last element)
SkillNode* partition(SkillNode* head, SkillNode* end, SkillNode** newHead, SkillNode** newEnd) {
    SkillNode* pivot = end;
    SkillNode* prev = nullptr;
    SkillNode* cur = head; // moveable pointer from head
    SkillNode* tail = pivot; // moveable pointer from tail 

    // During partition, newHead and newEnd will be updated
    while (cur != pivot) {
        if (cur->skillName < pivot->skillName) {
            // First SkillNode with value < pivot becomes new head
            if (!(*newHead)) *newHead = cur;

            prev = cur;
            cur = cur->next;
        } else {
            // Move SkillNode to end
            if (prev) prev->next = cur->next;
            SkillNode* tmp = cur->next;
            cur->next = nullptr;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }

    // If pivot is the smallest element, it becomes head
    if (!(*newHead)) *newHead = pivot;

    *newEnd = tail;

    return pivot;
}

// Get the tail of jobSkill linked list
SkillNode* getTail(SkillNode* tail) {
    while (tail && tail->next) tail = tail->next;
    return tail;
};

// Quicksort
SkillNode* quickSort(SkillNode* head, SkillNode* end) {
    if (!head || head == end) {
        return head;
    }

    SkillNode *newHead = nullptr;
    SkillNode *newEnd = nullptr;

    // Partition the list
    SkillNode* pivot = partition(head, end, &newHead, &newEnd);

    // If pivot is not the smallest element, sort left part
    if (newHead != pivot) {
        // Find SkillNode before pivot
        SkillNode* tmp = newHead;
        while (tmp->next != pivot) tmp = tmp->next;
        tmp->next = nullptr;

        // Recur on left list
        newHead = quickSort(newHead, tmp);

        // Connect left part with pivot
        tmp = newHead;
        while (tmp->next) tmp = tmp->next;
        tmp->next = pivot;
    }

    // Recur on right part
    pivot->next = quickSort(pivot->next, newEnd);

    return newHead;
}