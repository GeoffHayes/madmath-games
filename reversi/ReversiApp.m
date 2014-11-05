function varargout = ReversiApp(varargin)
    % REVERSIAPP MATLAB code for ReversiApp.fig
    %      REVERSIAPP, by itself, creates a new REVERSIAPP or raises the existing
    %      singleton*.
    %
    %      H = REVERSIAPP returns the handle to a new REVERSIAPP or the handle to
    %      the existing singleton*.
    %
    %      REVERSIAPP('CALLBACK',hObject,eventData,handles,...) calls the local
    %      function named CALLBACK in REVERSIAPP.M with the given input arguments.
    %
    %      REVERSIAPP('Property','Value',...) creates a new REVERSIAPP or raises the
    %      existing singleton*.  Starting from the left, property value pairs are
    %      applied to the GUI before ReversiApp_OpeningFcn gets called.  An
    %      unrecognized property name or invalid value makes property application
    %      stop.  All inputs are passed to ReversiApp_OpeningFcn via varargin.
    %
    %      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
    %      instance to run (singleton)".
    %
    % See also: GUIDE, GUIDATA, GUIHANDLES

    % Edit the above text to modify the response to help ReversiApp

    % Last Modified by GUIDE v2.5 04-Nov-2014 22:18:53

    % Begin initialization code - DO NOT EDIT
    gui_Singleton = 1;
    gui_State = struct('gui_Name',       mfilename, ...
                       'gui_Singleton',  gui_Singleton, ...
                       'gui_OpeningFcn', @ReversiApp_OpeningFcn, ...
                       'gui_OutputFcn',  @ReversiApp_OutputFcn, ...
                       'gui_LayoutFcn',  [] , ...
                       'gui_Callback',   []);
    if nargin && ischar(varargin{1})
        gui_State.gui_Callback = str2func(varargin{1});
    end

    if nargout
        [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
    else
        gui_mainfcn(gui_State, varargin{:});
    end
    % End initialization code - DO NOT EDIT]
end

