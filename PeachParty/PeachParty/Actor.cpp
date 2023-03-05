#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor definitions
Actor::Actor(int name, int x, int y, StudentWorld* gameboard, int dir, int depth, double size): GraphObject(name, x, y, dir, depth, size){
    inactive = false;
    m_gameboard = gameboard;
    impactable = false;
}
bool Actor::isInactive(){
    return inactive;
}
void Actor::setInactive(){
    inactive = true;
}
StudentWorld* Actor::Board(){
    return m_gameboard;
}
void Actor::setImpactable(){
    impactable = true;
}
bool Actor::isImpactable(){
    return impactable;
}

//MovingActor Definition
MovingActor::MovingActor(int name, int x, int y, StudentWorld* gameboard):
Actor(name, x, y, gameboard){
    m_state = WAITING;
    m_ticks_to_move = 0;
    m_traveling_direction = 0;
    first_move = true;
}
void MovingActor::setTicks(int ticks){
    m_ticks_to_move = ticks;
}
void MovingActor::setState(bool state){
    m_state = state;
}
void MovingActor::setTravelDirection(int d){
    m_traveling_direction = d;
}
int MovingActor::getTicks(){
    return m_ticks_to_move;
}
bool MovingActor::getState(){
    return m_state;
}
int MovingActor::getTravelDirection(){
    return m_traveling_direction;
}
bool MovingActor::validDirection(){
    switch (m_traveling_direction){
        case left: // is Y up and down?
            if (getX() == 0) //See if it is at the furthest left (protect against undefined behavior)
            {
                return false;
            }
            else if (getX() % SPRITE_WIDTH == 0 && Board()->board().getContentsOf((getX()-SPRITE_WIDTH)/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) != Board::empty){ //if is at the very left of a square, check square after
                return true;
            }
            else if (getX() % SPRITE_WIDTH != 0 && Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) != Board::empty)// See if there is a valid spot in the board one to the left
            {
                return true;// if the first condition is true, the second one should always be true, because it is check if the square it is on is not empty. But hey it doesn't hurt to double check right?
            }
            else
            {
                return false;
            }
            break;
        case right:
            if (getX()/SPRITE_WIDTH == SPRITE_WIDTH - 1)//See if it is at the furthest right (protect against undefined behavior)
                return false;
            else if (Board()->board().getContentsOf(getX()/SPRITE_WIDTH + 1, getY()/SPRITE_HEIGHT) != Board::empty)// See if there is a valid spot in the board one to the right
                return true;
            else
                return false;
            break;
        case up:
            if (getY()/SPRITE_HEIGHT == SPRITE_HEIGHT - 1)//See if it is at the furthest up (protect against undefined behavior)
                return false;
            else if (Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT + 1) != Board::empty)// See if there is a valid spot in the board one above
                return true;
            else
                return false;
            break;
        case down:
            if (getY() == 0)//See if it is at the furthest down (protect against undefined behavior
                return false;
            else if (getY() % SPRITE_HEIGHT == 0 && Board()->board().getContentsOf(getX()/SPRITE_WIDTH, (getY()-SPRITE_HEIGHT)/SPRITE_HEIGHT) != Board::empty){
                return true;
            }
            else if (getY() % SPRITE_HEIGHT != 0 && Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) != Board::empty)// See if there is a valid spot in the board one to the left
            {
                return true;
            }
            else
                
                return false;
            break;
        case -1:
            return false;
    }
    return true;//Hypothetically, this should never run
}
void MovingActor::changeDirections(){
    if (m_traveling_direction == left || m_traveling_direction == right){
        setDirection(right);
        m_traveling_direction = up;
        if (validDirection() == false){
            m_traveling_direction = down;
        }
    }
    else if (m_traveling_direction == up || m_traveling_direction == down){
        setDirection(right);
        m_traveling_direction = right;
        if (validDirection() == false){
            setDirection(left);
            m_traveling_direction = left;
        }
    }
}
void MovingActor::changeDirections(int d){
    m_traveling_direction = d;
}// Repetitive function with setTravelDirection
bool MovingActor::isOnTopOfSquare(){
    return (getX() % 16 == 0 && getY() % 16 == 0);
}
bool MovingActor::isDirectionalSquare(int& dir){
    if (Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) == Board::up_dir_square){
        dir = up;
        return true;
    }
    else if (Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) == Board::down_dir_square){
        dir = down;
        return true;
    }
    else if (Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) == Board::right_dir_square){
        dir = right;
        return true;
    }
    else if (Board()->board().getContentsOf(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT) == Board::left_dir_square){
        dir = left;
        return true;
    }
    return false;
}
bool MovingActor::isAtFork(){
    if (!isOnTopOfSquare() || first_move == true){
        return false;
    }
    int dir = m_traveling_direction;
    int numValidDirections = 0;
    int directions [4] = {right, up, left, down};
    for (int i = 0; i < 4; i++){
        if(isBacktracking(directions[i])){
            directions[i] = -1; // Remove the backtracking direction
        }
    }
    for (int i = 0; i < 4; i++){
        if (directions[i] != -1){
            m_traveling_direction = directions[i];
            if (validDirection()){
                numValidDirections++;
            }
            m_traveling_direction = dir;
        }
    }
    m_traveling_direction = dir;
    return (numValidDirections >= 2);
}
bool MovingActor::isBacktracking(int dir){ // Sees if a given direction is the opposite of the current direction
    if (m_traveling_direction == right || m_traveling_direction == up){
        return (m_traveling_direction + 180 == dir);
    }
    else {
        return (m_traveling_direction - 180 == dir);
    }
}
void MovingActor::makeFirstMove(){ // prevents first move from being treated like a fork
    first_move = 0;
}

