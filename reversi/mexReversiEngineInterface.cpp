/**
 * MATLAB MEX function interface to the ReversiEngine.
 *
 * 2014-11-04   Geoff Hayes     Initial Release.
 */
 
#include <math.h>
#include "mex.h"
#include "ReversiEngineInterface.h"
#include <iostream>

// commands
#define	CREATE_ENGINE  1
#define DISPLAY_BOARD  2
#define GET_SCORE      3
#define GET_BOARD_DIM  4
#define GET_BOARD      5
#define APPLY_MOVE     6
#define IS_RUNNING     7
#define	DESTROY_ENGINE 8
#define RESTART_GAME   9
#define GET_CURR_PLYR  10

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])   
{ 
    unsigned int command = 0;
    
    if (nrhs>=1)
    {
        command = static_cast<unsigned int>(*mxGetPr(prhs[0]));

        switch(command)
        {
            case CREATE_ENGINE:
            {
                std::cout << 
                    "mexRevEngIface - creating reversi engine" << std::endl;

                createReversiEngine();
                break;
            }
            case DESTROY_ENGINE:
            {
                std::cout << 
                    "mexRevEngIface - destroying reversi engine" << std::endl;
                    
               destroyReversiEngine();
               break;
            }
            case DISPLAY_BOARD:
            {  
               displayReversiBoard();
               break;
            }   
            case GET_SCORE:
            {
                if (nlhs<1)
                {
                    std::cout << 
                        "mexRevEngIface - missing score output parameter" << std::endl;
                }
                else if (nrhs>=2)
                {
                    const unsigned int playerId = 
                            static_cast<unsigned int>(*mxGetPr(prhs[1]));
                    
                    const unsigned int score = getPlayerScore(playerId);

                    plhs[0] = mxCreateDoubleScalar(static_cast<double>(score));
                }
                else
                {
                    std::cout << 
                        "mexRevEngIface - missing player id input" << std::endl;
                }
                break;
            } 
            case GET_BOARD_DIM:
            {
                if (nlhs<1)
                {
                    std::cout << 
                        "mexRevEngIface - missing board dim output parameter" << std::endl;
                }
                else
                {
                    const unsigned int boardDim = getBoardDim();

                    plhs[0] = mxCreateDoubleScalar(static_cast<double>(boardDim));
                }
                break;
            }  
            case GET_BOARD:
            {
                if (nlhs<1)
                {
                    std::cout << 
                        "mexRevEngIface - missing board dim output parameter" << std::endl;
                }
                else
                {
                    const unsigned int        boardDim = getBoardDim();
                    const unsigned int* const pBoard   = getBoard();
    
                    // create the output matrix
                    plhs[0] = mxCreateDoubleMatrix(boardDim,boardDim,mxREAL);
                    double* ptr = mxGetPr(plhs[0]);
                    
                    // copy the data
                    for (int m=0;m<boardDim;++m)
                    {
                        for (int n=0;n<boardDim;++n)
                        {
                            ptr[m+boardDim*n] = pBoard[m*boardDim+n];
                        }
                    }                    
                }
                break;
            }  
            case APPLY_MOVE:
            {
                if (nlhs<8)
                {
                    std::cout << 
                        "mexRevEngIface - missing move output parameters" << std::endl;
                }
                else if (nrhs>=3)
                {
                    const unsigned int xcoord = 
                            static_cast<unsigned int>(*mxGetPr(prhs[1]));
                    
                    const unsigned int ycoord = 
                            static_cast<unsigned int>(*mxGetPr(prhs[2]));   
                    
                    const sGameStatus gameStatus = applyMove(xcoord,ycoord);

                    plhs[0] = mxCreateDoubleScalar(static_cast<double>(gameStatus.currentPlayerId));
                    plhs[1] = mxCreateDoubleScalar(static_cast<double>(gameStatus.opposingPlayerId));
                    plhs[2] = mxCreateDoubleScalar(static_cast<double>(gameStatus.isGameOver));
                    plhs[3] = mxCreateDoubleScalar(static_cast<double>(gameStatus.isOpponentFinished));
                    plhs[4] = mxCreateDoubleScalar(static_cast<double>(gameStatus.wasLastMoveValid));
                    plhs[5] = mxCreateDoubleScalar(static_cast<double>(gameStatus.player1Score));
                    plhs[6] = mxCreateDoubleScalar(static_cast<double>(gameStatus.player2Score));
                    
                    // create the output matrix
                    plhs[7] = mxCreateDoubleMatrix(REVERSI_BOARD_DIM,REVERSI_BOARD_DIM,mxREAL);
                    double* ptr = mxGetPr(plhs[7]);
                    
                    // copy the data
                    for (int m=0;m<REVERSI_BOARD_DIM;++m)
                    {
                        for (int n=0;n<REVERSI_BOARD_DIM;++n)
                        {
                            ptr[m+REVERSI_BOARD_DIM*n] = gameStatus.gameBoard[m*REVERSI_BOARD_DIM+n];
                        }
                    }  
                    
                }
                else
                {
                    std::cout << 
                        "mexRevEngIface - missing move coordinates inputs" << std::endl;
                }
                break;
            }       
            case IS_RUNNING:
            {
                if (nlhs<1)
                {
                    std::cout << 
                        "mexRevEngIface - missing is running output parameter" << std::endl;
                }
                else
                {
                    const bool isRunning = isEngineRunning();
                    
                    plhs[0] = mxCreateDoubleScalar(static_cast<double>(isRunning));                 
                }
                break;
            }    
            case RESTART_GAME:
            {
                std::cout << 
                        "mexRevEngIface - restarting reversi engine" << std::endl;
                
                restartGame();
                
                break;
            }   
            case GET_CURR_PLYR:
            {
                if (nlhs<1)
                {
                    std::cout << 
                        "mexRevEngIface - missing player id output parameter" << std::endl;
                }
                else
                {
                    const unsigned int playerId = getCurrentPlayerId();

                    plhs[0] = mxCreateDoubleScalar(static_cast<double>(playerId));
                }
                break;
            }             
            default:
            {
                std::cout << 
                    "mexRevEngIface - invalid command " << command << std::endl; 
            }
        }
    }
}