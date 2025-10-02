#include "searching.hpp"

bool binarySearchSkillList(SkillNode *head, const string &target) {
    SkillNode* left = head;
    SkillNode* right = nullptr;

    while (left != right) {
        SkillNode* mid = getMiddleSkillNodeForSearch(left, right);
        if (!mid) return false;

        if (mid->skillName == target) {
            return true;
        } else if (mid->skillName < target) {
            left = mid->next;
        } else {
            right = mid;
        }
    }

    return false;
}