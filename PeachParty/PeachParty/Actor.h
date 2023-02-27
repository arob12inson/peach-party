#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject{
    protected:
        void setInactive(); // only doSomething should set it as inactive
        StudentWorld* Board(); // only doSomething needs to see the board
    public:
        Actor(int name, int x, int y, StudentWorld* gameboard, int dir = right, int depth = 0, double size = 1.0);
        virtual void doSomething() {};//TODO: should this be a pure virtual function?
        bool isInactive();//Student world will call isInactive to see if objects are active or not, so it will be a public function
    private:
        bool inactive;
        StudentWorld* m_gameboard;
};

class MovingActor: public Actor{
    protected:
        const bool WAITING = 0;
        const bool WALKING = 1;
        void setTicks(int ticks);
        void setTravelDirection(int d);
        int getTicks();
        int getTravelDirection();
        bool validDirection();
        void changeDirections();
        void setState(bool state);
        bool getState();
    public:
        MovingActor(int name, int x, int y, StudentWorld* gameboard);
        virtual void doSomething() {};
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
    public:
        Square(int name, int x, int y, StudentWorld* gameboard, int dir = right, int dept = 1, double size = 1.0);
        virtual void doSomething();
    private:
};

class CoinSquare: public Actor{
    public:
        CoinSquare(int name, int x, int y, StudentWorld* gameboard, int giveOrTake);
        virtual void doSomething();
    private:
        int m_coinAmount;
};

#endif // ACTOR_H_
