#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Q1Q2.h"
#include "structs.h"
#include "Q3Q4Q5Q6Q7.h"
#include "alloc.h"

/* this function make a list empty */
void makeEmptyList(chessPosList* lst)
{
	lst->head = NULL;
	lst->tail = NULL;
}
/* seif 4*/
chessPosList* findKnightPathCoveringAllBoard(pathTree* pathTree)
{
	chessPosList* lst = (chessPosList*)malloc(1 * sizeof(chessPosList));
	allocCheck(&lst);
	makeEmptyList(lst);

	int level = 0;
	bool pathFound = false;
	findKnightPathCoveringAllBoardHelper(pathTree->root->next_possible_positions, lst, &level, &pathFound);

	if (lst->head)
	{
		insertDataToStartList(lst, pathTree->root->position);
		return lst;
	}
	else
	{
		return NULL;
	}
}
/* seif 4 helper */
void findKnightPathCoveringAllBoardHelper(treeNodeListCell* position, chessPosList* lst, int* level, bool* pathFound)
{
	if (position == NULL)
	{
		*level -= 1;
		return;
	}

	else
	{
		*level += 1;
		findKnightPathCoveringAllBoardHelper(position->node->next_possible_positions, lst, level, pathFound);
		if (*level >= (SIZE * SIZE) - 2 || *pathFound == true)
		{
			*pathFound = true;
			insertDataToStartList(lst, position->node->position);
			return;
		}

		if (position->next)
		{
			findKnightPathCoveringAllBoardHelper(position->next, lst, level, pathFound);
			*level += 1;
		}
		*level -= 1;
	}

}
/* create a list node of knight location */
chessPosCell* createNewListNode(chessPos position, chessPosCell* next)
{
	chessPosCell* res;
	res = (chessPosCell*)malloc(sizeof(chessPosCell));
	allocCheck(&res);
	res->position[0] = position[0];
	res->position[1] = position[1];
	res->next = next;
	return res;
}
/* insert data to start list */
void insertDataToStartList(chessPosList* lst, chessPos position)
{
	chessPosCell* newHaed;
	newHaed = createNewListNode(position, NULL);
	insertNodeToStartList1(lst, newHaed);
}
/* insert a list node to start of a list */
void insertNodeToStartList1(chessPosList* lst, chessPosCell* head)
{
	if (isEmptyList(lst))
	{
		head->next = NULL;
		lst->head = lst->tail = head;
	}
	else
	{
		head->next = lst->head;
		lst->head = head;
	}
}
/* seif 3 */
pathTree findAllPossibleKnightPaths(chessPos startingPosition)
{
	bool path[SIZE][SIZE];
	initTable(path, SIZE, SIZE);
	chessPosArray*** mat = validKnightMoves();

	pathTree tr;

	tr.root = findAllPossibleKnightPathsHelper(startingPosition, mat, path);

	freeMat(mat);
	return tr;
}
/* seif 3 helper */
treeNode* findAllPossibleKnightPathsHelper(chessPos rootdata, chessPosArray*** board, bool path[][SIZE])
{
	treeNode* root, * temp;
	unsigned int i, k, l;

	if (checkExistPos(path, rootdata))
		return NULL;
	else
	{
		root = createNewTreeNode(rootdata);
		k = rootdata[0] - 'A';
		l = rootdata[1] - '1';
		path[k][l] = true;
		for (i = 0; i < board[k][l]->size; i++)
		{
			if (!checkExistPos(path, board[k][l]->positions[i]))
			{
				temp = findAllPossibleKnightPathsHelper(board[k][l]->positions[i], board, path);
				insertDNodeToStartList(root, temp);
			}
		}
		path[k][l] = false;
	}
	return root;
}
/* insert a tree list node to start of tree-list nodes */
void insertDNodeToStartList(treeNode* head, treeNode* Node)
{
	treeNodeListCell* NewNode;
	NewNode = CreateTreeNodeListCell(Node);
	insertNodeToStartList(head, NewNode);
}
/* insert a tree list node to start of tree-list nodes */
void insertNodeToStartList(treeNode* head, treeNodeListCell* NewNode)
{
	if (isEmptyTreeList(head->next_possible_positions) == true)
	{
		head->next_possible_positions = NewNode;
		NewNode->next = NULL;
	}
	else
	{
		NewNode->next = head->next_possible_positions;
		head->next_possible_positions = NewNode;
	}

}
/* create a tree node */
treeNodeListCell* CreateTreeNodeListCell(treeNode* node)
{
	treeNodeListCell* NewCell = (treeNodeListCell*)malloc(sizeof(treeNodeListCell));
	if (NewCell == NULL)
		exit(1);

	NewCell->node = node;
	NewCell->next = NULL;

	return NewCell;
}
/* check if tree list is empty */
bool isEmptyTreeList(treeNodeListCell* head)
{
	if (head == NULL)
		return true;
	else
		return false;
}
/* check if a chesspos was already been touched  */
bool checkExistPos(bool path[][SIZE], chessPos position)
{
	int i, j;

	i = position[0] - 'A';
	j = position[1] - '1';

	if (path[i][j] == true)
		return true;
	else
		return false;
}
/* create New Tree Node */
treeNode* createNewTreeNode(chessPos position)
{
	treeNode* tNode;

	tNode = (treeNode*)malloc(sizeof(treeNode));
	if (tNode == NULL)
		exit(6);

	tNode->position[0] = position[0];
	tNode->position[1] = position[1];

	tNode->next_possible_positions = NULL;

	return tNode;
}
/* seif 5 */
void saveListToBinFile(char* file_name, chessPosList* pos_list)
{
	if (isEmptyList(pos_list))
	{
		printf("List is Empty!..");
		return;
	}

	int i = 0;
	short int size = sizeOfList(pos_list); /* realloc */

	FILE* fpw = fopen(file_name, "wb");
	fileOpenCheck(fpw);

	fwrite(&size, sizeof(short int), 1, fpw); /* write the size of the list */

	int size_of_array = (size * 6 / 8) + 1;

	BYTE* bitsArr = (BYTE*)malloc(size_of_array * sizeof(BYTE));
	allocCheck(&bitsArr);
	BYTE tmp;
	BYTE saver1, saver2;

	chessPosCell* curr = pos_list->head;

	while (curr) /* we will divide it to 3 diffrent options by % 3*/
	{
		/* we have a cycle of 3 steps */

		/* first step */
		tmp = 0x00;
		saver1 = 0x00;
		tmp |= (curr->position[0] - 'A') << 5; /* this gives us 1110_0000 */
		tmp |= (curr->position[1] - '1') << 2; /* this gives us 0001_1100 */
		if (curr->next)
		{
			saver1 = 0x00;
			saver1 |= (curr->next->position[0] - 'A') << 7; /* this give us 1000_0000 we want that for later */
			tmp |= (curr->next->position[0] - 'A') >> 1; /* this give us 0000_0011 to add to the tmp BYTE */

			bitsArr[i] = tmp; /* insert to array */
			i++; /* array index value update */
			curr = curr->next; /* moving on to the next location */
		}
		else
		{
			bitsArr[i] = tmp; /* insert to array */
			i++; /* array index value update */
			break;
		}

		/* second step */

		tmp = 0x00;
		saver2 = 0x00;
		tmp |= saver1; /* which is 1000_0000 */
		tmp |= (curr->position[1] - '1') << 4; /* 0111_0000*/
		if (curr->next)
		{
			tmp |= (curr->next->position[0] - 'A') << 1; /* is 0000_1110 */
			saver2 = 0x00;
			saver2 |= (curr->next->position[1] - '1') << 6; /* which is 1100_0000*/
			tmp |= (curr->next->position[1] - '1') >> 2; /* which gives us this 0000_0001*/

			bitsArr[i] = tmp; /* insert to array*/
			i++;
			if (curr->next->next)
			{
				curr = curr->next->next;
			}
			else
			{
				tmp = 0x00;
				tmp |= saver2;
				bitsArr[i] = tmp;
				i++;
				break; /* since there isn't anymore locations we break and skip the third step */

				/* this case have 6 bits off at the end */
			}
		}
		else
		{
			bitsArr[i] = tmp;
			i++;
			break;

			/* this case have 4 bits off at the end */
		}

		/* third step */

		tmp = 0x00;
		tmp |= saver2;
		tmp |= (curr->position[0] - 'A') << 3; /* this gives us 0011_1000*/
		tmp |= (curr->position[1] - '1'); /* 0000_0111*/

		bitsArr[i] = tmp;
		i++;
		if (curr->next)
		{
			curr = curr->next;
		}
		else
		{
			break;
		}

		/* this case have NONE bits off at the end */

	}
	/* after we got all positions */

	fwrite(bitsArr, sizeof(BYTE), size_of_array, fpw); /* size_of_array == the value of i */
	free(bitsArr);
	fclose(fpw);
}
/* calculate size of list nodes in list */
short int sizeOfList(chessPosList* pos_list)
{
	if (pos_list->head == NULL)
		return 0;

	chessPosCell* curr = pos_list->head;
	short int counter = 0;
	while (curr)
	{
		counter++;
		curr = curr->next;
	}
	return counter;

}
/* file openning seccess check */
void fileOpenCheck(FILE* fp)
{
	if (fp == NULL)
	{
		printf("Failed opening the file. \n");
		exit(1);
	}
}
/* seif 6 */
int checkAndDisplayPathFromFile(char* fileName)
{
	FILE* data;
	chessPosList* lst = (chessPosList*)malloc(sizeof(chessPosList));
	makeEmptyList(lst);

	data = fopen(fileName, "rb");
	if (data == NULL) /*case of file doesn't exist */
	{
		return -1;
	}

	makeListFromFile(data, lst); /* read and create the list from the file */
	fclose(data);

	if (!CheckValidKnightPath(lst))
		return 1;

	display(lst); /* if list is valid we call display func */

	int size = sizeOfList(lst);

	if (size == SIZE * SIZE)
	{
		freeList(lst);
		return 2;
	}
	else
	{
		freeList(lst);
		return 3;
	}

}
/*function that checks if the path in the list is valid*/
bool CheckValidKnightPath(chessPosList* lst)
{
	chessPosCell* curr = lst->head;
	bool validpath = true;

	if (isEmptyList(lst))
		return false;

	while (curr->next != NULL && validpath == true)
	{
		if (!CheckValidMove(curr, curr->next))
		{
			validpath = false;
			break;
		}
		curr = curr->next;
	}
	return validpath;
}
/* function that gets 2 cells and check if the move from the current pos to the next pos is valid*/
bool CheckValidMove(chessPosCell* curr, chessPosCell* nextPos)
{
	int rowCurr, colCurr, rowNext, colNext;

	rowCurr = curr->position[0] - 'A';
	colCurr = curr->position[1] - '1';
	rowNext = nextPos->position[0] - 'A';
	colNext = nextPos->position[1] - '1';

	/*direction: clockwise*/
	if (rowCurr - 2 == rowNext && colCurr + 1 == colNext) /*Up Right*/
		return true;
	else if (colCurr + 2 == colNext && rowCurr - 1 == rowNext) /*Right Up*/
		return true;
	else if (colCurr + 2 == colNext && rowCurr + 1 == rowNext) /*Right Down*/
		return true;
	else if (rowCurr + 2 == rowNext && colCurr + 1 == colNext) /*Down Right*/
		return true;
	else if (rowCurr + 2 == rowNext && colCurr - 1 == colNext) /*Down Left*/
		return true;
	else if (colCurr - 2 == colNext && rowCurr + 1 == rowNext) /*Left Down*/
		return true;
	else if (colCurr - 2 == colNext && rowCurr - 1 == rowNext) /*Left Up*/
		return true;
	else if (rowCurr - 2 == rowNext && colCurr - 1 == colNext) /*Up Left*/
		return true;
	else
		return false;
}
/* create a list from binary file */
void makeListFromFile(FILE* data, chessPosList* lst)
{
	int i, numBytes;
	BYTE tempData[3];	/* we read 3 bytes each time and we convert them to 4 locations */

	/* the following is equivalent to fileSize() */
	fseek(data, 0, SEEK_END);
	numBytes = (int)ftell(data) - 2;
	fseek(data, 0, SEEK_SET);   //or, to rewind();

	fseek(data, 2, SEEK_SET); /* skip the short int value of size of locations */
	for (i = 0; i < numBytes; i += 3)   //we read 3 bytes at a time
	{
		if (numBytes % 3 == 0)
		{
			fread(tempData, sizeof(BYTE), 3, data);
			getDecodedData(tempData, lst, 3);
		}
		else if (numBytes % 3 == 1)
		{
			if (numBytes - 1 == i) /* last byte to be read from file */
			{
				fread(tempData, sizeof(BYTE), 1, data);
				getDecodedData(tempData, lst, 1);
			}
			else
			{
				fread(tempData, sizeof(BYTE), 3, data);
				getDecodedData(tempData, lst, 3);
			}
		}
		else if (numBytes % 3 == 2)
		{
			if (numBytes - 2 == i) /* last two bytes to be read from file */
			{
				fread(tempData, sizeof(BYTE), 2, data);
				getDecodedData(tempData, lst, 2);
			}
			else
			{
				fread(tempData, sizeof(BYTE), 3, data);
				getDecodedData(tempData, lst, 3);
			}
		}
	}
}
/* translate binary file to chesspos */
void getDecodedData(BYTE* data, chessPosList* lst, int size)
{
	BYTE mask1 = 0xE0;	/* 11100000  byte 1 (most significant byte - the left most one) */
	BYTE mask2 = 0x1C;	/* 00011100  byte 1 */
	BYTE mask3a = 0x03, mask3b = 0x80;	/* 0000001110000000 bytes 1+2 */
	BYTE mask4 = 0x70;	/* 01110000  byte 2 */
	BYTE mask5 = 0x0E;	/* 00001110  byte 2 */
	BYTE mask6a = 0x01, mask6b = 0xC0;	/* 0000000111000000 bytes 2+3 */
	BYTE mask7 = 0x38;	/* 00111000  byte 3 (least significant byte) */
	BYTE mask8 = 0x07;	/* 00000111	byte 3  */

	chessPos pos1, pos2, pos3, pos4;
	if (size == 3)
	{
		pos1[0] = ((data[0] & mask1) >> 5) + 'A';
		pos1[1] = ((data[0] & mask2) >> 2) + '1';
		pos2[0] = (((data[0] & mask3a) << 1) | ((data[1] & mask3b) >> 7)) + 'A';
		pos2[1] = ((data[1] & mask4) >> 4) + '1';
		pos3[0] = ((data[1] & mask5) >> 1) + 'A';
		pos3[1] = (((data[1] & mask6a) << 2) | ((data[2] & mask6b) >> 6)) + '1';
		pos4[0] = ((data[2] & mask7) >> 3) + 'A';
		pos4[1] = ((data[2] & mask8)) + '1';

		insertNodeToEndList(lst, createNewListNode(pos1, NULL));
		insertNodeToEndList(lst, createNewListNode(pos2, NULL));
		insertNodeToEndList(lst, createNewListNode(pos3, NULL));
		insertNodeToEndList(lst, createNewListNode(pos4, NULL));
	}
	else if (size == 2)
	{
		pos1[0] = ((data[0] & mask1) >> 5) + 'A';
		pos1[1] = ((data[0] & mask2) >> 2) + '1';
		pos2[0] = (((data[0] & mask3a) << 1) | ((data[1] & mask3b) >> 7)) + 'A';
		pos2[1] = ((data[1] & mask4) >> 4) + '1';

		insertNodeToEndList(lst, createNewListNode(pos1, NULL));
		insertNodeToEndList(lst, createNewListNode(pos2, NULL));
	}
	else if (size == 1)
	{
		pos1[0] = ((data[0] & mask1) >> 5) + 'A';
		pos1[1] = ((data[0] & mask2) >> 2) + '1';

		insertNodeToEndList(lst, createNewListNode(pos1, NULL));
	}

}
/* insert Node To End List */
void insertNodeToEndList(chessPosList* lst, chessPosCell* tail)
{
	if (isEmptyList(lst))
	{
		lst->head = lst->tail = tail;
	}
	else
	{
		lst->tail->next = tail;
		lst->tail = tail;
	}
	tail->next = NULL;
}
/* free the  list */
void freeList(chessPosList* lst)
{
	chessPosCell* curr, * saver;
	curr = lst->head;
	while (curr)
	{
		saver = curr->next;
		free(curr);
		curr = saver;
	}
	free(lst);
}
/* seif 7  the menu */
void PrintMenu()
{
	printf("\n1. Enter a knight's starting position\n");
	printf("2. Create all possible knight paths\n");
	printf("3. Find a knight path covering all board\n");
	printf("4. Save knight path covering all board to file\n");
	printf("5. Load and display path from file\n");
	printf("6. Exit\n");
}
/* check if chessPos is valid pos */
bool isValidPos(chessPos pos)
{
	return (pos[0] >= 'A' && pos[0] <= 'H' && pos[1] >= '1' && pos[1] <= '8') ? true : false;
}
/*free tree*/
void FreeTree(pathTree* tr)
{
	if (tr->root == NULL)
		return;

	FreeTreeHelper(tr->root);
}

