#ifndef STRUCTS_H
#define STRUCTS_H

#define SIZE 5
#define MAXPOS 8
#define INIT_SIZE 1

typedef unsigned char BYTE;

typedef char chessPos[2];

typedef struct _chessPosArray
{
	unsigned int size;
	chessPos* positions;
} chessPosArray;

typedef struct _treeNodeListCell treeNodeListCell;

typedef struct _treeNode
{
	chessPos position;
	treeNodeListCell* next_possible_positions;
} treeNode;

typedef struct _treeNodeListCell
{
	treeNode* node;
	struct _treeNodeListCell* next;
} treeNodeListCell;

typedef struct _pathTree
{
	treeNode* root;
} pathTree;

typedef struct _chessPosCell
{
	chessPos position;
	struct _chessPosCell* next;
} chessPosCell;

typedef struct _chessPosList
{
	chessPosCell* head;
	chessPosCell* tail;
} chessPosList;

#endif