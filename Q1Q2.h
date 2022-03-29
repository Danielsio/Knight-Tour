#ifndef Q1Q2_H
#define Q1Q2_H

#include "structs.h"

chessPosArray*** validKnightMoves();
chessPosArray* CheckCellMoves(int row, int col);
void initTable(bool path[][SIZE], int rows, int cols);
bool isEmptyList(chessPosList* lst);
void display(chessPosList* lst);
void deleteNodeFromChessList(chessPosList* lst, chessPosCell* node);
void checkDoubleOccurrences(chessPosList* lst);
void printChessBoard(chessPosList* lst);
void printLine(int count, char ch);
void initBoard(int chessBoard[][SIZE], int rows, int cols, chessPosList* lst);
bool checkExistPosition(bool posTable[][SIZE], int rows, int cols, chessPos position);

#endif 