//Avatar Definition
Avatar::Avatar(int name, int x, int y, StudentWorld* gameboard, int playerNumber):
MovingActor(name, x, y, gameboard){
    m_playerNumber = playerNumber;
    setState(WAITING);
    setTicks(0);
    m_coins = 0;
    m_stars = 0;
    can_be_teleported = true;
}
void Avatar::doSomething(){
    
    if (getState() == WAITING){
        int action = Board()->getAction(m_playerNumber);
        if (action != ACTION_NONE){
            if (action == ACTION_ROLL){
                setTicks(randInt(1, 10) * 8);
                setState(WALKING);
            }
            else{
                return;
            }
        }
        else{
            return;
        }
    }
    if (getState() == WALKING){
        int x = 0;
        if (isOnTopOfSquare() && isDirectionalSquare(x)){
            changeDirections(x);//TODO: change to setTravelDirection- don't overcomplicate it
            if(x == left){
                setDirection(x);
            }
            else {
                setDirection(right);
            }
            
        }
        else if (isAtFork() && getTravelDirection() != JUST_TELEPORTED){ //Else if the Avatar is directly on top of a square at a fork (with multiple directions where it could move next)

            int action = convertAction(Board()->getAction(m_playerNumber));
            int td = getTravelDirection();// What to do if you're at a T intersection?
            if (action == -1){
                return;
            }
            else if (!isBacktracking(action)){
                setTravelDirection(action);
                if (!validDirection()){
                    setTravelDirection(td); //Any problems when td
                    return;
                }
                if (getTravelDirection() == left){
                    setDirection(left);
                }
                else {
                    setDirection(right);
                }
            }
            else if (isBacktracking(action)){
                return;
            }
        }
        else if (validDirection() == false){//Else if the Avatar can't continue moving forward in its current direction
            changeDirections();
        }
        switch (getTravelDirection()) {
            case left:
                moveTo(getX()-2, getY());//Move two pixels in the walk direction
                break;
            case right:
                moveTo(getX()+2, getY());//Move two pixels in the walk direction
                break;
            case up:
                moveTo(getX(), getY()+2);//Move two pixels in the walk direction

                break;
            case down:
                moveTo(getX(), getY()-2);//Move two pixels in the walk direction
                break;
        }
        can_be_teleported = true; // When actor moves from the square it was on, it should be able to teleport again (Only relevant when traveling away from an event square
        setTicks(getTicks()-1);//Decrement the ticks_to_move count by 1.
        if (getTicks() == 0)//If ticks_to_move is 0 then:
            setState(WAITING);//Change the Avatar's state to the waiting to roll state.
    }
    makeFirstMove();//tell program that avatar isn't at first move (and therefore should follow fork logic);
}
bool Avatar::canBeTeleported(){
    return can_be_teleported;
};
void Avatar::changeTeleportationStatus(bool value){
    can_be_teleported = value;
}
int Avatar::convertAction(int keyAction){ //converts keyboard press to its corresponding n/e/s/w counterpart returnts -1 if didn't press a direction
    if (keyAction == ACTION_DOWN){
        return down;
    }
    else if (keyAction == ACTION_RIGHT){
        return right;
    }
    else if (keyAction == ACTION_LEFT){
        return left;
    }
    else if (keyAction == ACTION_UP){
        return up;
    }
    return -1;
}
void Avatar::addCoins(int coins){
    m_coins = (m_coins + coins >= 0) ? m_coins + coins : m_coins;
}
void Avatar::addStars(){
    m_stars++;
}
void Avatar::subtractStars(){
    m_stars--;
}
int Avatar::getCoins(){
    return m_coins;
}
int Avatar::getStars(){
    return m_stars;
}
void Avatar::teleport(int x, int y){
    setTravelDirection(JUST_TELEPORTED);
    moveTo(x, y);
} // TODO: what happens when it teleports onto a square that demands an action? Should it still have the same action?
//Solution: SInce the player probably can't get a move in within one tick, this will rarely be the case
void Avatar::swap(Avatar* other){ //TODO: Make sure that it doesn't reactivate square when teleported
    if (can_be_teleported){
        Board()->playSound(SOUND_PLAYER_TELEPORT);
        
        
        
        int otherX = other->getX();
        int otherY = other->getY();
        int otherDir = other->getDirection();
        int otherWalkDir = other->getTravelDirection();
        int otherTicks = other->getTicks();
        int otherState = (getTicks() == 0) ? WAITING : WALKING;
        
        other->moveTo(getX(), getY());
        other->setTravelDirection(getTravelDirection());
        other->setDirection(getDirection());
        other->setState(getTicks() > 0 );
        other->setTicks(getTicks());
        
        moveTo(otherX, otherY);
        setTravelDirection(otherWalkDir);
        setDirection(otherDir);
        setState(otherState);
        setTicks(otherTicks);
        

        
        can_be_teleported = false;
        other->changeTeleportationStatus(false);
        std::cerr << "Peach got teleported" << std::endl;
    }

    
    // when teleported
//        can_be_teleported = false; // breaks because at the very next move call, can_be_teleported is changed true
//        other->changeTeleportationStatus();
    
} //TODO: problem– if peach and yoshi reach event square at the same time, then only peach's will run

