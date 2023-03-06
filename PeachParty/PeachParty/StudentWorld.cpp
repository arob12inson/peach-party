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
            if (ge == Board::player){
                peach = new Avatar(IID_PEACH, width, height, this, 1);
                yoshi = new Avatar(IID_YOSHI, width, height, this, 2);
            }
            
        }
    }
    
    for (int i = 0; i < BOARD_HEIGHT; i++){
        for (int j = 0; j < BOARD_WIDTH; j++){
            Board::GridEntry ge = m_board.getContentsOf(i, j);
            int width = i*SPRITE_WIDTH;
            int height = j*SPRITE_HEIGHT;
            
            switch (ge){
                case Board::empty://TODO: Put cases for each board type
                    break;
                case Board::player://allocating players
                case Board::blue_coin_square:
                    m_actors.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, width, height, this, 3, peach, yoshi));
                    break;
                case Board::red_coin_square:
                    m_actors.push_back(new CoinSquare(IID_RED_COIN_SQUARE, width, height, this, -3, peach, yoshi));
                    break;
                case Board::up_dir_square:
                    m_actors.push_back(new Square(IID_DIR_SQUARE, width, height, this, peach, yoshi, 90));
                    break;
                case Board::down_dir_square:
                    m_actors.push_back(new Square(IID_DIR_SQUARE, width, height, this, peach, yoshi,  270));
                    break;
                case Board::left_dir_square:
                    m_actors.push_back(new Square(IID_DIR_SQUARE, width, height, this, peach, yoshi,  180));
                    break;
                case Board::right_dir_square:
                    m_actors.push_back(new Square(IID_DIR_SQUARE, width, height, this, peach, yoshi,  0));
                    break;
                case Board::event_square:
                    m_actors.push_back(new EventSquare(IID_EVENT_SQUARE, width, height, this, peach, yoshi));
                    break;
                case Board::bank_square:
                    m_actors.push_back(new BankSquare(IID_BANK_SQUARE, width, height, this, peach, yoshi));
                    break;
                case Board::star_square:
                    m_actors.push_back(new StarSquare(IID_STAR_SQUARE, width, height, this, peach, yoshi));
                    break;
                case Board::bowser:
                    m_actors.push_back(new Bowser(IID_BOWSER, width, height, this, peach, yoshi));
                    m_actors.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, width, height, this, 3, peach, yoshi));
                    break;
                case Board::boo:
                    m_actors.push_back(new Boo(IID_BOO, width, height, this, peach, yoshi));
                    m_actors.push_back(new CoinSquare(IID_BLUE_COIN_SQUARE, width, height, this, 3, peach, yoshi));
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
    
    string stat = "Peach: Coins– " + to_string(peach->getCoins()) + " Stars: " + to_string(peach->getStars());
    setGameStatText(stat);
    peach->doSomething();
    yoshi->doSomething();
    for (int it = 0; it < m_actors.size(); it++){
        if (m_actors[it]->isInactive() == false)
            m_actors[it]->doSomething();
    }
    
    //cleaning up deadCharacters
    
    for (int i = 0; i < m_actors.size(); i++){
        if (m_actors[i]->isInactive() == true){
            delete (m_actors[i]);
            m_actors.erase(m_actors.begin() + i);
            i--;
        }
    }
    if (timeRemaining() <= 0)
		return GWSTATUS_NOT_IMPLEMENTED;
    
	return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::cleanUp()
{
    delete peach;
    peach = nullptr;
    delete yoshi;
    yoshi = nullptr;
    vector<Actor*>::iterator it = m_actors.begin(); 
    for (; it != m_actors.end(); it++){
        delete *it;
        *it = nullptr;
    }
    
}
Board& StudentWorld::board()
{
    return m_board;
}
StudentWorld::~StudentWorld()
{
    cleanUp();
}
void StudentWorld::createDroppingSquare(int x, int y){
    Actor* ds = new DroppingsSquare(IID_DROPPING_SQUARE, x, y, this, peach, yoshi);
    for (int i = 0; i < m_actors.size(); i++){
        if (m_actors[i]->getX() == x && m_actors[i]->getY() == y){
            m_actors[i]->setInactive();

        }
    }
    m_actors.push_back(ds);
}
Vortex* StudentWorld::createVortex(Avatar* x){
    int x1 = x->getX();
    int y = x->getY();
    int dir = x->getTravelDirection();
    int switchDir = x->getTravelDirection();
    switch (switchDir) {
        case 180:
            x1-=2;
            break;
        case 0:
            x1+= 2;
            break;
        case 90:
            y+=2;
            break;
        case 270:
            y -=2;
            break;
        case -1:
            dir = 0;
            break;
        default:
            break;
    }
    
    Vortex* v = new Vortex(IID_VORTEX, x1, y, this, dir);
    m_actors.push_back(v);
    return v;
}

