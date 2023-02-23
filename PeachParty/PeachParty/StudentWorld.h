#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <vector>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Board& board();//will this cause performance issues? Pointer vs reference?
    
private:
    Board m_board;
    std::vector<Actor*> m_actors;
    Actor* peach;
    Actor* yoshi;
    
    
};

#endif // STUDENTWORLD_H_
