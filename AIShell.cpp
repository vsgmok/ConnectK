#include "AIShell.h"
#include <iostream>
#include <cstdlib>



AIShell::AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove)
{
    this->deadline=0;
    this->numRows=numRows;
    this->numCols=numCols;
    this->gravityOn=gravityOn;
    this->gameState=gameState;
    this->lastMove=lastMove;
    this->move = Move(0,0);
}


AIShell::~AIShell()
{
    
    //delete the gameState variable.
    for (int i =0; i<numCols; i++){
        delete [] gameState[i];
    }
    delete [] gameState;
    
}

Move AIShell::check_empty(int** state) {
    
    //Default:
    Move best_move = Move(-1000,-1000);

    std::vector<std::string> dirs;
    dirs.push_back("up");
    dirs.push_back("down");
    dirs.push_back("left");
    dirs.push_back("right");
    dirs.push_back("downleft");
    dirs.push_back("downright");
    dirs.push_back("upright");
    dirs.push_back("upleft");

    std::string best_dir;
    int best_val = 0;
    int val;
    
    int d[2];

    Move tempMove = lastMove;
    Move emptyMove(-1000, -1000);

    for (std::string dir : dirs) {
        if (dir == "up")
        {
            d[0] = 0;
            d[1] = 1;
        }
        else if (dir == "down")
        {
            d[0] = 0;
            d[1] = -1;
        }
        else if (dir == "left")
        {
            d[0] = -1;
            d[1] = 0;
        }
        else if (dir == "right")
        {
            d[0] = 1;
            d[1] = 0;
        }
        else if (dir == "upright")
        {
            d[0] = 1;
            d[1] = 1;
        }
        else if (dir == "downright")
        {
            d[0] = 1;
            d[1] = -1;
        }
        else if (dir == "downleft")
        {
            d[0] = -1;
            d[1] = -1;
        }
        else if (dir == "upleft")
        {
            d[0] = -1;
            d[1] = 1;
        }
        

        val = 0; //Number of human pieces
    
        tempMove = lastMove;

        for (int offset = 0; offset < k; ++offset)
        {
            
            tempMove.col = lastMove.col + offset*d[0];
            tempMove.row = lastMove.row + offset*d[1];
            //std::cout << "col: " << tempMove.col << ", row: " << tempMove.row << ", offset: " << offset << ", offset*d[0]: " << offset*d[0] << std::endl;

            if (tempMove.col >= numCols || \
                tempMove.row >= numRows || \
                tempMove.col < 0 || \
                tempMove.row < 0)
            {
                break; //out of bounds
            }

            if (state[tempMove.col][tempMove.row] == HUMAN_PIECE)
            {
                ++val;
            }
            else if (state[tempMove.col][tempMove.row] == NO_PIECE)
            {
                emptyMove = tempMove;
            }
            else if (state[tempMove.col][tempMove.row] = AI_PIECE)
            {
                break;
            }
        }
        //std::cout << "Checking direction " << dir << std::endl;
        //std::cout << "Value: " << val << std::endl << std::endl;

        if (val > best_val) 
        {
            if (emptyMove.col != -1000 && emptyMove.row != -1000)
            {
                best_move = emptyMove;
                best_val = val;
                best_dir = dir;
            }
        }
    }
    if (best_val >= 3)
    {
        return find_move(state, best_dir);
    }
    return Move(-1000,-1000);
}

