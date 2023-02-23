#include "Actor.h"
#include "StudentWorld.h"
#include <ctime> //TODO: How do we generate a random number
#include <cstdlib>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor definitions
Actor::Actor(int name, int x, int y, StudentWorld* gameboard): GraphObject(name, x, y){
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
            if (getX()/16 == 0) //See if it is at the furthest left (protect against undefined behavior)
                return false;
            else if (Board()->board().getContentsOf(getX()/16 - 1, getY()/16) != Board::empty)// See if there is a valid spot in the board one to the left
                return true;
            else
                return false;
            break;
        case right:
            if (getX()/16 == 15)//See if it is at the furthest right (protect against undefined behavior)
                return false;
            else if (Board()->board().getContentsOf(getX()/16 + 1, getY()/16) != Board::empty)// See if there is a valid spot in the board one to the right
                return true;
            else
                return false;
            break;
        case up:
            if (getY()/16 == 15)//See if it is at the furthest up (protect against undefined behavior)
                return false;
            else if (Board()->board().getContentsOf(getX()/16, getY()/16 + 1) != Board::empty)// See if there is a valid spot in the board one above
                return true;
            else
                return false;
            break;
        case down:
            if (getY()/16 == 0)//See if it is at the furthest down (protect against undefined behavior)
                return false;
            else if (Board()->board().getContentsOf(getX()/16, getY()/16 - 1) != Board::empty)// See if there is a valid spot in the board one below (x-16)
                return true;
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
        if (Board()->getAction(m_playerNumber) != ACTION_NONE){
            if (Board()->getAction(m_playerNumber) == ACTION_ROLL){
                int die_roll = (rand() % 10) + 1;//generate random integer from 1-10 inclusive
                setTicks(die_roll * 8);
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
        
    }
}

