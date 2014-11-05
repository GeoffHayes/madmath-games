/*! \file ReversiEngine.h
    \brief The Reversi Engine class.
 
    Engine to enforce the rules for a 2-player Reversi/Othello game.
 
    2014-11-04   Geoff Hayes     Initial Release.
*/

#ifndef REVERSI_ENGINE_H_
#define REVERSI_ENGINE_H_

//! Engine to enforce the rules for a 2-player Reversi/Othello game.
class ReversiEngine
{
    public:

        //! Type def for the player id.
        typedef unsigned int player_id_t;
        //! Type def for the game "board".
        typedef unsigned int board_dim_t;
        //! Type def for the player score.
        typedef unsigned int player_score_t;

        //! Enum to indicate game status given the last applied move.
        enum GAME_STATUS_TYPE
        {
            GAME_OVER=0,   /**< enumeration for game over indicator */
            NEXT_PLAYER,   /**< enumeration for next player's turn  */
            INVALID_MOVE,  /**< enumeration indicating invalid move */
            INVALID_COORD, /**< enumeration indicating invalid coordinate */
            VALID_MOVE,    /**< enumeration indicating valid move */
            OPP_FINISHED   /**< enumeration indicating opponent is finished */
        };

        //! Class constructor; builds the engine given the board size.
        /**
         * Class constructor to initialize the Reversi engine.
         *
         * @param   boardDim   Optional dimension of board game board.
         *
         * @note    boardDim must be even and at least four.
         */
        ReversiEngine(const board_dim_t boardDim = MIN_BOARD_DIM);
        
        //! Class desctructor; frees memory allocated to game board.
        ~ReversiEngine();          
        
        //! Returns the board dimension.
        board_dim_t getBoardDim() const {return _boardDim;}
        
        //! Returns the player's score.
        /**
         * Returns the score given the player id.
         *
         * @param   playerId   The identifier for the player.
         *
         * @note    playerId is assumed to be positive (>0).
         */
        player_score_t getPlayerScore(const player_id_t playerId) const;
        
        //! Returns the current player id.
        /**
         * Returns the current player identifier.
         */
        player_id_t getCurrentPlayerId() const {return _currentPlayerId;} 
        
        //! Returns the opponent id.
        /**
         * Gets the opponent (other player) identifier given the specified
         * player.
         */
        player_id_t getOpponentId(const player_id_t playerId) const;        
        
        //! Resizes the board.
        /**
         * Resizes the board.
         *
         * @param   newBoardDim   The new board dimension.
         *
         * @warning If a current game is in progress, it will be lost and 
         *          a new one started.
         */
        void resizeBoard(const board_dim_t newBoardDim);
        
        //! Restarts the game.
        void restartGame();
    
        //! Displays the board on the console.
        void displayBoard() const;   
        
        //! Returns the board.
        const board_dim_t* const getBoard(){return _gameBoard;};
        
        //! Applies the player's move.
        /**
         * Applies a move for the current player.
         *
         * @param   xcoord        The x coordinate on the gaming board.
         * @param   ycoord        The y coordinate on the gaming board.
         *
         * @return  The status of the game given that move.
         *
         * @note    The x and y coordinates are one-based with coordinate
         *          (1,1) the top-left square of the board, (1,_boardDim) 
         *          the top-right square of the board, (_boardDim,1) the 
         *          bottom-left square of the board, and (_boardDim,_boardDim)
         *          the bottom-right square of the board.
         */
        GAME_STATUS_TYPE applyMove(const board_dim_t xcoord, 
                                   const board_dim_t ycoord);
        
    private:
        
        //! Enum to manage default values.
        enum DEFAULT_VALUES
        {
            MIN_BOARD_DIM    = 4, /**< enumeration for the minimum board dimension */
            MAX_NUM_PLAYERS = 2,  /**< enumeration for the maximum number of players */
            FREE_SPACE_ID   = 0   /**< enumeration for the free space identifier */
        };
        
        //! Initializes the board game for two players.
        /**
         * Initializes the board game board for two players, with the four 
         * centre squares set with two discs for each player.
         */
        void initBoard();
        
        //! Change turns with the other player.
        void gotoNextPlayer();

        //! Updates the scores for all players.
        void updateScores();
        
        //! Determines if move exists for player.
        /**
         * Determines if at least one valid move exists for the player.
         *
         * @param   playerId   The player identifier.
         *
         * @retval  true if at least one move exists.
         * @retval  false if no moves exist.
         */
        bool doesMoveExist(const player_id_t playerId);
        
        //! Indicates if player's move is valid.
        /**
         * Indicates if the move is valid or not for the specified player.  
         * Applies move if optional flag is set.
         *
         * @param   xcoord        The x coordinate on the gaming board.
         * @param   ycoord        The y coordinate on the gaming board.
         * @param   playerId      The player identifier.
         * @param   applyMove     Optional indicator to apply move the move.
         *
         * @return  The status of the game given that move.
         *
         * @note    The x and y coordinates are one-based with coordinate
         *          (1,1) the top-left square of the board, (1,_boardDim) 
         *          the top-right square of the board, (_boardDim,1) the 
         *          bottom-left square of the board, and (_boardDim,_boardDim)
         *          the bottom-right square of the board.         
         */
        GAME_STATUS_TYPE isMoveValid(const board_dim_t xcoord, 
                                     const board_dim_t ycoord,
                                     const player_id_t playerId,
                                     const bool applyMove = false);       

        //! Dimension of square board game board.
        board_dim_t _boardDim;
        
        //! Game board with discs.
        board_dim_t* _gameBoard;
        
        //! Array of player identifiers.
        player_id_t _playerIds[MAX_NUM_PLAYERS];
        
        //! Array of player scores.
        player_score_t _playerScores[MAX_NUM_PLAYERS];
        
        //! Identifier for the current player.
        player_id_t _currentPlayerId;
        
        //! Indicates whether game is in play or not.
        bool _isGameInPlay;
};

#endif /* REVERSI_ENGINE_H_ */