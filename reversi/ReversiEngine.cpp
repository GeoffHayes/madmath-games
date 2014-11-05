/**
 * Engine to enforce the rules for a 2-player Reversi/Othello game.
 *
 * 2014-11-04   Geoff Hayes     Initial Release.
 *
 */
#include "ReversiEngine.h"
#include <cmath>
#include <cstring>
#include <iostream>

ReversiEngine::ReversiEngine(const board_dim_t boardDim) : 
    _boardDim(0),
    _gameBoard(0),
    _currentPlayerId(0),
    _isGameInPlay(true)
{ 
    // initialize the player ids
    for (int k=0;k<MAX_NUM_PLAYERS;++k)
    {
        _playerIds[k] = k+1;
    }
    
    // initialize the player scores
    memset(_playerScores,0,sizeof(player_score_t)*MAX_NUM_PLAYERS);
    
    // size the board and initialize the board
    resizeBoard(boardDim);
}
 
ReversiEngine::~ReversiEngine()
{
    if (_gameBoard)
    {
        delete [] _gameBoard;
        _gameBoard = 0;
    }
}    
    
void ReversiEngine::initBoard()
{
    // determine the origin, subtracting one since arrays are zero based
    const board_dim_t xOrigin = _boardDim/2-1;
    
    memset(_gameBoard,0,sizeof(board_dim_t)*_boardDim*_boardDim);
    
    _gameBoard[xOrigin*_boardDim+xOrigin]       = _playerIds[1];
    _gameBoard[xOrigin*_boardDim+xOrigin+1]     = _playerIds[0];
    _gameBoard[(xOrigin+1)*_boardDim+xOrigin]   = _playerIds[0];
    _gameBoard[(xOrigin+1)*_boardDim+xOrigin+1] = _playerIds[1];  
    
    // update the player scores
    updateScores();
    
    _currentPlayerId = 1;
}

ReversiEngine::player_score_t ReversiEngine::getPlayerScore(const player_id_t playerId) const
{
    player_score_t score = 0;
    
    if (playerId>=1 && playerId<=MAX_NUM_PLAYERS)
    {
        score = _playerScores[playerId-1];
    }
    
    return score;
}

void ReversiEngine::displayBoard() const
{
   for (int u=0;u<_boardDim;++u)
   {
       for (int v=0;v<_boardDim;++v)
       {
           std::cout << _gameBoard[u*_boardDim+v] << "\t";
       }
       std::cout << std::endl;
   }
}

void ReversiEngine::gotoNextPlayer()
{
    _currentPlayerId++;
    
    if (_currentPlayerId>MAX_NUM_PLAYERS)
    {
        _currentPlayerId = 1;
    }
}

void ReversiEngine::resizeBoard(const board_dim_t newBoardDim)
{
    const board_dim_t oldBoardDim = _boardDim;
    
    _boardDim = newBoardDim;
    
    // validate the board size
    if (_boardDim%2!=0)
    {
        _boardDim += 1;
    }
    
    if (_boardDim<MIN_BOARD_DIM)
    {
        _boardDim = MIN_BOARD_DIM;
    }
    
    // allocate memory for the board (if necessary)
    if (_gameBoard && oldBoardDim!=_boardDim)
    {
        delete [] _gameBoard;
        _gameBoard = 0;
    }
    else if (!_gameBoard)
    {
        _gameBoard = new board_dim_t[_boardDim*_boardDim];
    }
    
    initBoard();
}

void ReversiEngine::updateScores()
{
    board_dim_t const*  pGameBoard = _gameBoard;

    memset(_playerScores,0,sizeof(player_score_t)*MAX_NUM_PLAYERS);
    
    for (int k=0;k<_boardDim*_boardDim;++k)
    {
        const board_dim_t id = *pGameBoard++;
        if (id>0 && id<=MAX_NUM_PLAYERS)
        {
            _playerScores[id-1]++;
        }
    }
}

ReversiEngine::player_id_t ReversiEngine::getOpponentId(
        const player_id_t playerId) const
{
    player_id_t opponentId = playerId+1;
    
    if (opponentId>MAX_NUM_PLAYERS)
    {
        opponentId = 1;
    }
    
    return opponentId;
}

ReversiEngine::GAME_STATUS_TYPE ReversiEngine::applyMove(
        const board_dim_t xcoord, 
        const board_dim_t ycoord)
{
    ReversiEngine::GAME_STATUS_TYPE status = INVALID_MOVE;
    
    if (_isGameInPlay)
    {
    	status = isMoveValid(xcoord,ycoord,_currentPlayerId,true);
    
        switch (status)
        {
            case VALID_MOVE:
            {
                // update the scores
                updateScores();
                
                // do valid moves exists for opponent?
                const player_id_t opponentId = getOpponentId(_currentPlayerId);
                if (doesMoveExist(opponentId))
                {
                    // allow the opponent to play
                    gotoNextPlayer(); 
                    status = NEXT_PLAYER;  
                }
                else
                {
                    // opponent can't play, but can current player?
                    if (doesMoveExist(_currentPlayerId))
                    {
                        status = OPP_FINISHED;
                    }
                    else
                    {
                        // game over!
                        status = GAME_OVER;
                        _isGameInPlay = false;
                    }
                }
                break;
            }
            case INVALID_MOVE:
            case INVALID_COORD:
            {
                // intentionally left blank
                break;
            }
            default:
            {
                // intentionally left blank
            }
        }
    }

    return status;
}