Move AIShell::find_move(int** state, std::string dir)
{
    int thatdir[2];
    int oppdir[2];

    if (dir == "up")
    {
        thatdir[0] = 0;
        thatdir[1] = 1;
    }
    else if (dir == "down")
    {
        thatdir[0] = 0;
        thatdir[1] = -1;
    }
    else if (dir == "left")
    {
        thatdir[0] = -1;
        thatdir[1] = 0;
    }
    else if (dir == "right")
    {
        thatdir[0] = 1;
        thatdir[1] = 0;
    }
    else if (dir == "upright")
    {
        thatdir[0] = 1;
        thatdir[1] = 1;
    }
    else if (dir == "downright")
    {
        thatdir[0] = 1;
        thatdir[1] = -1;
    }
    else if (dir == "downleft")
    {
        thatdir[0] = -1;
        thatdir[1] = -1;
    }
    else if (dir == "upleft")
    {
        thatdir[0] = -1;
        thatdir[1] = 1;
    }

    oppdir[0] = thatdir[0]*-1;
    oppdir[1] = thatdir[-1]*-1;

    std::cout << dir << std::endl;

    Move tempMove = lastMove;
    for (int offset = 0; offset < k; ++offset)
    {
        tempMove.col = lastMove.col + offset*thatdir[0];
        tempMove.row = lastMove.row + offset*thatdir[1];

        std::cout << "col: " << tempMove.col << ", row: " << tempMove.row << ", offset: " << offset << ", offset*thatdir[0]: " << offset*thatdir[0] << std::endl;
        std::cout << std::endl;

        if (tempMove.col >= numCols || \
            tempMove.row >= numRows || \
            tempMove.col < 0 || \
            tempMove.row < 0)
        {
            break; //out of bounds
        }

        else if (state[tempMove.col][tempMove.row] == NO_PIECE)
        {
            return tempMove;
        }
        else if (state[tempMove.col][tempMove.row] == AI_PIECE)
        {
            break;
        }
    }

    for (int offset = 0; offset < k; ++offset)
    {
        tempMove.col = lastMove.col + offset*oppdir[0];
        tempMove.row = lastMove.row + offset*oppdir[1];

        std::cout << "col: " << tempMove.col << ", row: " << tempMove.row << ", offset: " << offset << ", offset*oppdir[0]: " << offset*oppdir[0] << std::endl;
        std::cout << std::endl;

        if (tempMove.col >= numCols || \
            tempMove.row >= numRows || \
            tempMove.col < 0 || \
            tempMove.row < 0)
        {
            break; //out of bounds
        }
        else if (state[tempMove.col][tempMove.row] == NO_PIECE)
        {
            return tempMove;
        }
        else if (state[tempMove.col][tempMove.row] == AI_PIECE)
        {
            break;
        }
    }
    return Move(-1000,-1000);
}

