#pragma once
#include "linkedList.hpp"

// Linked List Search Functions
bool binarySearchSkillList(SkillNode *head, const string &target);
bool linearSearchSkill(SkillNode* head, const string& target);

// Array Search Functions
bool binarySearchSkillArray(string *skills, int arrSize, const string &target);