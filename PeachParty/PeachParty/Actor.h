#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject{
    public:
        Actor(int name, int x, int y, StudentWorld* gameboard, int dir = right, int depth = 0, double size = 1.0);
        virtual void doSomething() {};//TODO: should this be a pure virtual function?
        virtual bool isInactive(); //TODO: for baddies, this should always return true (since you don't want to destroy the baddies)
        void setInactive();
        void setImpactable();
        bool isImpactable();
        StudentWorld* Board();
        
        
    private:
        bool inactive;
        bool impactable;
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
        void setTravelDirection(int d);
        int getTicks();
        bool getState();
        int getTravelDirection();
        bool validDirection();
        void changeDirections();
        
    private:
        int m_ticks_to_move;
        bool m_state;
        int m_traveling_direction;
};

class Avatar: public MovingActor{
    public:
        Avatar(int name, int x, int y, StudentWorld* gameboard, int playerNumber); //Name should be YOSHI or PEACH
        virtual void doSomething();
    private:
        int m_playerNumber;
    
};

class Square: public Actor{
    protected:
    //    const bool ALIVE = true;
    //    const bool DEAD = false;
    public:
        Square(int name, int x, int y, StudentWorld* gameboard, int dir = right, int dept = 1, double size = 1.0);
        virtual void doSomething();
    //    bool getAlive();
    //    void setAlive();
        
    private:
    //    bool m_isalive;
};

class CoinSquare: public Actor{
    protected:
    //    const bool ALIVE = true;
    //    const bool DEAD = false;
    public:
        CoinSquare(int name, int x, int y, StudentWorld* gameboard, int giveOrTake);
        virtual void doSomething();
    //    bool getAlive();
    //    void setAlive();
        
    private:
    //    bool m_isalive;
        int m_coinAmount;
};

#endif // ACTOR_H_
