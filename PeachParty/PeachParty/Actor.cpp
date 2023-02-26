#include "Actor.h"
#include "StudentWorld.h"
#include <list>


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor definitions
Actor::Actor(int name, int x, int y, StudentWorld* gameboard, int dir, int depth, double size): GraphObject(name, x, y, dir, depth, size){
    inactive = false;
    m_gameboard = gameboard;
    impactable = false;
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
void Actor::setImpactable(){
    impactable = true;
}
bool Actor::isImpactable(){
    return impactable;
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
void MovingActor::changeDirections(int d){
    m_traveling_direction = d;
}
bool MovingActor::isOnTopOfSquare(){
    return (getX() % 16 == 0 && getY() % 16 == 0);
}
bool MovingActor::isDirectionalSquare(int& dir){
    if (Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) == Board::up_dir_square){
        dir = up;
        return true;
    }
    else if (Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) == Board::down_dir_square){
        dir = down;
        return true;
    }
    else if (Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) == Board::right_dir_square){
        dir = right;
        return true;
    }
    else if (Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) == Board::left_dir_square){
        dir = left;
        return true;
    }
    return false;
}
bool MovingActor::isAtFork(){
    if (!isOnTopOfSquare()){
        return false;
    }
    
    int dir = m_traveling_direction;
    int numValidDirections = 0;
    
    std::list<int> directions{right, up, left, down};
    if (m_traveling_direction >= right || m_traveling_direction < left){
        directions.remove(m_traveling_direction + 180);
    }
    else {
        directions.remove(m_traveling_direction - 180);
    }
    
    for (std::list<int>::iterator it = directions.begin(); it != directions.end(); it++){
        m_traveling_direction = *it;
        if (validDirection()){
            numValidDirections++;
        }
        m_traveling_direction = dir;
    }
    m_traveling_direction = dir;
    return true;
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
        int x = 0;
        if (isOnTopOfSquare() && isDirectionalSquare(x)){//TODO: make the Directional Square Class handle this (Give it a pointer to peach and yoshi )
            changeDirections(x);//TODO: change to setTravelDirection- don't overcomplicate it
            if(x == left){
                setDirection(x);
            }
            
        }
//        else if (isOnTopOfSquare() && validDirection()){ //Else if the Avatar is directly on top of a square at a fork (with multiple directions where it could move next)
//            int action = Board()->getAction(m_playerNumber);
//            int td = getTravelDirection();// What to do if you're at a T intersection?
//            if (action == ACTION_UP){
//                if (getTravelDirection() == up){
//
//                }
//            } else if (action == ACTION_DOWN){
//                if (getTravelDirection() == down){
//
//                }
//            } else if (action == ACTION_LEFT){
//                if (getTravelDirection() == left){
//
//                }
//            } else if (action == ACTION_RIGHT){
//                if (getTravelDirection() == right){
//
//                }
//            }
//
//        }
        else if (validDirection() == false){//Else if the Avatar can't continue moving forward in its current direction
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


