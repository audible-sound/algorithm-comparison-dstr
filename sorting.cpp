#include "sorting.hpp"

// Recursively merge two sorted linked lists
SkillNode* mergeTwoSortedSkillLists(SkillNode *left, SkillNode *right) {
    // If list is empty return the other list
    if (!left) return right;
    if (!right) return left;

    SkillNode* result = nullptr;
    if (left->skillName <= right->skillName) {
        result = left;
        result->next = mergeTwoSortedSkillLists(left->next, right);
    } else {
        result = right;
        result->next = mergeTwoSortedSkillLists(left, right->next);
    }
    return result;
}

// Merge sort implementation for linked list
SkillNode *mergeSortSkillLinkedList(SkillNode *head) {
    if (!head || !head->next) return head;

    SkillNode* mid = getMiddleSkillNodeForSort(head);
    SkillNode* secondHalf = mid->next;
    mid->next = nullptr;

    SkillNode* left = mergeSortSkillLinkedList(head);
    SkillNode* right = mergeSortSkillLinkedList(secondHalf);

    return mergeTwoSortedSkillLists(left, right);
}