#include "Actor.h"
#include "StudentWorld.h"
#include <ctime> //TODO: How do we generate a random number
#include <cstdlib>

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

StudentWorld* Actor::getBoard(){
    return m_gameboard;
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
        if (getBoard()->getAction(m_playerNumber) != ACTION_NONE){
            if (getBoard()->getAction(m_playerNumber) == ACTION_ROLL){
                int die_roll = (rand() % 10) + 1;//generate random integer from 1-10 inclusive
                m_ticks_to_move = die_roll * 8;
                m_state = WALKING;
            }
            else{
                return;
            }
        }
        else{
            return;
        }
    }
    
    if (m_state == WALKING){
        
    }
}

