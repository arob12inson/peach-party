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

StudentWorld* Actor::getBoard(){
    return m_gameboard;
}


//MovingActor Definition
MovingActor::MovingActor(int name, int x, int y, StudentWorld* gameboard):
Actor(name, x, y, gameboard)
{
    m_state = WAITING;
    m_ticks_to_move = 0;
}
void MovingActor::setTicks(int ticks){
    m_ticks_to_move = ticks;
}
void MovingActor::setState(bool state){
    m_state = state;
}
int MovingActor::getTicks(){
    return m_ticks_to_move;
}
bool MovingActor::getState(){
    return m_state;
}

//Avatar Definition
Avatar::Avatar(int name, int x, int y, StudentWorld* gameboard, int playerNumber):
MovingActor(name, x, y, gameboard)
{
    m_playerNumber = playerNumber;
    setState(WAITING);
    setTicks(0);
}

void Avatar::doSomething(){//TODO: Do i need the virtual keyword in here?
    if (getState() == WAITING){
        if (getBoard()->getAction(m_playerNumber) != ACTION_NONE){
            if (getBoard()->getAction(m_playerNumber) == ACTION_ROLL){
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

