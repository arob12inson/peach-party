#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject{
    protected:
        virtual bool isInactive(); //TODO: for baddies, this should always return true (since you don't want to destroy the baddies)
        void setInactive();
        void setImpactable();
        bool isImpactable();
        StudentWorld* Board();
        
    public:
        Actor(int name, int x, int y, StudentWorld* gameboard, int dir = right, int depth = 0, double size = 1.0);
        virtual void doSomething() {};//TODO: should this be a pure virtual function?
    virtual ~Actor() {};
    private:
        bool inactive;
        bool impactable;
        StudentWorld* m_gameboard;
        
};

class MovingActor: public Actor{
    protected:
        const bool WAITING = 0;
        const bool WALKING = 1;
        void setTicks(int ticks);
        void setState(bool state);
        void setTravelDirection(int d);
        int getTicks();
        int getTravelDirection();
        bool validDirection();
        void changeDirections();
        void changeDirections(int d);
        bool isOnTopOfSquare();
        bool isDirectionalSquare(int& dir);
        bool isAtFork();
        bool isBacktracking(int dir);
        void makeFirstMove();
    public:
        MovingActor(int name, int x, int y, StudentWorld* gameboard);
        virtual void doSomething() {};
        bool getState();
    private:
        int m_ticks_to_move;
        bool m_state;
        bool first_move;
        int m_traveling_direction;
};

class Avatar: public MovingActor{
    
    public:
        Avatar(int name, int x, int y, StudentWorld* gameboard, int playerNumber); //Name should be YOSHI or PEACH
        virtual void doSomething();
        int convertAction(int keyAction);
        void addCoins(int coins);
    private:
        int m_playerNumber;
        int m_coins;
    
};

class Square: public Actor{
    protected:
        virtual void peachPassesSquare() {};
        virtual void peachLandsOnSquare() {};
        virtual void peachLeavesSquare() {};
        virtual void yoshiPassesSquare() {};
        virtual void yoshiLandsOnSquare() {};
        virtual void yoshiLeavesSquare() {};
        Avatar* peach();
        Avatar* yoshi();
    public:
        Square(int name, int x, int y, StudentWorld* gameboard, Avatar* peach, Avatar* yoshi, int dir = right, int dept = 1, double size = 1.0);
        virtual void doSomething();
    private:
        Avatar* m_peach;
        Avatar* m_yoshi;
        bool peachOnSquare;
        bool yoshiOnSquare;
};

class CoinSquare: public Square{
    protected:
        virtual void peachLandsOnSquare();
        virtual void yoshiLandsOnSquare();
    public:
        CoinSquare(int name, int x, int y, StudentWorld* gameboard, int giveOrTake, Avatar* peach, Avatar* yoshi);
        
    private:
        int m_coinAmount;
};

#endif // ACTOR_H_