Move AIShell::makeMove(){

    
    best_moves.clear();
    std::vector<Move> moves = availableMoves(gameState);
    if(moves.size() == 1)
        return moves.back();
    
    std::chrono::steady_clock::time_point start_time =  std::chrono::steady_clock::now();
    
    if(lastMove.col != -1 && lastMove.row != -1)
    {
        
        Move try_empty = check_empty(gameState);
        if (try_empty.col != -1000 && try_empty.row != -1000)
        {
            return try_empty;
        }
        
        std::cout << "CHECKING ALL SIDES" << std::endl;
        
        std::pair<std::string,int> waze;
        
        int right = rightPoints(gameState,lastMove, true);
        int left = leftPoints(gameState,lastMove,true);
        int horizontal = right + left;
        waze.first = "horizontal";
        waze.second = horizontal;
        
        
        
        
        int up = upPoints(gameState,lastMove, true);
        int down = downPoints(gameState,lastMove,true);
        int vertical = up + down;
        if(vertical > waze.second)
        {
            waze.first = "vertical";
            waze.second = vertical;
        }
        int leftDiag = leftDiagPoints(gameState, lastMove, true);
        int rightDiag = rightDiagPoints(gameState, lastMove, true);
        int downLeftDiag = downLeftDiagPoints(gameState, lastMove, true);
        int downRightDiag = downRightDiagPoints(gameState, lastMove, true);
        
        int diagOne = leftDiag + downRightDiag;
        if(diagOne > waze.second)
        {
            waze.first = "diagOne";
            waze.second = diagOne;
        }
        int diagTwo = rightDiag + downLeftDiag;
        if(diagTwo > waze.second)
        {
            waze.first = "diagTwo";
            waze.second = diagTwo;
        }
        
        
        
        if(horizontal >= k-3 || vertical >= k-3 || diagOne >= k-3 || diagTwo >= k-3)
        {
            std::cout << "Last Move Has Consecutives: " <<waze.first  << std::endl;
            
            
            
            if(waze.first == "horizontal")
            {
                Move temp = lastMove;
                if(left < right)
                {
                    temp.col -=1;
                    while(temp.col >= 0)
                    {
                        if(gameState[temp.col][temp.row] == AI_PIECE)
                            break;
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                        {
                            
                            return Move(temp.col, temp.row);
                        }
                        temp.col -= 1;
                    }
                    
                    temp = lastMove;
                    temp.col +=1;
                    while(temp.col < numCols)
                    {
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                        {
        
                            return Move(temp.col, temp.row);
                        }
                        temp.col += 1;
                    }
                    
                }
                else
                {
                    temp.col +=1;
                    while(temp.col < numCols)
                    {
                        if(gameState[temp.col][temp.row] == AI_PIECE)
                            break;
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                        {
                            
                            return Move(temp.col, temp.row);
                        }
                        temp.col += 1;
                    }
                    temp = lastMove;
                    temp.col -=1;
                    while(temp.col >= 0)
                    {
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                        {
                            
                            return Move(temp.col, temp.row);
                        }
                        temp.col -= 1;
                    }
                    
                }
            }
            else if(waze.first == "vertical")
            {
                Move temp = lastMove;
                std::cout << "UP: " << up << std::endl;
                std::cout << "down: " << down << std::endl;
                if(up <= down)
                {
                    std::cout << "UP <= Down" << std::endl;
                    temp.row -=1;
                    while(temp.row >= 0)
                    {
                        if(gameState[temp.col][temp.row] == AI_PIECE)
                            break;
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                            return Move(temp.col, temp.row);
                        temp.row -= 1;
                    }
                    temp = lastMove;
                    temp.row += 1;
                    while(temp.row < numRows)
                    {
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                            return Move(temp.col, temp.row);
                        temp.row += 1;
                    }
                    
                }
                else
                {
                    std::cout << "UP > Down" << std::endl;
                    temp.row+=1;
                    while(temp.row < numRows)
                    {
                        if(gameState[temp.col][temp.row] == AI_PIECE)
                            break;
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                            return Move(temp.col, temp.row);
                        temp.row += 1;
                    }
                    temp = lastMove;
                    temp.row -= 1;
                    while(temp.row >= 0)
                    {
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                            return Move(temp.col, temp.row);
                        temp.row -= 1;
                    }
                    
                }
            }
            else if(waze.first == "diagOne")
            {
                Move temp = lastMove;
                if(leftDiag < downRightDiag)
                {
                    temp.row -= 1;
                    temp.col += 1;
                    while(temp.row >= 0 && temp.col < numCols)
                    {
                        if(gameState[temp.col][temp.row] == AI_PIECE)
                            break;
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                            return Move(temp.col, temp.row);
                        temp.row -= 1;
                        temp.col += 1;
                    }
                    temp = lastMove;
                    temp.row += 1;
                    temp.col -=1;
                    while(temp.row < numRows && temp.col >= 0)
                    {
                        if(gameState[temp.col][temp.row] == AI_PIECE)
                            break;
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                            return Move(temp.col, temp.row);
                        temp.row += 1;
                        temp.col -= 1;
                        
                    }

                    
                }
                else
                {
                    temp.row += 1;
                    temp.col -=1;
                    while(temp.row < numRows && temp.col >= 0)
                    {
                        if(gameState[temp.col][temp.row] == AI_PIECE)
                            break;
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                            return Move(temp.col, temp.row);
                        temp.row += 1;
                        temp.col -= 1;
                        
                    }
                    temp = lastMove;
                    temp.row -= 1;
                    temp.col += 1;
                    while(temp.row >= 0 && temp.col < numCols)
                    {
                        if(gameState[temp.col][temp.row] == AI_PIECE)
                            break;
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                            return Move(temp.col, temp.row);
                        temp.row -= 1;
                        temp.col += 1;
                    }
                }
            }
            else if(waze.first== "diagTwo")
            {
                Move temp = lastMove;
                if(rightDiag < downLeftDiag)
                {
                    temp.row -= 1;
                    temp.col -= 1;
                    while(temp.row >= 0 && temp.col >= 0)
                    {
                        if(gameState[temp.col][temp.row] == AI_PIECE)
                            break;
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                            return Move(temp.col, temp.row);
                        temp.row -= 1;
                        temp.col -= 1;
                    }
                    temp = lastMove;
                    temp.row += 1;
                    temp.col +=1;
                    while(temp.row < numRows && temp.col < numCols)
                    {
                        if(gameState[temp.col][temp.row] == AI_PIECE)
                            break;
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                            return Move(temp.col, temp.row);
                        temp.row += 1;
                        temp.col += 1;
                        
                    }
                    
                    
                }
                else
                {
                    temp.row += 1;
                    temp.col +=1;
                    while(temp.row < numRows && temp.col < numCols)
                    {
                        if(gameState[temp.col][temp.row] == AI_PIECE)
                            break;
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                            return Move(temp.col, temp.row);
                        temp.row += 1;
                        temp.col += 1;
                        
                    }
                    std::cout << "here" << std::endl;
                    temp = lastMove;
                    std::cout << "LAST MOVE: " << temp.col << " " << temp.row << std::endl;
                    temp.row -= 1;
                    temp.col -= 1;
                    while(temp.row >= 0 && temp.col >= 0)
                    {
                        std::cout << "while" << std::endl;
                        if(gameState[temp.col][temp.row] == AI_PIECE)
                            break;
                        if(gameState[temp.col][temp.row] == NO_PIECE)
                        {
                            std::cout << "if" << std::endl;
                            return Move(temp.col, temp.row);
                        }
                        temp.row -= 1;
                        temp.col -= 1;
                    }
                }

            }
            
            
        
        
            
            
        }
    
        
    }



    //Attempting IDS Sorting
    
    std::cout << "MAKING IDS MOVE" << std::endl;
    
    std::chrono::milliseconds ms(deadline-1000);
    
    int depth_limit = 1;
    
    Move best_move_so_far = Move(-1000,-1000);
        
    int ok = 0;
    while(true)
    {
        auto current_time =  std::chrono::steady_clock::now();
        auto total_time = current_time - start_time;
        
        if(std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count() >= ms.count())
            break;
        ok = min_max(gameState,true,0,depth_limit,-1000,1000,Move(0,0),start_time);
        if (ok != -10000)
        {
            best_move_so_far = best_moves.back().first;
        }
        depth_limit+=1;
    }
        
        //int value = 0;
        //value = std::max(min_max(gameState,true,0,-1000,1000,Move(0,0)),value);
    
    std::cout << "DEPTH: " << depth_limit << std::endl;
    return best_move_so_far;
    //return best_moves.back().first;
}

