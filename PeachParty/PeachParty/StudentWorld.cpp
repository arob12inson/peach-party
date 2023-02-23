#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"//TODO: Should I include Actor.h here?
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    peach = nullptr;
    yoshi = nullptr;
}

int StudentWorld::init()
{
    //load the correct board
    int file_number = getBoardNumber();
    string board_file = assetPath() + "board0" + to_string(file_number) + ".txt";
    Board::LoadResult result = m_board.loadBoard(board_file);
    
    //throw error if board is invalid
    if (result == Board::load_fail_file_not_found || result == Board::load_fail_bad_format){
        return GWSTATUS_BOARD_ERROR;
    }
    
    //initialize data from board
    for (int i = 0; i < BOARD_HEIGHT; i++){
        for (int j = 0; j < BOARD_WIDTH; j++){
            Board::GridEntry ge = m_board.getContentsOf(i, j);
            int width = i*SPRITE_WIDTH;
            int height = j*SPRITE_HEIGHT;
            
            switch (ge){
                case Board::empty://TODO: Put cases for each board type
                    break;
                case Board::player://allocating players
                    peach = new Avatar(IID_PEACH, width, height, this, 1);
                    yoshi = new Avatar(IID_YOSHI, width, height, this, 2);
                    break;
                    //TODO: Add coinSquare
                default:
                    m_actors.push_back(new Actor(IID_BLUE_COIN_SQUARE, width, height, this));

            }

        }
    }
//    m_board->loadBoard(assetPath());
//    for (int i = 0; i < BOARD_HEIGHT; i++){
//        for (int j = 0; j < BOARD_WIDTH; j++){
//            if (m_board->getContentsOf(i, j) != Board::empty){
//
//            }
//        }
//    }
    startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    setGameStatText("Game will end in a few seconds");
    
    if (timeRemaining() <= 0)
		return GWSTATUS_NOT_IMPLEMENTED;
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}
