#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int name, int x, int y): GraphObject(name, x, y){
//    inactive = false;
}

bool Actor::isInactive(){
    return inactive;
}

