#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int name, int x, int y): GraphObject(name, x, y){
    inactive = false;
}

bool Actor::isInactive(){
    return inactive;
}

void Actor::setInactive(){
    inactive = true;
}



Avatar::Avatar(int name, int x, int y, int playerNumber):
Actor(name, x, y)
{
    m_playerNumber = playerNumber;
    m_state = WAITING_TO_ROLL;
    m_ticks_to_move = 0;
}