//SquareClass
Square::Square(int name, int x, int y, StudentWorld* gameboard, Avatar* peach, Avatar* yoshi, int dir, int depth, double size) : Actor(name, x, y, gameboard, dir, depth, size){
    m_peach = peach;
    m_yoshi = yoshi;
    peachOnSquare = false;
    yoshiOnSquare = false;
}
void Square::doSomething(){//TODO: eventually declare as pure virtual
    if (!peachOnSquare && (m_peach->getX() == getX() && m_peach->getY() == getY() && m_peach->getState() == false)){ //If Peach
        peachOnSquare = true;
        peachLandsOnSquare();
    }
    else if (!peachOnSquare && (m_peach->getX() == getX() && m_peach->getY() == getY() && m_peach->getState() == true)){// if peach walks over the square
        peachPassesSquare();
    }
    else if (peachOnSquare && (m_peach->getX() != getX() || m_peach->getY() != getY())){ // If peach leaves the square
        peachOnSquare = false;
        peachLeavesSquare();
    }
    if (!yoshiOnSquare && (m_yoshi->getX() == getX() && m_yoshi->getY() == getY() && m_yoshi->getState() == false)){ // if yoshi lands on the square
        yoshiOnSquare = true;
        yoshiLandsOnSquare();
    }
    else if (!yoshiOnSquare && (m_yoshi->getX() == getX() && m_yoshi->getY() == getY() && m_yoshi->getState() == true)){ // if yoshi walks over the square
        yoshiPassesSquare();
    }
    else if (yoshiOnSquare && (m_yoshi->getX() != getX() || m_yoshi->getY() != getY())){ // if yoshi leaves the square
        yoshiOnSquare = false;
        yoshiLandsOnSquare();
    }
}
Avatar* Square::peach(){
    return m_peach;
}
Avatar* Square::yoshi(){
    return m_yoshi;
}
void Square::setPeachOnSquare(bool value){
    peachOnSquare = value;
}
void Square::setYoshiOnSquare(bool value){
    yoshiOnSquare = value;
}