int AIShell::min_max(int** state, bool max, int depth,int cutoff, int alpha, int beta, Move previousMove, std::chrono::steady_clock::time_point start_time)
{
    int tempV;
    std::vector<Move> moves = availableMoves(state);
    
    //int alpha;
    //int beta;
    auto current_time = std::chrono::steady_clock::now();
    std::chrono::milliseconds ms(deadline-1000);
    auto total_time = current_time - start_time;
    if(std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count() >= ms.count())
        return -10000;
    
    int value = (max ? -1000 : 1000);
    
    if(depth == cutoff+1)
    {
       
        return heuristic(state, previousMove);
    }
    if(max)
    {
        
        
        for(int i = 0; i < moves.size(); ++i)
        {
            state[moves[i].col][moves[i].row] = AI_PIECE;
            //std::cout << "At Depth " << depth << " AI made move at: col " << moves[i].col << " and row " << moves[i].row << std::endl;
            tempV = min_max(state,false,depth+1,cutoff,alpha,beta, Move(moves[i].col,moves[i].row), start_time);
            state[moves[i].col][moves[i].row] = NO_PIECE;
            
            //Alpha Beta Pruning
            if(tempV < beta)
            {
                // Regular Max
                if(tempV > value)
                {
                   
                    value = tempV;
                    
                    Move m(moves[i].col,moves[i].row);
                   
                    // MAybe add a dictionary or array of moves here
                    if(!best_moves.empty())
                    {
                        for(int i = 0; i < best_moves.size(); i++)
                        {
                            if(value > best_moves[i].second)
                            {
                                best_moves[i] = std::pair<Move,int>(m,value);
                            }
                        }
                    }
                    else
                    {
                        best_moves.push_back(std::pair<Move,int>(m,value));
                    }
                    move = m;
                    //std::cout << "MOVE: " << move.col << " " << move.row << std::endl;
                    
                    alpha = tempV;
                }
            }
            else
            {
                //std::cout << "PRUNED" << std::endl;
                break;
            }
        }
        
    }
    else
    {
        
        
        for(int i = 0; i < moves.size(); ++i)
        {
            
            state[moves[i].col][moves[i].row] = HUMAN_PIECE;
            //std::cout << "At Depth " << depth << " Human made move at: col " << moves[i].col << " and row " << moves[i].row << std::endl;
            tempV = min_max(state,true,depth+1,cutoff,alpha,beta,Move(moves[i].col,moves[i].row), start_time);
            state[moves[i].col][moves[i].row] = NO_PIECE;
            
            //Alpha Beta Pruning
            
            if(tempV > alpha)
            {
            //Regular Min
                if(tempV < value)
                {
                    
                    value = tempV;
                    Move m(moves[i].col,moves[i].row);
                    
                    if(!best_moves.empty())
                    {
                        for(int i = 0; i < best_moves.size(); i++)
                        {
                            if(value > best_moves[i].second)
                            {
                                best_moves[i] = std::pair<Move,int>(m,value);
                            }
                        }
                    }
                    else
                    {
                        best_moves.push_back(std::pair<Move,int>(m,value));
                    }
                    move = m;
                    //std::cout << "MOVE: " << move.col << " " << move.row << std::endl;
                    
                    beta = tempV;
                    
                }
            }
            else
            {
                //std::cout << "PRUNED" << std::endl;
                break;
            }
        }
    
    }

    return value;
}

