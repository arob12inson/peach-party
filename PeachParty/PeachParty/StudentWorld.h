#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <vector>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Avatar;
class Vortex;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Board& board();//will this cause performance issues? Pointer vs reference?
    virtual ~StudentWorld();
    void createDroppingSquare(int x, int y);
    Vortex* createVortex(Avatar* x);
    bool determineImpact(Vortex* v);
//    int withdrawCoins(){temp x = bankAmount; bankAmount = 0; return x;}
//    void depositCoins(int x){return bankAmount;}
    
    
private:
    Board m_board;
    std::vector<Actor*> m_actors;
    Avatar* peach;
    Avatar* yoshi;
//    int bankAmount; 
    
    
};

#endif // STUDENTWORLD_H_