void FreeTreeHelper(treeNode* root)
{
	if (root == NULL)
		return;

	FreetreeNodeListCell(root->next_possible_positions);
	free(root);
}

void FreetreeNodeListCell(treeNodeListCell* headnode)
{
	if (headnode == NULL)
		return;

	treeNodeListCell* saver, * curr = headnode;

	while (curr != NULL)
	{
		saver = curr->next;
		FreeTreeHelper(curr->node);
		free(curr);
		curr = saver;
	}
}

void GameProcess()
{
	int checkInput;
	chessPos startingPos;
	pathTree tr;
	tr.root = NULL;
	chessPosList* lst = NULL;
	int choice;
	bool exit = false;

	while (!exit)
	{
		PrintMenu();
		checkInput = scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			printf("Enter your position: \n"); /* messege to user */
			while ((getchar()) != '\n'); /* buffer clean */
			checkInput = scanf("%c%c", &startingPos[0], &startingPos[1]); /* scan chessPos */

			if (isValidPos(startingPos))
				break;
			else
			{
				printf("Invalid position!! please try again. \n\n");
				break;
			}

		case 2:
			OptionTwo(startingPos, &tr);
			break;
		case 3:
			OptionThree(startingPos, &tr, lst);
			break;
		case 4:
			OptionFour(startingPos, &tr, lst);
			break;
		case 5:
			OptionFive(startingPos);
			break;
		case 6:
			if (lst)
				freeList(lst);
			if (tr.root)
				FreeTree(&tr);
			exit = true;
			break;
		default:
			printf("Invalid choice please try again. \n");
			break;
		}
	}
}