//CoinSquare Class
CoinSquare::CoinSquare(int name, int x, int y, StudentWorld* gameboard, int giveOrTake, Avatar* peach, Avatar* yoshi):
Square(name, x, y, gameboard, peach, yoshi, right, 1){
    m_coinAmount = giveOrTake;
}
void CoinSquare::peachLandsOnSquare() {
    peach()->addCoins(m_coinAmount);
    if (m_coinAmount == 3){
        Board()->playSound(SOUND_GIVE_COIN);
    }
    else{
        Board()->playSound(SOUND_TAKE_COIN);
    }
};
void CoinSquare::yoshiLandsOnSquare(){
    yoshi()->addCoins(m_coinAmount);
    if (m_coinAmount == 3){
        Board()->playSound(SOUND_GIVE_COIN);
    }
    else{
        Board()->playSound(SOUND_TAKE_COIN);
    }
}

//StarSquareImplementations:
StarSquare::StarSquare(int name, int x, int y, StudentWorld* gameboard, Avatar* peach, Avatar* yoshi) : Square(name, x, y, gameboard, peach, yoshi, right, 1) {
    
}
void StarSquare::peachLandsOnSquare(){
    if (peach()->getCoins() >= 20){
        peach()->addCoins(-20);
        peach()->addStars();
        Board()->playSound(SOUND_GIVE_STAR);
    }
}
void StarSquare::peachPassesSquare(){
    peachLandsOnSquare();
}
void StarSquare::yoshiLandsOnSquare(){
    if (yoshi()->getCoins() >= 20){
        yoshi()->addCoins(-20);
        yoshi()->addStars();
        Board()->playSound(SOUND_GIVE_STAR);
    }
}
void StarSquare::yoshiPassesSquare(){
    yoshiLandsOnSquare();
}

//DroppingsSquare Implementations: TODO: Need to see if it works properly
DroppingsSquare::DroppingsSquare(int name, int x, int y, StudentWorld* gameboard, Avatar* peach, Avatar* yoshi) : Square(name, x, y, gameboard, peach, yoshi, right, 1){
}
void DroppingsSquare::peachLandsOnSquare(){
    int option = 1;
    Board()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    if (peach()->getStars() < 1){
        option = randInt(1, 2);
    }
    
    switch (option) {
        case 1: // Remove 10 coins
            peach()->addCoins(-10);
            break;
        case 2:
            peach()->subtractStars();
            break;
    }
}
void DroppingsSquare::yoshiLandsOnSquare(){
    int option = 1;
    Board()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
    if (yoshi()->getStars() < 1){
        option = randInt(1, 2);
    }
    switch (option) {
        case 1: // Remove 10 coins
            yoshi()->addCoins(-10);
            break;
        case 2:
            yoshi()->subtractStars();
            break;
    }
}

//BankSquare Implementations:
BankSquare::BankSquare(int name, int x, int y, StudentWorld* gameboard, Avatar* peach, Avatar* yoshi) : Square(name, x, y, gameboard, peach, yoshi, right, 1){
    m_amount_stored = 0;
}
void BankSquare::peachLandsOnSquare(){
    peach()->addCoins(m_amount_stored);
    m_amount_stored = 0;
    Board()->playSound(SOUND_WITHDRAW_BANK);
}
void BankSquare::peachPassesSquare(){
    int coins = peach()->getCoins();
    peach()->addCoins(-5);
    coins -= peach()->getCoins(); //if peach started with less than 5 coins, bank will only add the coins it "took"
    m_amount_stored += coins;
    Board()->playSound(SOUND_DEPOSIT_BANK);
}
void BankSquare::yoshiLandsOnSquare(){
    yoshi()->addCoins(m_amount_stored);
    m_amount_stored = 0;
    Board()->playSound(SOUND_WITHDRAW_BANK);
}
void BankSquare::yoshiPassesSquare(){
    int coins = yoshi()->getCoins();
    yoshi()->addCoins(-5);
    coins -= yoshi()->getCoins(); //if yoshi started with less than 5 coins, bank will only add the coins it "took"
    m_amount_stored += coins;
    Board()->playSound(SOUND_DEPOSIT_BANK);
}

