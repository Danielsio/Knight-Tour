#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Q1Q2.h"
#include "structs.h"
#include "Q3Q4Q5Q6Q7.h"
#include "alloc.h"

/* check if list is empty */
bool isEmptyList(chessPosList* lst)
{
	if (lst->head == NULL)
		return true;
	else
		return false;
}
/* seif 1*/
chessPosArray*** validKnightMoves()
{
	chessPosArray*** KnightMoves;
	int i, j;

	KnightMoves = (chessPosArray***)malloc(SIZE * sizeof(chessPosArray**));
	if (KnightMoves == NULL)
		exit(1);

	for (i = 0; i < SIZE; i++)
	{
		KnightMoves[i] = (chessPosArray**)malloc(SIZE * sizeof(chessPosArray*));
		if (KnightMoves[i] == NULL)
			exit(1);
	}

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			KnightMoves[i][j] = CheckCellMoves(i, j);
		}
	}

	return KnightMoves;
}
/* second function for seif 1*/
chessPosArray* CheckCellMoves(int row, int col)
{
	chessPosArray* res = (chessPosArray*)malloc(sizeof(chessPosArray));
	if (res == NULL)
		exit(1);

	res->positions = (chessPos*)malloc(MAXPOS * sizeof(chessPos));
	int count = 0;

	if (res->positions == NULL)
		exit(1);

	if (row - 2 >= 0 && col - 1 >= 0) /*Up Left*/
	{
		res->positions[count][0] = (row - 2) + 'A';
		res->positions[count][1] = (col - 1) + '1';
		count++;
	}
	if (row - 2 >= 0 && col + 1 < SIZE) /*Up Right*/
	{
		res->positions[count][0] = (row - 2) + 'A';
		res->positions[count][1] = (col + 1) + '1';
		count++;
	}
	if (col + 2 < SIZE && row - 1 >= 0) /*Right Up*/
	{
		res->positions[count][0] = (row - 1) + 'A';
		res->positions[count][1] = (col + 2) + '1';
		count++;
	}
	if (col + 2 < SIZE && row + 1 < SIZE) /*Right Down*/
	{
		res->positions[count][0] = (row + 1) + 'A';
		res->positions[count][1] = (col + 2) + '1';
		count++;
	}
	if (row + 2 < SIZE && col - 1 >= 0) /*Down Left*/
	{
		res->positions[count][0] = (row + 2) + 'A';
		res->positions[count][1] = (col - 1) + '1';
		count++;
	}
	if (row + 2 < SIZE && col + 1 < SIZE) /*Down Right*/
	{
		res->positions[count][0] = (row + 2) + 'A';
		res->positions[count][1] = (col + 1) + '1';
		count++;
	}
	if (col - 2 >= 0 && row - 1 >= 0) /*Left Up*/
	{
		res->positions[count][0] = (row - 1) + 'A';
		res->positions[count][1] = (col - 2) + '1';
		count++;
	}
	if (col - 2 >= 0 && row + 1 < SIZE) /*Left Down*/
	{
		res->positions[count][0] = (row + 1) + 'A';
		res->positions[count][1] = (col - 2) + '1';
		count++;
	}

	res->size = count;
	res->positions = (chessPos*)realloc(res->positions, count * sizeof(chessPos));
	allocCheck(&res->positions);

	return res;
}
/* this function intialize a two dimensional array of boolians */
void initTable(bool path[][SIZE], int rows, int cols)
{
	int i, j;
	for (i = 0; i < rows; i++)
		for (j = 0; j < cols; j++)
			path[i][j] = false;
}
/* seif 2 */
void display(chessPosList* lst)
{
	if (isEmptyList(lst)) /* empty list check */
		return;

	checkDoubleOccurrences(lst); /* checking double occurrences */
	printChessBoard(lst); /* print the chess board */
}
/* this function deletes a node from a list */
void deleteNodeFromChessList(chessPosList* lst, chessPosCell* node)
{
	chessPosCell* curr, * prev;

	if (lst->head == lst->tail) /*  == node)*/
	{
		lst->head = lst->tail = NULL;
		free(node);
	}
	else if (lst->head == node) /* delete the head node */
	{
		lst->head = lst->head->next;
		free(node);
	}
	else if (lst->tail == node) /* delete tail node */
	{
		curr = lst->head;
		while (curr->next)
		{
			if (curr->next->next == NULL) /* in other words Ycurr is one behind the last node we need to delete */
			{
				lst->tail = curr;
				break;
			}
			curr = curr->next;
		}
		lst->tail->next = NULL;
		free(node);
	}
	else /* middle */
	{
		curr = lst->head->next;
		prev = lst->head;
		while (curr)
		{
			if (curr == node)
			{
				prev->next = curr->next;
				free(node);
				break;
			}
			curr = curr->next;
			prev = prev->next;
		}
	}
}
/* this function check if there is same positions in the list */
void checkDoubleOccurrences(chessPosList* lst)
{
	bool posTable[SIZE][SIZE];
	initTable(posTable, SIZE, SIZE);

	if (isEmptyList(lst)) /* check if list is empty */
		return;
	else if (lst->head->next == NULL) /* if list as only one node there is nothing to check */
		return;

	chessPosCell* curr1; /* set up a pointer that will help us run the entire list and check positions */
	curr1 = lst->head;

	while (curr1->next) /* loop to check if there is same positions at the list */
	{
		if (checkExistPosition(posTable, SIZE, SIZE, curr1->position) == true) /* this function checks if the position at the list has double occurrences */
		{
			deleteNodeFromChessList(lst, curr1); /* delete the extra node */
		}
		curr1 = curr1->next;
	}
	/* eficient ~ (N) */;
}
/* this function prints the chess board */
void printChessBoard(chessPosList* lst)
{
	int chessBoard[SIZE][SIZE];
	initBoard(chessBoard, SIZE, SIZE, lst);
	int col, row, count = 0;

	printf("    ");
	for (col = 1; col <= SIZE; col++)
		printf(" %d     ", col);

	printf("\n");
	printf("   ");
	printLine(35, '_');
	char rowCh = 'A';
	for (row = 0; row < SIZE; row++)
	{
		printf("%c |", rowCh);
		rowCh++;
		for (col = 0; col < SIZE; col++)
		{
			printf("  %2d  |", chessBoard[row][col]);
		}
		printf("\n");
	}
	printf("   ");
	printLine(35, '_');
}
/* print a line of sort */
void printLine(int count, char ch)
{
	int i;

	for (i = 0; i < count; i++)
		printf("%c", ch);

	printf("\n");
}
/* this function sets the display board*/
void initBoard(int chessBoard[][SIZE], int rows, int cols, chessPosList* lst)
{
	int i, j, count = 1;
	chessPosCell* curr;
	curr = lst->head;
	while (curr)
	{
		i = curr->position[0] - 'A';
		j = curr->position[1] - '1';
		chessBoard[i][j] = count;
		count++;
		curr = curr->next;
	}
}
/* check if a chesspos was already been touched  */
bool checkExistPosition(bool posTable[][SIZE], int rows, int cols, chessPos position)
{
	int i, j;
	bool res;

	i = position[0] - 'A';
	j = position[1] - '1';

	if (posTable[i][j] == true)
		res = true;
	else
	{
		posTable[i][j] = true;
		res = false;
	}
	return res;
}