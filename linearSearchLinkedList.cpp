#include <iostream>
#include "linearSearchLinkedList.hpp"
using namespace std;

// Linear search in linked list
SkillNode* linearSearchSkill(SkillNode* head, const string& target) {
    SkillNode* current = head;
    while (current != nullptr) {
        if (current->skillName == target) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}
