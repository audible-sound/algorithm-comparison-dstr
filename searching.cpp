#include "searching.hpp"
#include "fileUtils.hpp"

// Array search
bool binarySearchSkillArray(string *skills, int arrSize, const string &target)
{
    int low = 0, high = arrSize - 1;
    string tgt = toLower(target);
    while (low <= high)
    {
        int mid = (low + high) / 2;
        string midVal = toLower(skills[mid]);
        if (midVal == tgt)
            return true;
        else if (midVal < tgt)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return false;
}

// Linked List search
bool binarySearchSkillList(SkillNode *head, const string &target) {
    SkillNode* left = head;
    SkillNode* right = nullptr;

    string tgt = toLower(target);

    while (left != right) {
        SkillNode* mid = getMiddleSkillNodeForSearch(left, right);
        if (!mid) return false;

        string midVal = toLower(mid->skillName);
        if (midVal == tgt) {
            return true;
        } else if (midVal < tgt) {
            left = mid->next;
        } else {
            right = mid;
        }
    }

    return false;
}

bool linearSearchSkill(SkillNode* head, const string& target) {
    SkillNode* current = head;
    string tgt = toLower(target);
    while (current != nullptr) {
        if (toLower(current->skillName) == tgt) {
            return true;
        }
        current = current->next;
    }
    return false;
}