int AIShell::heuristic(int** state, Move previousMove)
{
    int vertical = upPoints(state, previousMove, false) + downPoints(state, previousMove, false);
    int horizontal = leftPoints(state, previousMove, false) + rightPoints(state, previousMove, false);
    //std::cout << "horizontal" << std::endl;
    int diagOne = rightDiagPoints(state, previousMove, false) + downLeftDiagPoints(state, previousMove, false);
    //std::cout << "after diag one" << std::endl;
    int diagTwo = leftDiagPoints(state, previousMove, false) + downRightDiagPoints(state, previousMove, false);

    //std::cout << "Diag 1: " << diagOne << std::endl;
    
    //std::cout << "Diag 2: " << diagTwo << std::endl;
    if(vertical == k)
        return 100* vertical;
    else if(horizontal == k)
        return 100*horizontal;
    else if(diagOne == k)
        return 100*diagOne;
    else if(diagTwo == k)
        return 100*diagTwo;
    
    return std::max(diagTwo,diagOne) > std::max(vertical,horizontal) ? std::max(diagTwo,diagOne) : std::max(vertical,horizontal);
}


int AIShell::rightDiagPoints(int** state, Move m, bool defense)
{
    Move rightDiag = checkRightDiag(state,m,defense);
    if(rightDiag.col == -1000 && rightDiag.row == -1000)
        return 0;
    
    return 1 + rightDiagPoints(state, rightDiag, defense);
}
int AIShell::leftDiagPoints(int** state, Move m, bool defense)
{
    Move leftDiag = checkLeftDiag(state,m,defense);
    if(leftDiag.col == -1000 && leftDiag.row == -1000)
        return 0;
    return 1 + leftDiagPoints(state, leftDiag, defense);
}
int AIShell::downLeftDiagPoints(int** state, Move m, bool defense)
{
    Move downLeftDiag = checkDownLeftDiag(state,m,defense);
    //std::cout << downLeftDiag.col << " " << downLeftDiag.row << std::endl;
    if(downLeftDiag.col == -1000 && downLeftDiag.row == -1000)
        return 0;
    return 1 + downLeftDiagPoints(state, downLeftDiag, defense);
}
int AIShell::downRightDiagPoints(int** state, Move m, bool defense)
{
    Move downRightDiag = checkDownRightDiag(state,m,defense);
    
    if(downRightDiag.col == -1000 && downRightDiag.row == -1000)
        return 0;
    return 1 + downRightDiagPoints(state, downRightDiag, defense);
}
int AIShell::upPoints(int** state, Move m, bool defense)
{
    
    Move moveAbove = checkUp(state,m, defense);
    if(moveAbove.col == -1000 && moveAbove.row == -1000)
        return 0;
    //std::cout << "Up Points" << std::endl;
    return 1 + upPoints(state,moveAbove, defense);
    
}
int AIShell::downPoints(int** state, Move m, bool defense)
{
    Move moveBelow = checkDown(state,m, defense);
    if(moveBelow.col == -1000 && moveBelow.row == -1000)
        return 0;
    //std::cout << "Down Points" << std::endl;
    return 1 + downPoints(state,moveBelow, defense);
}
int AIShell::leftPoints(int** state, Move m, bool defense)
{
    horizontalConsecutives.push_back(m);
    Move moveToLeft = checkLeft(state,m, defense);
    if(moveToLeft.col == -1000 && moveToLeft.row == -1000)
        return 0;
    //std::cout << "Left Points" << std::endl;
    
    
    return 1 + leftPoints(state,moveToLeft, defense);
}
int AIShell::rightPoints(int** state, Move m, bool defense)
{
    horizontalConsecutives.push_back(m);
    int counter = 0;
    Move moveToRight = checkRight(state,m, defense);
    if(moveToRight.col == -1000 && moveToRight.row == -1000)
        return 0;
    //std::cout << "Right Points" << std::endl;
    
    return 1 + rightPoints(state,moveToRight, defense);
}
Move AIShell::checkUp(int ** state, Move m, bool defense)
{   if(defense)
    {
        if(m.row+1 >= 0 && m.row+1 < numRows && state[m.col][m.row+1] == HUMAN_PIECE)
            return Move(m.col,m.row+1);
    }
    else
    {
        if(m.row+1 >= 0 && m.row+1 < numRows && state[m.col][m.row+1] == AI_PIECE)
            return Move(m.col,m.row+1);
    }
    return Move(-1000,-1000);
}
Move AIShell::checkDown(int ** state, Move m, bool defense)
{
    if(defense)
    {
        if(m.row-1 >= 0 && m.row-1 < numRows && state[m.col][m.row-1] == HUMAN_PIECE)
            return Move(m.col,m.row-1);
    }
    else
    {
        if(m.row-1 >= 0 && m.row-1 < numRows && state[m.col][m.row-1] == AI_PIECE)
            return Move(m.col,m.row-1);
    }
    return Move(-1000,-1000);
}
Move AIShell::checkLeft(int ** state, Move m, bool defense)
{
    if(defense)
    {
        if(m.col-1 >= 0 && m.col-1 < numCols && state[m.col-1][m.row] == HUMAN_PIECE)
            return Move(m.col-1,m.row);
    }
    else
    {
        if(m.col-1 >= 0 && m.col-1 < numCols && state[m.col-1][m.row] == AI_PIECE)
            return Move(m.col-1,m.row);
    }
    return Move(-1000,-1000);
}
Move AIShell::checkRight(int ** state, Move m, bool defense)
{
    if(defense)
    {
        if(m.col+1 >= 0 && m.col+1 < numCols && state[m.col+1][m.row] == HUMAN_PIECE)
            return Move(m.col+1,m.row);
    }
    else
    {
        if(m.col+1 >= 0 && m.col+1 < numCols && state[m.col+1][m.row] == AI_PIECE)
            return Move(m.col+1,m.row);
    }
    return Move(-1000,-1000);
}
Move AIShell::checkRightDiag(int ** state, Move m, bool defense)
{
    //std::cout << "Right Diag" << std::endl;
    if(defense)
    {
        if(m.col+1 >= 0 && m.col+1 < numCols && m.row+1 >= 0 && m.row+1 < numRows && state[m.col+1][m.row+1] == HUMAN_PIECE)
           return Move(m.col+1,m.row+1);
    }
    else
    {
        if(m.col+1 >= 0 && m.col+1 < numCols && m.row+1 >= 0 && m.row+1 < numRows && state[m.col+1][m.row+1] == AI_PIECE)
            return Move(m.col+1,m.row+1);
    }
    return Move(-1000,-1000);
}
Move AIShell::checkLeftDiag(int ** state, Move m, bool defense)
{
    //std::cout << "Left Diag" << std::endl;
    if(defense)
    {
        if(m.col-1 >= 0 && m.col-1 < numCols && m.row+1 >= 0 && m.row+1 < numRows && state[m.col-1][m.row+1] == HUMAN_PIECE)
            return Move(m.col-1,m.row+1);
    }
    else
    {
        if(m.col-1 >= 0 && m.col-1 < numCols && m.row+1 >= 0 && m.row+1 < numRows && state[m.col-1][m.row+1] == AI_PIECE)
            return Move(m.col-1,m.row+1);
    }
    return Move(-1000,-1000);
}
Move AIShell::checkDownLeftDiag(int ** state, Move m, bool defense)
{
    //std::cout << "Down Left Diag" << std::endl;
    if(defense)
    {
        if(m.col-1 >= 0 && m.col-1 < numCols && m.row-1 >= 0 && m.row-1 < numRows && state[m.col-1][m.row-1] == HUMAN_PIECE)
            return Move(m.col-1,m.row-1);
    }
    else
    {
        if(m.col-1 >= 0 && m.col-1 < numCols && m.row-1 >= 0 && m.row-1 < numRows && state[m.col-1][m.row-1] == AI_PIECE)
            return Move(m.col-1,m.row-1);
    }
        //std::cout << "didnt return in if" << std::endl;
    return Move(-1000,-1000);
}
Move AIShell::checkDownRightDiag(int ** state, Move m, bool defense)
{
    //std::cout << "Down Right Diag" << std::endl;
    if(defense)
    {
        if(m.col+1 >= 0 && m.col+1 < numCols && m.row-1 >= 0 && m.row-1 < numRows && state[m.col+1][m.row-1] == HUMAN_PIECE)
            return Move(m.col+1,m.row-1);
    }
    else
    {
        if(m.col+1 >= 0 && m.col+1 < numCols && m.row-1 >= 0 && m.row-1 < numRows && state[m.col+1][m.row-1] == AI_PIECE)
            return Move(m.col+1,m.row-1);
    }
    return Move(-1000,-1000);
}

bool AIShell::terminalState(Move m)
{
    return false;
}

std::vector<Move> AIShell::availableMoves(int** state)
{
    std::vector<Move> moves;
    for(int col = 0; col < numCols; col++)
    {
        for(int row = 0; row < numRows; row++)
        {
            if(state[col][row] == NO_PIECE)
            {
                Move m(col,row);
                moves.push_back(m);
            }
        }
    }
    return moves;
}





