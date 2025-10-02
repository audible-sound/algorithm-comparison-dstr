#include "searching.hpp"
#include "sorting.hpp"

SkillNode* binarySearchSkillList(SkillNode *head, const string &target) {
    SkillNode* start = head;
    SkillNode* end = nullptr;

    while (start != end)
    {
        SkillNode* mid = getMiddleSkillNode(start);

        if (mid == nullptr)
            return nullptr;

        if (mid->skillName == target)
            return mid;
        else if (mid->skillName < target) // Move to the right half
            start = mid->next;
        else // Move to the left half
            end = mid;
    }
    
    return nullptr;
}