bool checkListIsEmpty(chessPosList* lst)
{
	if (lst == NULL)
		return true;
	else
		return false;
}

char* getName()
{
	int checkInput, logSize = 0, phsSize = INIT_SIZE;
	char* NewFileName = (char*)malloc(INIT_SIZE * sizeof(char));
	allocCheck(&NewFileName);
	char ch;

	while ((getchar()) != '\n');

	checkInput = scanf("%c", &ch);
	while (ch != '\n')
	{
		if (phsSize == logSize)
		{
			phsSize *= 2;
			NewFileName = (char*)realloc(NewFileName, phsSize * sizeof(char));
			allocCheck(&NewFileName);
		}

		NewFileName[logSize] = ch;
		logSize++;
		checkInput = scanf("%c", &ch);
	}
	NewFileName[logSize] = '\0';

	return NewFileName;
}

void freeMat(chessPosArray*** mat)
{
	int i, j;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			free(mat[i][j]->positions);
			free(mat[i][j]);
		}
		free(mat[i]);
	}
	free(mat);
}

/*Menu Options, covering all the cases, every case in a function*/
void OptionTwo(chessPos startingPos, pathTree* tr)
{
	if (isValidPos(startingPos))
	{
		if (tr->root)
		{
			FreeTree(tr);
		}

		*tr = findAllPossibleKnightPaths(startingPos);
		printf("\ntree has been built properly!! \n");
	}
	else
	{
		printf("\nPlease insert a starting position.\n\n");
	}
}

