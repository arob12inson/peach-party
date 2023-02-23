#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject{
    public:
        Actor(int name, int x, int y, StudentWorld* gameboard);
        virtual void doSomething() {};//TODO: should this be a pure virtual function?
        bool isInactive();
        void setInactive();
        StudentWorld* getBoard();
        
        
    private:
        bool inactive;
        StudentWorld* m_gameboard;
        
};

class MovingActor: public Actor{
protected:
    const bool WAITING = 0;
    const bool WALKING = 1;
public:
    MovingActor(int name, int x, int y, StudentWorld* gameboard);
    virtual void doSomething() {};
    void setTicks(int ticks);
    void setState(bool state);
    int getTicks();
    bool getState();
    
private:
    int m_ticks_to_move;
    bool m_state;
};

class Avatar: public MovingActor{
//    const bool WAITING_TO_ROLL = 0;
//    const bool WALKING = 1;
    public:
        Avatar(int name, int x, int y, StudentWorld* gameboard, int playerNumber); //Name should be YOSHI or PEACH
        virtual void doSomething();
    private:
        int m_playerNumber;
    
};

#endif // ACTOR_H_
