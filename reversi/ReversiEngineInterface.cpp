/**
 * Interface to the 2 player Reversi Engine.
 *
 * 2014-11-04   Geoff Hayes     Initial Release.
 */

#include "ReversiEngineInterface.h"
#include "ReversiEngine.h"
#include <exception>
#include <cstring>

ReversiEngine* pReversiEngine = 0;

bool createReversiEngine()
{
    try
    {
        if (pReversiEngine)
        {
            delete pReversiEngine;
            pReversiEngine = 0;
        }

        pReversiEngine = new ReversiEngine(REVERSI_BOARD_DIM);
    }
    catch(const std::exception& ex)
    {
        pReversiEngine = 0;
        
        // log an error
    }
    
    return pReversiEngine;
}

void destroyReversiEngine()
{
    try
    {
        if (pReversiEngine)
        {
            delete pReversiEngine;
            pReversiEngine = 0;
        } 
    }
    catch(const std::exception& ex)
    {
        pReversiEngine = 0;
        
        // log an error
    }
}

void displayReversiBoard()
{
    try
    {
        if (pReversiEngine)
        {
            pReversiEngine->displayBoard();
        }
    }
    catch(const std::exception& ex)
    {
        // intentionally left blank
    }
}
    
unsigned int getPlayerScore(const unsigned int playerId)
{
    unsigned int score = 0;
    
    try
    {
        if (pReversiEngine)
        {
            score =  pReversiEngine->getPlayerScore(playerId);
        }  
    }
    catch(const std::exception& ex)
    {
        // intentionally left blank
    }    
    
    return score;
}

unsigned int getBoardDim()
{
    unsigned int boardDim = 0;
    
    try
    {
        if (pReversiEngine)
        {
            boardDim =  pReversiEngine->getBoardDim();
        }  
    }
    catch(const std::exception& ex)
    {
        // intentionally left blank
    }        
    
    return boardDim;   
}

const unsigned int* const getBoard()
{
    if (pReversiEngine)
    {
        try
        {
            return pReversiEngine->getBoard();
        }
        catch(const std::exception& ex)
        {
        	return 0;
        }
    }
    else
    {
        return 0;
    }
}

sGameStatus applyMove(const unsigned int xcoord, const unsigned int ycoord)
{
    ReversiEngine::GAME_STATUS_TYPE moveStatus = ReversiEngine::INVALID_MOVE;
    
    sGameStatus gameStatus;
    
    try
    {
        if (pReversiEngine)
        {
            moveStatus = pReversiEngine->applyMove(xcoord,ycoord);
            
            gameStatus.wasLastMoveValid = (moveStatus==ReversiEngine::NEXT_PLAYER ||
                moveStatus==ReversiEngine::OPP_FINISHED || 
                moveStatus==ReversiEngine::GAME_OVER);
            
            gameStatus.isGameOver         = (moveStatus==ReversiEngine::GAME_OVER);
            gameStatus.isOpponentFinished = (moveStatus==ReversiEngine::OPP_FINISHED);
            gameStatus.currentPlayerId    = pReversiEngine->getCurrentPlayerId();
            gameStatus.opposingPlayerId   = pReversiEngine->getOpponentId(gameStatus.currentPlayerId);
            gameStatus.player1Score       = pReversiEngine->getPlayerScore(1);
            gameStatus.player2Score       = pReversiEngine->getPlayerScore(2);
            memcpy(gameStatus.gameBoard,pReversiEngine->getBoard(),
                   sizeof(unsigned int)*REVERSI_BOARD_DIM*REVERSI_BOARD_DIM); 
        }
    }
    catch(const std::exception& ex)
    {
        // intentionally left blank
    }
    
    return gameStatus;
}

bool isEngineRunning()
{
    return pReversiEngine;
}

void restartGame()
{
    try
    {
        if (pReversiEngine)
        {
            pReversiEngine->restartGame();
        }  
    }
    catch(const std::exception& ex)
    {
        // intentionally left blank
    }    
}

unsigned int getCurrentPlayerId()
{
    unsigned int playerId = 0;
    
    try
    {
        if (pReversiEngine)
        {
            playerId = pReversiEngine->getCurrentPlayerId();
        }
    }
    catch(const std::exception& ex)
    {
        // intentionally left blank
    }    

    return playerId;
}