void OptionThree(chessPos startingPos, pathTree* tr, chessPosList* lst)
{
	if (isValidPos(startingPos))  /* starting pos is valid */
	{
		if (tr->root) /* tree is built */
		{
			if (!checkListIsEmpty(lst))
			{
				freeList(lst);
			}
			lst = findKnightPathCoveringAllBoard(tr);
			if (lst == NULL)
			{
				printf("PATH NOT FOUND ! \n");
				return;
			}
			printf("\nList has been built properly!! \n\n");
		}
		else
		{
			*tr = findAllPossibleKnightPaths(startingPos); /* if tree isnt built already we build it now */
			if (!checkListIsEmpty(lst))
			{
				freeList(lst);
			}
			lst = findKnightPathCoveringAllBoard(tr); /* we search for a path to cover all board */
			if (lst == NULL)
			{
				printf("\nPATH NOT FOUND ! \n");
				return;
			}
			printf("\nList has been built properly!! \n\n");
		}
	}
	else
	{
		printf("\nPlease insert a starting position.\n\n");
		return;
	}
}

void OptionFour(chessPos startingPos, pathTree* tr, chessPosList* lst)
{
	char* fileName;

	if (isValidPos(startingPos))
	{
		printf("Please enter your file-name: \n\n");

		fileName = getName(); /* get the name */

		if (!checkListIsEmpty(lst))
		{
			saveListToBinFile(fileName, lst);
			free(fileName);
			printf("\nDONE\n");
		}
		else
		{
			if (tr->root) /* tree is built */
			{
				lst = findKnightPathCoveringAllBoard(tr); /* get the list */
				if (lst == NULL) /* check seccess */
				{
					printf("PATH NOT FOUND ! \n");
					return;
				}

				saveListToBinFile(fileName, lst);
				free(fileName);
				printf("\nDONE\n");
			}
			else
			{
				*tr = findAllPossibleKnightPaths(startingPos); /* if tree isnt built already we build it now */
				lst = findKnightPathCoveringAllBoard(tr); /* we search for a path to cover all board */
				if (lst == NULL)
				{
					printf("PATH NOT FOUND ! \n");
					return;
				}
				else
				{
					saveListToBinFile(fileName, lst);
					free(fileName);
					printf("\nDONE\n");
				}
			}
		}
	}
	else
		printf("\nPlease insert a starting position.\n\n");
}

void OptionFive(chessPos startingPos)
{
	char* fileName;
	int checkInput;

	printf("\nPlease enter your file-name: \n\n");
	fileName = getName();

	checkInput = checkAndDisplayPathFromFile(fileName);
	if (checkInput == -1)
	{
		printf("\nFile isnt exist!. \n");
		return;
	}
	else if (checkInput == 1)
	{
		printf("\nList is illegal!. \n");
	}
	else if (checkInput == 2)
	{
		printf("\nPath is covering all the board !! \n");
	}
	else if (checkInput == 3)
	{
		printf("\nPath is not covering all the board !! \n");
	}
}
/* print list data */
void printList(chessPosList* lst)
{
	if (isEmptyList(lst))
	{
		return;
	}
	chessPosCell* curr;
	curr = lst->head;
	while (curr != NULL)
	{
		printf("%c%c ", curr->position[0], curr->position[1]);
		curr = curr->next;
	}
	printf("\n");
}