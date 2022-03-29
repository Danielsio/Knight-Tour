#ifndef Q3Q4Q5Q6Q7_H
#define Q3Q4Q5Q6Q7_H
#include <stdio.h>
#include "structs.h"

chessPosList* findKnightPathCoveringAllBoard(pathTree* pathTree);
void findKnightPathCoveringAllBoardHelper(treeNodeListCell* position, chessPosList* lst, int* level, bool* pathFound);
chessPosCell* createNewListNode(chessPos position, chessPosCell* next);
void insertDataToStartList(chessPosList* lst, chessPos position);
void insertNodeToStartList1(chessPosList* lst, chessPosCell* head);
pathTree findAllPossibleKnightPaths(chessPos startingPosition);
treeNode* findAllPossibleKnightPathsHelper(chessPos rootdata, chessPosArray*** board, bool path[][SIZE]);
treeNodeListCell* CreateTreeNodeListCell(treeNode* node);
void insertDNodeToStartList(treeNode* head, treeNode* Node);
void insertNodeToStartList(treeNode* head, treeNodeListCell* NewNode);
treeNode* createNewTreeNode(chessPos position);
bool isEmptyTreeList(treeNodeListCell* head);
void saveListToBinFile(char* file_name, chessPosList* pos_list);
short int sizeOfList(chessPosList* pos_list);
void fileOpenCheck(FILE* fp);
int checkAndDisplayPathFromFile(char* fileName);
bool CheckValidKnightPath(chessPosList* lst);
bool CheckValidMove(chessPosCell* curr, chessPosCell* nextPos);
bool checkExistPos(bool path[][SIZE], chessPos position);
void makeListFromFile(FILE* data, chessPosList* lst);
void getDecodedData(BYTE* data, chessPosList* lst, int size);
void insertNodeToEndList(chessPosList* lst, chessPosCell* tail);
void freeList(chessPosList* lst);
void FreeTree(pathTree* tr);
void FreeTreeHelper(treeNode* root);
void FreetreeNodeListCell(treeNodeListCell* headnode);
void PrintMenu();
bool isValidPos(chessPos pos);
void GameProcess();
void OptionTwo(chessPos startingPos, pathTree* tr);
void OptionThree(chessPos startingPos, pathTree* tr, chessPosList* lst);
void OptionFour(chessPos startingPos, pathTree* tr, chessPosList* lst);
void OptionFive(chessPos startingPos);
bool checkListIsEmpty(chessPosList* lst);
char* getName();
void freeMat(chessPosArray*** mat);
void printList(chessPosList* lst);
void makeEmptyList(chessPosList* lst);
#endif