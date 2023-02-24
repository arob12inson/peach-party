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
                    m_actors.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, width, height, this, 3));
                    break;
                case Board::blue_coin_square:
                    m_actors.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, width, height, this, 3));
                    break;
                case Board::red_coin_square:
                    m_actors.push_back(new CoinSquare(IID_RED_COIN_SQUARE, width, height, this, -3));
                    break;
                case Board::up_dir_square:
                    m_actors.push_back(new Square(IID_DIR_SQUARE, width, height, this, 90));
                    break;
                case Board::down_dir_square:
                    m_actors.push_back(new Square(IID_DIR_SQUARE, width, height, this, 270));
                    break;
                case Board::left_dir_square:
                    m_actors.push_back(new Square(IID_DIR_SQUARE, width, height, this, 180));
                    break;
                case Board::right_dir_square:
                    m_actors.push_back(new Square(IID_DIR_SQUARE, width, height, this, 0));
                    break;
                case Board::event_square:
                    m_actors.push_back(new Square(IID_EVENT_SQUARE, width, height, this));
                    break;
                case Board::bank_square:
                    m_actors.push_back(new Square(IID_BANK_SQUARE, width, height, this));
                    break;
                case Board::star_square:
                    m_actors.push_back(new Square(IID_STAR_SQUARE, width, height, this));
                    break;
                case Board::bowser:
                    m_actors.push_back(new MovingActor(IID_BOWSER, width, height, this));
                    m_actors.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, width, height, this, 3));
                    break;
                case Board::boo:
                    m_actors.push_back(new MovingActor(IID_BOO, width, height, this));
                    m_actors.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, width, height, this, 3));
                    break;
            }
        }
    }
    startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    setGameStatText("Game will end in a few seconds");
    
    peach->doSomething();
    
    if (timeRemaining() <= 0)
		return GWSTATUS_NOT_IMPLEMENTED;
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete peach;
    delete yoshi;
    vector<Actor*>::iterator it = m_actors.begin(); //TODO: Why don't you need an std::vector?
    for (; it != m_actors.end(); it++){
        delete *it;
    }
    
}

Board& StudentWorld::board(){
    return m_board;
}

void StudentWorld::~StudentWorld(){
    cleanUp();
}

