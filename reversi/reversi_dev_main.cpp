/**
 * Test code for the Reversi Engine Interface.
 *
 * 2014-11-04   Geoff Hayes     Initial Release.
 */

// code compiled as 
// 
//   gcc -o reversi reversi_dev_main.cpp ReversiEngine.cpp ReversiEngineInterface.cpp -lstdc++

#include "ReversiEngineInterface.h"
#include <iostream>

void writeGameStatus(const sGameStatus& gameStatus);

int main()
{
    // create the engine
    const bool status = createReversiEngine();
    std::cout << "Reversi engine created: " << status << std::endl;
   
    // display the board
    std::cout << "\nReversi Game Board:\n";
    displayReversiBoard();
    
    // get player scores
    unsigned int playerScore = getPlayerScore(1);
    std::cout << "\nplayer 1 score: " << playerScore << std::endl;
    
    playerScore = getPlayerScore(2);
    std::cout << "player 2 score: " << playerScore << std::endl;
    
    playerScore = getPlayerScore(3);
    std::cout << "player 3 score: " << playerScore << std::endl;
    
    const unsigned int boardDim = getBoardDim();
    std::cout << "\nGame board dim: " << boardDim << std::endl;
    
    const unsigned int currentPlayerId = getCurrentPlayerId();
    std::cout << "\nCurrent player id: " << currentPlayerId << std::endl;
    
    const unsigned int* const board = getBoard();
    std::cout << "\nCurrent board:\n";
    for (int k=1;k<=boardDim;++k)
    {
        for (int m=1;m<=boardDim;++m)
        {
            std::cout << board[(k-1)*boardDim+(m-1)] << "\t";
        }
        std::cout << std::endl;
    }
    
    bool isRunning = isEngineRunning();
    std::cout << "\nEngine is running: " << isRunning << std::endl;
    
    // mini-game, assumes 4x4 grid
    sGameStatus gameStatus = applyMove(1,1);
    writeGameStatus(gameStatus);
    
    gameStatus = applyMove(2,1);
    writeGameStatus(gameStatus);
    
    gameStatus = applyMove(3,1);
    writeGameStatus(gameStatus);    
    
    gameStatus = applyMove(4,1);
    writeGameStatus(gameStatus);    
    
    gameStatus = applyMove(1,3);
    writeGameStatus(gameStatus); 
    
    gameStatus = applyMove(2,4);
    writeGameStatus(gameStatus);
    
    gameStatus = applyMove(1,1);
    writeGameStatus(gameStatus);
    
    gameStatus = applyMove(4,2);
    writeGameStatus(gameStatus);
    
    gameStatus = applyMove(4,3);
    writeGameStatus(gameStatus);
    
    gameStatus = applyMove(1,2);
    writeGameStatus(gameStatus);
    
    gameStatus = applyMove(1,4);
    writeGameStatus(gameStatus);
    
    gameStatus = applyMove(3,4);
    writeGameStatus(gameStatus);
    
    gameStatus = applyMove(4,4);
    writeGameStatus(gameStatus);
    
    // destroy the engine
    destroyReversiEngine();
    std::cout << "\nReversi engine destroyed!" << std::endl;
    
    isRunning = isEngineRunning();
    std::cout << "\nEngine is running: " << isRunning << std::endl;
    
    return 0; 
}

void writeGameStatus(const sGameStatus& gameStatus)
{
    std::cout << "\nCurrent player id:  " << gameStatus.currentPlayerId 
              << "\nOpponent player id: " << gameStatus.opposingPlayerId
              << "\nGame over:          " << gameStatus.isGameOver
              << "\nIs opponent out:    " << gameStatus.isOpponentFinished
              << "\nLast move valid:    " << gameStatus.wasLastMoveValid
              << "\nPlayer 1 score:     " << gameStatus.player1Score
              << "\nPlayer 2 score:     " << gameStatus.player2Score
              << "\nGame board:\n";
    
    for (int k=1;k<=REVERSI_BOARD_DIM;++k)
    {
        for (int m=1;m<=REVERSI_BOARD_DIM;++m)
        {
            std::cout << gameStatus.gameBoard[(k-1)*REVERSI_BOARD_DIM+(m-1)] << "\t";
        }
        std::cout << std::endl;
    }
}