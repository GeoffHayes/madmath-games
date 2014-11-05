% Test code to compile the Reversi C++ code and build the MEX function
function reversi_dev_mex_main

% declare commands
MEX_CMD_CREATE_ENGINE  = 1;
MEX_CMD_DISPLAY_BOARD  = 2;
MEX_CMD_GET_SCORE      = 3;
MEX_CMD_GET_BOARD_DIM  = 4;
MEX_CMD_GET_GAME_BOARD = 5;
MEX_CMD_APPLY_MOVE     = 6;
MEX_CMD_IS_RUNNING     = 7;
MEX_CMD_DESTROY_ENGINE = 8;
MEX_CMD_RESTART_GAME   = 9;
MEX_CMD_GET_CURR_PLYR  = 10;

BOARD_DIM = 4;

% compile the code
mex mexReversiEngineInterface.cpp ReversiEngine.cpp ReversiEngineInterface.cpp -lstdc++

try
    % create the engine
    mexReversiEngineInterface(MEX_CMD_CREATE_ENGINE,BOARD_DIM) 

    % display the board
    mexReversiEngineInterface(MEX_CMD_DISPLAY_BOARD)
    
    % get the scores for players 4 (invalid) and 2 (valid)
    score    = mexReversiEngineInterface(MEX_CMD_GET_SCORE,4)
    score    = mexReversiEngineInterface(MEX_CMD_GET_SCORE,2)
    
    % get the board dimension
    boarddim = mexReversiEngineInterface(MEX_CMD_GET_BOARD_DIM)
    
    % get the game board, make a move, and repeat (valid and invalid moves)
    game     = uint8(mexReversiEngineInterface(MEX_CMD_GET_GAME_BOARD))
    [plyrId,oppntId,isGameOver,oppntFinished,lastMoveValid,p1Score,p2Score,gameBoard] = ...
        mexReversiEngineInterface(MEX_CMD_APPLY_MOVE,1,2)
    
    game    = uint8(mexReversiEngineInterface(MEX_CMD_GET_GAME_BOARD))
    [plyrId,oppntId,isGameOver,oppntFinished,lastMoveValid,p1Score,p2Score,gameBoard] = ...
        mexReversiEngineInterface(MEX_CMD_APPLY_MOVE,4,4)
    
    game    = uint8(mexReversiEngineInterface(MEX_CMD_GET_GAME_BOARD))
    [plyrId,oppntId,isGameOver,oppntFinished,lastMoveValid,p1Score,p2Score,gameBoard] = ...
        mexReversiEngineInterface(MEX_CMD_APPLY_MOVE,1,9)
    
    game    = uint8(mexReversiEngineInterface(MEX_CMD_GET_GAME_BOARD))
    [plyrId,oppntId,isGameOver,oppntFinished,lastMoveValid,p1Score,p2Score,gameBoard] = ...
        mexReversiEngineInterface(MEX_CMD_APPLY_MOVE,3,3) 
    
    game    = uint8(mexReversiEngineInterface(MEX_CMD_GET_GAME_BOARD))
    [plyrId,oppntId,isGameOver,oppntFinished,lastMoveValid,p1Score,p2Score,gameBoard] = ...
        mexReversiEngineInterface(MEX_CMD_APPLY_MOVE,3,4) 
    
    game    = uint8(mexReversiEngineInterface(MEX_CMD_GET_GAME_BOARD))
    
    % is the engine running?
    isRunning = uint8(mexReversiEngineInterface(MEX_CMD_IS_RUNNING))
catch e
    fprintf('ERROR: %s\n',e.message);
end

% destroy the engine
mexReversiEngineInterface(MEX_CMD_DESTROY_ENGINE)

% is the engine running?
isRunning = uint8(mexReversiEngineInterface(MEX_CMD_IS_RUNNING))
