#include "sorting.hpp"

// Use fast and slow pointer to find the middle node
SkillNode* getMiddleSkillNode(SkillNode *head) {
    // If the list is empty or has only one element return head
    if (!head || !head->next) return head;

    SkillNode* slow = head;
    SkillNode* fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

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

    SkillNode* mid = getMiddleSkillNode(head);
    SkillNode* secondHalf = mid->next;
    mid->next = NULL;

    SkillNode* left = mergeSortSkillLinkedList(head);
    SkillNode* right = mergeSortSkillLinkedList(secondHalf);

    return mergeTwoSortedSkillLists(left, right);
}