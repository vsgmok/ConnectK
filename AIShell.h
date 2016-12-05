#ifndef AISHELL_H
#define AISHELL_H

#pragma once
#include "Move.h"
#include <vector>
#include <algorithm>
#include <chrono>
#include <sys/time.h>
#include <map>



// A new AIShell will be created for every move request.
class AIShell{

public:
	//these represent the values for each piece type.
	static const int AI_PIECE=1;
	static const int HUMAN_PIECE = -1;
	static const int NO_PIECE=0;


private:
	//Do not alter the values of numRows or numcols.
	//they are used for deallocating the gameState variable.
	int numRows; //the total number of rows in the game state.
	int numCols; //the total number of columns in the game state.
	int **gameState; //a pointer to a two-dimensional array representing the game state.
	bool gravityOn; //this will be true if gravity is turned on. It will be false if gravity is turned off.
	Move lastMove; //this is the move made last by your opponent. If your opponent has not made a move yet (you move first) then this move will hold the value (-1, -1) instead.
    
    Move move; //Updates thorughout the process of min_max
    std::vector<std::pair<Move,int> > best_moves;
    
    
    int min_max(int** state, bool max, int depth, int cutoff, int alpha, int beta, Move previousMove);
    
    bool terminalState(Move m);
    int heuristic(int** state, Move previousMove);
    int rightDiagPoints(int** state, Move m, bool defense);
    int leftDiagPoints(int** state, Move m, bool defense);
    int downRightDiagPoints(int** state, Move m, bool defense);
    int downLeftDiagPoints(int** state, Move m, bool defense);
    int upPoints(int** state, Move m, bool defense);
    int downPoints(int** state, Move m, bool defense);
    int leftPoints(int** state, Move m, bool defense);
    int rightPoints(int** state, Move m, bool defense);
    Move checkUp(int ** state, Move m, bool defense);
    Move checkDown(int ** state, Move m, bool defense);
    Move checkLeft(int ** state, Move m, bool defense);
    Move checkRight(int ** state, Move m, bool defense);
    Move checkRightDiag(int ** state, Move m, bool defense);
    Move checkLeftDiag(int ** state, Move m, bool defense);
    Move checkDownLeftDiag(int ** state, Move m, bool defense);
    Move checkDownRightDiag(int ** state, Move m, bool defense);

    Move check_empty(int** state);
    Move find_move(int** state, std::string dir);
    
    std::vector<Move> availableMoves(int** state);
    
    std::vector<Move> horizontalConsecutives;
    
    
    


public:
	int deadline; //this is how many milliseconds the AI has to make move.
	int k;        // k is the number of pieces a player must get in a row/column/diagonal to win the game. IE in connect 4, this variable would be 4

	AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move move);
	~AIShell();
	Move makeMove();
};

#endif //AISHELL_H