void ReversiEngine::restartGame()
{
    resizeBoard(_boardDim);
    _isGameInPlay = true;
}

ReversiEngine::GAME_STATUS_TYPE ReversiEngine::isMoveValid(
        const board_dim_t xcoord, 
        const board_dim_t ycoord,
        const player_id_t playerId,
        const bool applyMove)
{
    ReversiEngine::GAME_STATUS_TYPE status = INVALID_MOVE;
    
    bool foundMove = false;
    
    // ensure that x and y are within the gaming area
    if (xcoord<1 || xcoord>_boardDim || ycoord<1 || ycoord>_boardDim)
    {
        status = ReversiEngine::INVALID_COORD;
    }
    
    // ensure that the selected square is empty
    else if (_gameBoard[(xcoord-1)*_boardDim + (ycoord-1)] != FREE_SPACE_ID)
    {
        status = ReversiEngine::INVALID_MOVE;
    }
    
    // ensure that there is a "string" of uninterrupted opponent
    // discs terminated with a player disc
    else
    {
        const player_id_t opponentId = getOpponentId(playerId);

        // consider all neighbouring squares surrounding the square that 
        // the user has applied his/her move to
        //
        // as there are eight neighbours, we look at all combinations of
        // left or right with above or below
        for (int m=-1;m<=1;++m)
        {
             const int xcoordP = static_cast<int>(xcoord) + m;
             
             // skip if new coordinate is at edge
             if (xcoordP<1 || xcoordP>_boardDim)
             {
                continue;
             }
        
             for (int n=-1;n<=1;++n)
             {
                const int ycoordP = static_cast<int>(ycoord) + n;
                
                // skip if new coordinate is at edge
                if (ycoordP<1 || ycoordP>_boardDim)
                {
                    continue;
                }

                if (_gameBoard[(xcoordP-1)*_boardDim + (ycoordP-1)]==opponentId)
                {
                    // neighbouring square is that for an opponent
              
                    // if we continue with the same +m,+n step sizes, is
                    // the end point the player's id?
                    int xcoordPP = xcoordP;
                    int ycoordPP = ycoordP;
                   
                    // count the number of opponent squares that we are
                    // stepping over (and that would be flipped if a successful
                    // move has been found)
                    int numOppSqrs = 1;

                    // there will be a maximum of _boardDim steps
                    for (int steps=0;steps<_boardDim;++steps)
                    {
                        xcoordPP = xcoordPP+m;
                        ycoordPP = ycoordPP+n;
                   
                        if (xcoordPP<1 || xcoordPP>_boardDim || 
                            ycoordPP<1 || ycoordPP>_boardDim)
                        {
                            // hit edge of game board, so exit
                            break;
                        }
                        else if (_gameBoard[
                            (xcoordPP-1)*_boardDim + (ycoordPP-1)] == 
                                FREE_SPACE_ID)
                        {
                            // hit free space so invalid
                            break;
                        }
                        else if (_gameBoard[
                            (xcoordPP-1)*_boardDim + (ycoordPP-1)] ==
                                playerId)
                        {
                            // a valid move has been found
                            foundMove = true;
                       
                            // exit if we are checking for a valid move
                            if (!applyMove)
                            {
                                break;
                            }
                            
                            // move is allowed, so apply it working backward
                            // until we reach the original position, marking
                            // opponent squares as the current player ones
                            // (we add one to numOppSqrs to capture the square
                            // that the user has selected (stating point))
                            for (int k=1;k<=numOppSqrs+1;++k)
                            {
                                xcoordPP = xcoordPP-m;
                                ycoordPP = ycoordPP-n;
                               
                                _gameBoard[
                                   (xcoordPP-1)*_boardDim + (ycoordPP-1)] =
                                        playerId;
                            }
 
                            // exit
                            break;
                        }
                        else
                        {
                            // neighbouring square is that of opponent
                            numOppSqrs++;
                        }
                    }
                }
                
                // exit early if a move has been found and we are checking only
                if (foundMove && !applyMove)
                {
                    break;
                }
            }
            
             // exit early if a move has been found and we are checking only
            if (foundMove && !applyMove)
            {
                break;
            }
        }
    }
    
    if (foundMove)
    {
        status = VALID_MOVE;
    }

    return status;
}

bool ReversiEngine::doesMoveExist(const player_id_t playerId)
{
    bool status = false;
    
    // iterate over all possible moves
    for (board_dim_t m=1;m<=_boardDim;++m)
    {
        for (board_dim_t n=1;n<=_boardDim;++n)
        {
            status = (isMoveValid(m,n,playerId)==VALID_MOVE);
            if (status)
            {
                break;
            }
        }
        if (status)
        {
            break;
        }
    }
    
    return status;
}