#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
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



Avatar::Avatar(int name, int x, int y, StudentWorld* gameboard, int playerNumber):
Actor(name, x, y, gameboard)
{
    m_playerNumber = playerNumber;
    m_state = WAITING_TO_ROLL;
    m_ticks_to_move = 0;
}

void Avatar::doSomething(){//TODO: Do i need the virtual keyword in here?
    if (m_state == WAITING_TO_ROLL){
        
    }
}

