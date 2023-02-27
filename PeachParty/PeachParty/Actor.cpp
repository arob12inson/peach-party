#include "Actor.h"
#include "StudentWorld.h"
//#include <ctime> //TODO: How do we generate a random number
//#include <cstdlib>
#include <cassert>


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor definitions
Actor::Actor(int name, int x, int y, StudentWorld* gameboard, int dir, int depth, double size): GraphObject(name, x, y, dir, depth, size){
    inactive = false;
    m_gameboard = gameboard;
}
bool Actor::isInactive(){
    return inactive;
}
void Actor::setInactive(){
    inactive = true;
}
StudentWorld* Actor::Board(){
    return m_gameboard;
}


//MovingActor Definition
MovingActor::MovingActor(int name, int x, int y, StudentWorld* gameboard):
Actor(name, x, y, gameboard)
{
    m_state = WAITING;
    m_ticks_to_move = 0;
    m_traveling_direction = 0;
}
void MovingActor::setTicks(int ticks){
    m_ticks_to_move = ticks;
}
void MovingActor::setState(bool state){
    m_state = state;
}
void MovingActor::setTravelDirection(int d){
    m_traveling_direction = d;
}
int MovingActor::getTicks(){
    return m_ticks_to_move;
}
bool MovingActor::getState(){
    return m_state;
}
int MovingActor::getTravelDirection(){
    return m_traveling_direction;
}
bool MovingActor::validDirection(){
    
    switch (m_traveling_direction){//TODO: Update with constants instead of 16
        case left: // is Y up and down?
            if (getX() == 0) //See if it is at the furthest left (protect against undefined behavior)
            {
                return false;
            }
            else if (getX() % SPRITE_WIDTH == 0 && Board()->board().getContentsOf((getX()-SPRITE_WIDTH)/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) != Board::empty){ //if is at the very left of a square, check square after
                return true;
            }
            else if (getX() % SPRITE_WIDTH != 0 && Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) != Board::empty)// See if there is a valid spot in the board one to the left
            {
                return true;// if the first condition is true, the second one should always be true, because it is check if the square it is on is not empty. But hey it doesn't hurt to double check right?
            }
            else
            {
                return false;
            }
            break;
        case right:
            if (getX()/SPRITE_WIDTH == SPRITE_WIDTH - 1)//See if it is at the furthest right (protect against undefined behavior)
                return false;
            else if (Board()->board().getContentsOf(getX()/SPRITE_WIDTH + 1, getY()/SPRITE_HEIGHT) != Board::empty)// See if there is a valid spot in the board one to the right
                return true;
            else
                return false;
            break;
        case up:
            if (getY()/SPRITE_HEIGHT == SPRITE_HEIGHT - 1)//See if it is at the furthest up (protect against undefined behavior)
                return false;
            else if (Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT + 1) != Board::empty)// See if there is a valid spot in the board one above
                return true;
            else
                return false;
            break;
        case down:
            if (getY() == 0)//See if it is at the furthest down (protect against undefined behavior
                return false;
            else if (getY() % SPRITE_HEIGHT == 0 && Board()->board().getContentsOf(getX()/SPRITE_WIDTH, (getY()-SPRITE_HEIGHT)/SPRITE_HEIGHT) != Board::empty){
                return true;
            }
            else if (getY() % SPRITE_HEIGHT != 0 && Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) != Board::empty)// See if there is a valid spot in the board one to the left
            {
                return true;
            }
            else
                
                return false;
            break;
    }
    
    
    return true;//Hypothetically, this should never run
}
void MovingActor::changeDirections(){ //TODO: Overload this function with one that takes a parameter of where to go
    if (m_traveling_direction == left || m_traveling_direction == right){
        setDirection(right);
        m_traveling_direction = up;
        if (validDirection() == false){
            m_traveling_direction = down;
        }
    }
    else if (m_traveling_direction == up || m_traveling_direction == down){
        setDirection(right);
        m_traveling_direction = right;
        if (validDirection() == false){
            setDirection(left);
            m_traveling_direction = left;
        }
    }
}

//Avatar Definition
Avatar::Avatar(int name, int x, int y, StudentWorld* gameboard, int playerNumber):
MovingActor(name, x, y, gameboard)
{
    m_playerNumber = playerNumber;
    setState(WAITING);
    setTicks(0);
}

void Avatar::doSomething(){
    if (getState() == WAITING){
        int action = Board()->getAction(m_playerNumber);
        if (action != ACTION_NONE){
            if (action == ACTION_ROLL){
                setTicks(randInt(1, 10) * 8);
                setState(WALKING);
            }
            else{
                return;
            }
        }
        else{
            return;
        }
    }
    
    if (getState() == WALKING){
        if (validDirection() == false){
//            std::cerr << "I returned false at " << std::to_string(getX()) << ", " << std::to_string(getY()) << std::endl;
            changeDirections();
        }
        switch (getTravelDirection()) {
            case left:
                moveTo(getX()-2, getY());//Move two pixels in the walk direction
                break;
            case right:
                moveTo(getX()+2, getY());//Move two pixels in the walk direction
                break;
            case up:
                moveTo(getX(), getY()+2);//Move two pixels in the walk direction

                break;
            case down:
                moveTo(getX(), getY()-2);//Move two pixels in the walk direction
                break;
        }
        setTicks(getTicks()-1);//Decrement the ticks_to_move count by 1.
        if (getTicks() == 0)//If ticks_to_move is 0 then:
            setState(WAITING);//Change the Avatar's state to the waiting to roll state.
    }
}

//SquareClass
Square::Square(int name, int x, int y, StudentWorld* gameboard, int dir, int depth, double size) : Actor(name, x, y, gameboard, dir, depth, size){
    
}
void Square::doSomething(){//TODO: eventually declare as pure virtual
    
}

//CoinSquare Class
CoinSquare::CoinSquare(int name, int x, int y, StudentWorld* gameboard, int giveOrTake):
Actor(name, x, y, gameboard, right, 1)
{
//    m_isAlive = ALIVE;//TODO: how does this overlap with the active/inactive function for base Actor class?
    
    m_coinAmount = giveOrTake;
}

void CoinSquare::doSomething(){//TODO: eventually declare as pure virtual
    if (isInactive() == true){
        return;
    }
    
}

//bool Square::getAlive(){
//    return m_isAlive;
//}
//
//void Square::setAlive(){
//    m_isAlive = !m_isAlive;
//}