%--------------------------------------------------------------------------
% --- Executes just before ReversiApp is made visible.
function ReversiApp_OpeningFcn(hObject, ~, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to ReversiApp (see VARARGIN)

    % Choose default command line output for ReversiApp
    handles.output = hObject;

    % defaut the board size
    boardSize = 8;

    % check to see if the board size has been passed in
    if nargin>3
       for index = 1:2:(nargin-3),
            if nargin-3==index
                break;
            end

            switch lower(varargin{index})
             case 'boardsize'
                boardSize = varargin{index+1};
            end
        end 
    end

    % initialize board properties
    handles.boardSize          = boardSize;
    handles.boardSqrSizePixels = 100;
    handles.boardObjRadius     = 45;
    handles.playerColours      = [0 0 0; 255 255 255];

    % Update handles structure
    guidata(hObject, handles);

    % format and refresh the board
    formatBoard(handles);
    refreshBoard(handles);

    % UIWAIT makes ReversiApp wait for user response (see UIRESUME)
    % uiwait(handles.figReversiApp);
end

%--------------------------------------------------------------------------
% --- Outputs from this function are returned to the command line.
function varargout = ReversiApp_OutputFcn(~, ~, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Get default command line output from handles structure
    varargout{1} = handles.output;
end

%--------------------------------------------------------------------------
function formatBoard(handles)

    % get the board layout 
    handles = guidata(handles.figReversiApp);
    boardSqrSizePixels = handles.boardSqrSizePixels;
    boardSize         = handles.boardSize;

    % create an image with a sky blue background
    gameBoardImg = uint8(zeros(boardSize*boardSqrSizePixels+2,...
        boardSize*boardSqrSizePixels+2,3));

    gameBoardImg(:,:,1) = 135;
    gameBoardImg(:,:,2) = 206;
    gameBoardImg(:,:,3) = 250;

    % delete the image if one already exists
    if isfield(handles,'hImage')
        delete(handles.hImage);
    end

    % draw the image on the axes
    handles.hImage = image(gameBoardImg,'Parent',handles.axesBoard);

    % ensure that the axes is square and the grid is turned off
    axis(handles.axesBoard,'square','off');

    % save the updated handles structure
    guidata(handles.axesBoard,handles);

    % allow for button down clicks on the image
    set(handles.hImage,'ButtonDownFcn',{@gameButtonDownFcn,handles.figReversiApp});
end

%--------------------------------------------------------------------------
function gameButtonDownFcn(hObject,~,hFigure)
   
    MEX_CMD_IS_RUNNING    = 7;

    % get the board layout 
    handles = guidata(hFigure);
    boardSqrSizePixels = handles.boardSqrSizePixels;

    % get the coordinate
    hAxes  = get(hObject,'Parent');
    coord  = get(hAxes,'CurrentPoint');
    coord  = coord(1,1:2);

    % map the coordinate to a square
    sqrCoord = ceil(coord/boardSqrSizePixels);

    % use the engine if it is running
    isRunning = uint8(mexReversiEngineInterface(MEX_CMD_IS_RUNNING));

    if isRunning
        applyMoveMex(sqrCoord,handles);
    else
        % apply the move
        [isValid,handles] = applyMove(sqrCoord,handles);

        if isValid

            % draw the circle
            objectColour = handles.playerColours(handles.playerTurnId,:);

            drawCircle(sqrCoord,objectColour,handles);

            handles.playerPieces(sqrCoord(2),sqrCoord(1)) = handles.playerTurnId;

            handles.playerTurnId = max(mod(handles.playerTurnId + 1,3),1);

            guidata(hFigure,handles);

            % update the scores
            player1Score = length(find(handles.playerPieces==1));
            player2Score = length(find(handles.playerPieces==2));

            set(handles.txtPlayer1Score,'String',['Player 1: ' num2str(player1Score)]);
            set(handles.txtPlayer2Score,'String',['Player 2: ' num2str(player2Score)]);

            % can the next player move?
            if doValidMovesExist(handles)
                msg = sprintf('It is Player %d''s move\n',handles.playerTurnId);
                set(handles.txtStatus,'String',msg);
            else
                msg = sprintf('There are no valid moves for Player %d\n',handles.playerTurnId);
                set(handles.txtStatus,'String',msg);
                handles.playerTurnId = max(mod(handles.playerTurnId + 1,3),1);
                guidata(hFigure,handles);

                if ~doValidMovesExist(handles)
                    msg = {msg ; sprintf('There are no valid moves for Player %d\n',handles.playerTurnId)};
                    msg = [msg ; sprintf('Game over!!\n')];
                    set(handles.txtStatus,'String',msg);
                end
            end
        else
            beep;
        end
    end
end

%--------------------------------------------------------------------------
function refreshBoard(handles)

    MEX_CMD_CREATE_ENGINE = 1;
    MEX_CMD_IS_RUNNING    = 7;
    MEX_CMD_RESTART_GAME  = 9;
    MEX_CMD_GET_CURR_PLYR = 10;

    % get the board layout 
    handles            = guidata(handles.figReversiApp);
    boardSqrSizePixels = handles.boardSqrSizePixels;
    boardSize          = handles.boardSize;

    % create or restart the engine depending upon whether it is running or not
    isRunning = uint8(mexReversiEngineInterface(MEX_CMD_IS_RUNNING));

    if ~isRunning
        % create the Reversi Engine
        mexReversiEngineInterface(MEX_CMD_CREATE_ENGINE);
        isRunning = uint8(mexReversiEngineInterface(MEX_CMD_IS_RUNNING));
    else
        % restart the Reversi Engine
        mexReversiEngineInterface(MEX_CMD_RESTART_GAME);
    end

    gameBoardImg = get(handles.hImage,'CData');

    % draw the board lines
    for k=1:boardSize+1
        idx = (k-1)*boardSqrSizePixels+1;
        if mod(k,2)==0
            gameBoardImg(idx:idx+2,:,:) = 0;
        else
            gameBoardImg(idx:idx+1,:,:) = 0;
        end
    end

    for k=1:boardSize+1
        idx = (k-1)*boardSqrSizePixels+1;
        if mod(k,2)==0
            gameBoardImg(:,idx:idx+2,:) = 0;
        else
            gameBoardImg(:,idx:idx+1,:) = 0;
        end
    end

    set(handles.hImage,'CData',gameBoardImg);

    % draw the player one and player two initial pieces
    firstSqr = [floor(boardSize/2) floor(boardSize/2)];
    drawCircle(firstSqr,handles.playerColours(2,:),handles);
    drawCircle(firstSqr+[1 0],handles.playerColours(1,:),handles);
    drawCircle(firstSqr+[0 1],handles.playerColours(1,:),handles);
    drawCircle(firstSqr+[1 1],handles.playerColours(2,:),handles);

    % if the engine is not running, then initialize the attributes for the
    % MATLAB version
    if ~isRunning
        handles.playerPieces       = uint8(zeros(handles.boardSize,handles.boardSize));

        % update the player pieces
        handles.playerPieces = uint8(zeros(handles.boardSize,handles.boardSize));
        handles.playerPieces(firstSqr(1),firstSqr(2))     = 2;
        handles.playerPieces(firstSqr(1)+1,firstSqr(2)+1) = 2;
        handles.playerPieces(firstSqr(1),firstSqr(2)+1)   = 1;
        handles.playerPieces(firstSqr(1)+1,firstSqr(2))   = 1;

        handles.playerTurnId = 1;

        currentPlayerId = handles.playerTurnId ;
    else
        currentPlayerId = uint8(mexReversiEngineInterface(MEX_CMD_GET_CURR_PLYR));
    end

    guidata(handles.figReversiApp,handles);

    set(handles.txtPlayer1Score,'String','Player 1: 2');
    set(handles.txtPlayer2Score,'String','Player 2: 2');

    set(handles.txtPlayer1Clr,'ForegroundColor',[1 0 0], ...
        'BackgroundColor',handles.playerColours(1,:)./255);
    set(handles.txtPlayer2Clr,'ForegroundColor',[1 0 0], ...
        'BackgroundColor',handles.playerColours(2,:)./255);

    msg = sprintf('It is Player %d''s move\n',currentPlayerId);
    set(handles.txtStatus,'String',msg);
end

%--------------------------------------------------------------------------
function drawCircle(sqrCoord,colour,handles)

    % get the board layout 
    boardSqrSizePixels = handles.boardSqrSizePixels;
    boardObjRadius     = handles.boardObjRadius;

    % draw a circle in the square
    circleOrigin = floor((sqrCoord-1)*boardSqrSizePixels + boardSqrSizePixels/2);

    circleToDraw = uint8(zeros(2*boardObjRadius+1,2*boardObjRadius+1,3));

    circleToDraw(:,:,1) = 135;
    circleToDraw(:,:,2) = 206;
    circleToDraw(:,:,3) = 250;

    for m=1:2*boardObjRadius
        for n=1:2*boardObjRadius
            if (m-2*boardObjRadius/2)^2 + (n-2*boardObjRadius/2)^2 <= ...
                    boardObjRadius^2
               circleToDraw(m,n,:) = colour; 
            end
        end
    end

    % get the image data
    gameBoardImg = get(handles.hImage,'CData');

    xo = circleOrigin(1);
    yo = circleOrigin(2);

    gameBoardImg(yo-boardObjRadius:yo+boardObjRadius, ...
              xo-boardObjRadius:xo+boardObjRadius, ...
              :) = circleToDraw;

    set(handles.hImage,'CData',gameBoardImg);
end

%--------------------------------------------------------------------------
function [isValid,handles] = applyMove(sqrCoord,handles,doCheckOnly)

    if ~exist('doCheckOnly','var')
        doCheckOnly = false;
    end

    x = sqrCoord(1);
    y = sqrCoord(2);

    % is the selected position empty?
    if handles.playerPieces(y,x)
        isValid = false;
    else
        % is there a neighbouring piece from the opponent?
        playerId  = handles.playerTurnId;
        oppontId  = max(mod(playerId + 1,3),1);

        foundMove = false;
        for m=-1:1:1
            xp = x+m;
            if xp<1 || xp>handles.boardSize
                continue;
            end

            for n=-1:1:1
                yp = y+n;
                if yp<1 || yp>handles.boardSize
                    continue;
                end

                if handles.playerPieces(yp,xp)==oppontId

                   % is the end point the player's id?
                   xpp = xp;
                   ypp = yp;

                   while true

                       xpp = xpp+m;
                       ypp = ypp+n;

                       if xpp<1 || xpp>handles.boardSize || ypp<1 || ypp>handles.boardSize
                           % hit edge of game board, so exit
                           break;
                       elseif handles.playerPieces(ypp,xpp)==0
                           % hit whitespace so invalid
                           break;

                       elseif handles.playerPieces(ypp,xpp)==playerId

                           foundMove = true;

                           if doCheckOnly
                               break;
                           end

                           % move is allowed, so apply it
                           xppp = xp;
                           yppp = yp;

                           while true

                               if handles.playerPieces(yppp,xppp)==playerId
                                   break;
                               else
                                   drawCircle([xppp yppp],handles.playerColours(playerId,:),handles);
                                   handles.playerPieces(yppp,xppp) = playerId;
                               end

                               xppp = xppp+m;
                               yppp = yppp+n;

                           end
                       end
                   end
                end 

                if foundMove && doCheckOnly
                    break;
                end
            end

            if foundMove && doCheckOnly
                break;
            end
        end

        isValid = foundMove;
    end
end

%--------------------------------------------------------------------------
function applyMoveMex(sqrCoord,handles)

    MEX_CMD_APPLY_MOVE = 6;
    MEX_CMD_GET_SCORE  = 3;

    x = sqrCoord(1);
    y = sqrCoord(2);

    % apply the move
    [currentPlayerId,opponentId,isGameOver,isOppFinished,lastMoveValid, ...
        player1Score,player2Score,board] = mexReversiEngineInterface(MEX_CMD_APPLY_MOVE,y,x);
    
    % convert to unsigned ints
    isGameOver      = uint8(isGameOver);
    isOppFinished   = uint8(isOppFinished);
    lastMoveValid   = uint8(lastMoveValid);
    board           = uint8(board);
    player1Score    = uint8(player1Score);
    player2Score    = uint8(player2Score);

    if lastMoveValid

        % redraw the board

        for m=1:handles.boardSize
            for n=1:handles.boardSize
                playerId = board(m,n);
                if playerId~=0
                    drawCircle([n m],handles.playerColours(playerId,:),handles);
                end
            end
        end

        % set the scores
        set(handles.txtPlayer1Score,'String',['Player 1: ' num2str(player1Score)]);
        set(handles.txtPlayer2Score,'String',['Player 2: ' num2str(player2Score)]);

        if isGameOver
            % game is over
            if (player1Score+player2Score) < handles.boardSize*handles.boardSize
                msg = sprintf('There are no valid moves for Player 1\n');
                msg = {msg ; sprintf('There are no valid moves for Player 2\n')};
            else
                msg = {};
            end
            msg = [msg ; sprintf('GAME OVER!\n')];            
        elseif ~isOppFinished
            % opponent's turn
            msg = sprintf('It is Player %d''s move\n',currentPlayerId); 
        else
            % oppponent is finished
            msg = sprintf('There are no valid moves for Player %d\n',opponentId);
            msg = {msg ; sprintf('It is Player %d''s move\n',currentPlayerId)};
        end

        set(handles.txtStatus,'String',msg);

    else
        beep;
    end
end

%--------------------------------------------------------------------------
function status = doValidMovesExist(handles)

    status = false;

    [ycoords,xcoords] = find(handles.playerPieces==0);

    for k=1:length(ycoords)
        [status,~] = applyMove([xcoords(k) ycoords(k)],handles,true);
        if status
            break;
        end
    end
end

%--------------------------------------------------------------------------
% --- Executes on button press in btnNewGame.
function btnNewGame_Callback(~, ~, handles)
% hObject    handle to btnNewGame (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % since starting a new game, reformat and refresh the board
    formatBoard(handles);
    refreshBoard(handles);
end

%--------------------------------------------------------------------------
% --- Executes when user attempts to close figReversiApp.
function figReversiApp_CloseRequestFcn(hObject, ~, ~)
% hObject    handle to figReversiApp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    MEX_CMD_DESTROY_ENGINE = 8;

    % destroy the Reversi Engine
    mexReversiEngineInterface(MEX_CMD_DESTROY_ENGINE);

    % Hint: delete(hObject) closes the figure
    delete(hObject);
end
