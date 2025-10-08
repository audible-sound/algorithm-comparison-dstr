#pragma once
#include "linkedList.hpp"
#include "array.hpp"

// Array sorting functions
string *mergeSortStringArray(string *arr, int size);
string *mergeTwoSortedStringArrays(string *left, int leftSize, string *right, int rightSize);

ResultCandidate *mergeCandidate(ResultCandidate *left, int leftSize, ResultCandidate *right, int rightSize);
ResultCandidate *mergeSortCandidate(ResultCandidate *arr, int size);

ResultJob *mergeJob(ResultJob *left, int leftSize, ResultJob *right, int rightSize);
ResultJob *mergeSortJob(ResultJob *arr, int size);

// Linked list sorting functions
SkillNode *mergeTwoSortedSkillLists(SkillNode *left, SkillNode *right);
SkillNode *mergeSortSkillLinkedList(SkillNode *head);

ResumeNode *mergeTwoSortedResume(ResumeNode *left, ResumeNode *right);
ResumeNode *mergeSortResume(ResumeNode *head);

JobDescriptionNode *mergeTwoSortedJob(JobDescriptionNode *left, JobDescriptionNode *right);
JobDescriptionNode *mergeSortJob(JobDescriptionNode *head);

SkillNode *partition(SkillNode *head, SkillNode *end, SkillNode **newHead, SkillNode **newEnd);
SkillNode *quickSort(SkillNode *head, SkillNode *end);
SkillNode *getTail(SkillNode *tail);