//EventSquare implementations:
EventSquare::EventSquare(int name, int x, int y, StudentWorld* gameboard, Avatar* peach, Avatar* yoshi) : Square(name, x, y, gameboard, peach, yoshi, right, 1){
    
}
//void EventSquare::peachLandsOnSquare(){
//    int action = randInt(1, 2);
//    switch (action) {
//        case 1: {
//            bool validSquare = false;
//            int x = randInt(0, SPRITE_WIDTH - 1);
//            int y = randInt(0, SPRITE_HEIGHT - 1);
//            while (!validSquare){
//                if (Board()->board().getContentsOf(x, y) != Board::empty){
//                    validSquare = true;
//                }else{
//                    x = randInt(0, SPRITE_WIDTH - 1);
//                    y = randInt(0, SPRITE_HEIGHT - 1);
//                }
//            }
//            peach()->teleport(x*SPRITE_WIDTH, y*SPRITE_HEIGHT);
//            changePeachOnSquare();
//            changeYoshiOnSquare();
//            Board()->playSound(SOUND_PLAYER_TELEPORT);
//            break;
//        }
//        case 2:{
//            peach()->swap(yoshi());
//            changePeachOnSquare();
//            changeYoshiOnSquare();
//            break;
//        }
//        case 3:{
//            //TODO: Implement a vortex
//            break;
//        }
//    }
//}
//void EventSquare::yoshiLandsOnSquare(){ //what happens when they are both on the same square?
//    int action = randInt(1, 3);
//    switch (action) {
//        case 1: {
//            bool validSquare = false;
//            int x = randInt(0, SPRITE_WIDTH - 1);
//            int y = randInt(0, SPRITE_HEIGHT - 1);
//            while (!validSquare){
//                if (Board()->board().getContentsOf(x, y) != Board::empty){
//                    validSquare = true;
//                }else{
//                    x = randInt(0, SPRITE_WIDTH - 1);
//                    y = randInt(0, SPRITE_HEIGHT - 1);
//                }
//            }
//            yoshi()->teleport(x*SPRITE_WIDTH, y*SPRITE_HEIGHT);
//            changePeachOnSquare();
//            changeYoshiOnSquare();
//            Board()->playSound(SOUND_PLAYER_TELEPORT);
//            break;
//        }
//        case 2:{
//            yoshi()->swap(peach());// TODO: why does this run when yoshi moves?
//            changePeachOnSquare();
//            changeYoshiOnSquare();
//            break;
//        }
//        case 3:{
//            //TODO: Implement a vortex
//            break;
//        }
//    }
//}
//

void EventSquare::doSomething(){
    if (peachIsOnSquare() == false && (peach()->getX() == getX() && peach()->getY() == getY() && peach()->getState() == false)){ // when peach stops walking and lands directly on top of the square
        int action = 2;
        if (action == 1){
            
        }
        else if (action == 2){
            peach()->swap(yoshi());
            setPeachOnSquare(false);
            setYoshiOnSquare(true);
            
        }
    }
    else if (peachIsOnSquare() == true && (peach()->getX() != getX() || peach()->getY() != getY()) && peach()->getState() == true){
        setPeachOnSquare(false);
    }
    if (yoshiIsOnSquare() == false && (yoshi()->getX() == getX() && yoshi()->getY()  == getY() && yoshi()->getState() == false)){
        int action = 2;
        if (action == 2){
            yoshi()->swap(peach());
            setPeachOnSquare(true);
            setYoshiOnSquare(false);
        }
    }
    else if (yoshiIsOnSquare() == true && (yoshi()->getX() != getX() || yoshi()->getY()  != getY()) && yoshi()->getState() == true){
        setYoshiOnSquare(false);
    }
}
