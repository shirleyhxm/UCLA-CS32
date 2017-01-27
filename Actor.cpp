#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <vector>
using namespace std;

// Actor

Actor::Actor(StudentWorld* wd, int imageID, int startX, int startY, bool visible, Direction dir = right, double size = 1.0, unsigned int depth = 0)
:GraphObject(imageID, startX, startY, dir, size, depth), m_world(wd), m_alive(true)
{
    setVisible(visible);
}

Actor::~Actor()
{
    
}

bool Actor::annoy(unsigned int amt)
{
    return false;
}

bool Actor::canActorsPassThroughMe() const
{
    return true;
}

bool Actor::canPickThingsUp() const
{
    return false;
}

bool Actor::huntsFrackMan() const
{
    return false;
}

bool Actor::moveToIfPossible(Direction dir)
{
    int x,y;
/*
    if (!this->canActorsPassThroughMe())
    {
        if (!getWorld()->willNotHit(getX(), getY(), dir, x, y, true, false))
            return false;
    }

    else */
    if (!getWorld()->willNotHit(getX(), getY(), dir, x, y, false, false))
        return false;
    
    moveTo(x, y);
    return true;
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

bool Actor::isAlive() const
{
    return m_alive;
}

void Actor::setDead()
{
    m_alive = false;
}

void Actor::addGold()
{
    
}


// Agent

Agent::Agent(StudentWorld* wd, int imageID, int startX, int startY, Direction dir, unsigned int hitPoints)
:Actor(wd, imageID, startX, startY, true, dir, 1, 0), m_hitPts(hitPoints)
{}

void Agent::doSomething()
{
    
}

unsigned int Agent::getHitPoints() const
{
    return m_hitPts;
}

bool Agent::annoy(unsigned int amount)
{
    return true;
}

bool Agent::canPickThingsUp() const
{
    return true;
}

void Agent::addGold()
{
    
}

bool Agent::huntsFrackMan() const
{
    return false;
}

// Dirt


Dirt::Dirt(StudentWorld* wd, int startX, int startY)
: Actor(wd, IID_DIRT, startX, startY, true, right, 0.25, 3)
{}

void Dirt::doSomething()
{
    
}

// Boulder

Boulder::Boulder(StudentWorld* wd, int startX, int startY, bool stable)
: Actor(wd, IID_BOULDER, startX, startY, true, down, 1.0, 1), m_stable(stable), m_waitingTicks(0), m_falling(false)
{}

void Boulder::doSomething()
{
    if (!isAlive())
        return;
    int x, y;
    if (m_stable)
    {
        if (getWorld()->willNotHit(getX(), getY(), down, x, y, true, false))
            m_stable = false;
    }
    else if (m_falling == true)
    {
        getWorld()->annoyAllNearByActors(this, 100, 3);

        if (getWorld()->findNearbyFrackMan(this, 3) != nullptr)
            getWorld()->findNearbyFrackMan(this, 3)->annoy(100);
        if (!moveToIfPossible(down))
        {
            setDead();
            return;
        }
    }
    else if (m_waitingTicks == 30)
    {
        m_falling = true;
        getWorld()->playSound(SOUND_FALLING_ROCK);
    }
    else
        m_waitingTicks++;
}

bool Boulder::canActorsPassThroughMe() const
{
    return false;
}

// Squirt

Squirt::Squirt(StudentWorld* wd, int startX, int startY, Direction dir)
: Actor(wd, IID_WATER_SPURT, startX, startY, true, dir, 1.0, 1)
{
    if (!moveToIfPossible(none))
        setDead();
}

void Squirt::doSomething()
{
    if (!isAlive())
    {
        setDead();
        return;
    }
    if (getWorld()->annoyAllNearByActors(this, 2, 3) > 0 || !moveToIfPossible(getDirection()))
        setDead();
}

// FrackMan

FrackMan::FrackMan(StudentWorld* wd)
: Agent(wd, IID_PLAYER, 30, 60, right, 10), m_water(5), m_sonar(1), m_gold(0), m_hlth(100)
{}

bool FrackMan::annoy(unsigned int amount)
{
    m_hitPts -= amount;
    if (getHitPoints() <= 0)
    {
        m_hitPts = 0;
        setDead();
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
    }
    m_hlth = m_hitPts*10;
    return true;
}

void FrackMan::doSomething()
{
    int a,b = 0;
    if (!isAlive())
        return;
    int ch;
    if (getWorld()->getKey(ch)==true)
    {
        switch (ch)
            {
                case KEY_PRESS_UP:
                    if (getDirection() != up)
                        setDirection(up);
                    else if (getWorld()->willNotHit(getX(), getY(), up, a, b, false, true))
                        moveTo(a,b);
                    break;
                case KEY_PRESS_DOWN:
                    if (getDirection() != down)
                        setDirection(down);
                    else if (getWorld()->willNotHit(getX(), getY(), down, a, b, false, true))
                        moveTo(a,b);
                    break;
                case KEY_PRESS_LEFT:
                    if (getDirection() != left)
                        setDirection(left);
                    else if (getWorld()->willNotHit(getX(), getY(), left, a, b, false, true))
                        moveTo(a,b);
                    break;
                case KEY_PRESS_RIGHT:
                    if (getDirection() != right)
                        setDirection(right);
                    else if (getWorld()->willNotHit(getX(), getY(), right, a, b, false, true))
                        moveTo(a,b);
                    break;
                case KEY_PRESS_ESCAPE:
                    setDead();
                    break;
                case KEY_PRESS_SPACE:
                    if (getWater() > 0)
                    {
                        switch (getDirection())
                        {
                            case GraphObject::up:
                                getWorld()->addActor(new Squirt(getWorld(), getX(), getY()+4, up));
                                break;
                            case GraphObject::down:
                                getWorld()->addActor(new Squirt(getWorld(), getX(), getY()-4, down));
                                break;
                            case GraphObject::left:
                                getWorld()->addActor(new Squirt(getWorld(), getX()-4, getY(), left));
                                break;
                            case GraphObject::right:
                                getWorld()->addActor(new Squirt(getWorld(), getX()+4, getY(), right));
                                break;
                            default:
                                break;
                        }
                        m_water--;
                        getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                    }
                    break;
                case 'Z':
                case 'z':
                    if (getSonar()>0)
                    {
                        getWorld()->revealAllNearbyObjects(getX(), getY(), 12);
                        m_sonar--;
                    }
                    break;
                case KEY_PRESS_TAB:
                    if (m_gold > 0)
                    {
                        getWorld()->addActor(new Gold(getWorld(), getX(), getY(), true, true, false, true));
                        m_gold--;
                    }
                default:
                    break;
            }
        size_t beforeDirt = getWorld()->getNumDirts();
        getWorld()->removeDirt(getX(),getY());
        if (beforeDirt != getWorld()->getNumDirts())
            getWorld()->playSound(SOUND_DIG);
    }
}

void FrackMan::addSonar()
{
    m_sonar++;
}

void FrackMan::addWater()
{
    m_water += 5;
}

void FrackMan::addGold()
{
    m_gold++;
}

unsigned int FrackMan::getGold() const
{
    return m_gold;
}

unsigned int FrackMan::getSonar() const
{
    return m_sonar;
}

unsigned int FrackMan::getWater() const
{
    return m_water;
}

unsigned int FrackMan::getHealth() const
{
    return m_hlth;
}

// Protester

Protester::Protester(StudentWorld* wd, int imageID, unsigned int hitPoints)
: Agent(wd, imageID, 60, 60, left, hitPoints), m_mustLeave(false), m_stunned(false), m_ticks(0), m_ticksAfterShout(15), m_ticksAfterPerpTurn(200)
{
    moveStraightNSquares(-1);
    setTicksToNextMove(m_stunned);
}

bool Protester::huntsFrackMan() const
{
    return true;
}

void Protester::setMustLeaveOilField()
{
    m_mustLeave = true;
}

bool Protester::mustLeave() const
{
    return m_mustLeave;
}

int Protester::getPauseTime() const
{
    return m_pauseTicks;
}

void Protester::moveStraightNSquares(int nSqrs)
{
    if (nSqrs >= 0)
        m_moveStraightNSquares = nSqrs;
    else
    {
        int sx = getX();
        int ex = getX();
        int sy = getY();
        int ey = getY();
        int i = 0;
        while (getWorld()->willNotHit(sx, sy, getDirection(), ex, ey, false, false))
        {
            sx = ex;
            sy = ey;
            i++;
        }
        m_moveStraightNSquares = i;
    }
}

int Protester::getMoveStraightNSquares() const
{
    return m_moveStraightNSquares;
}

void Protester::resetTicks(int startTicks)
{
    m_ticks = startTicks;
}

void Protester::setStunned()
{
    m_stunned = true;
}

bool Protester::isStunned() const
{
    return m_stunned;
}

void Protester::free()
{
    m_stunned = false;
}

int Protester::getTicks() const
{
    return m_ticks;
}

void Protester::addTick()
{
    m_ticks++;
}

bool Protester::seeDirectly()
{
    if (getWorld()->lineOfSightToFrackMan(this) == none)
        return false;
    
    Direction d = getDirection();
    int oriNSqrs = m_moveStraightNSquares;
    setDirection(getWorld()->lineOfSightToFrackMan(this));
    moveStraightNSquares(-1);
    if (abs( (getX()-getWorld()->findNearbyFrackMan(this, 60)->getX()) + (getY()-getWorld()->findNearbyFrackMan(this,60)->getY()))-4 <= m_moveStraightNSquares)
    {
        setDirection(d);
        moveStraightNSquares(oriNSqrs);
        return true;
    }
    return false;
}

void Protester::addGold()
{
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);
    setMustLeaveOilField();
}

GraphObject::Direction Protester::canTurnPerp()
{
    int a = 0;
    int b = 0;
    switch (getDirection())
    {
        case GraphObject::up:
        case GraphObject::down:
            if (getWorld()->willNotHit(getX(), getY(), left, a, b, false, false))
                return left;
            if (getWorld()->willNotHit(getX(), getY(), right, a, b, false, false))
                return right;
            return none;
            break;
        case GraphObject::left:
        case GraphObject::right:
            if (getWorld()->willNotHit(getX(), getY(), up, a, b, false, false))
                return up;
            if (getWorld()->willNotHit(getX(), getY(), down, a, b, false, false))
                return down;
            return none;
            break;
        default:
            return none;
    }
}

bool Protester::annoy(unsigned int amount)
{
    if (mustLeave())
        return false;
    m_hitPts -= amount;
    if (m_hitPts > 0)
    {
        setStunned();
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        setTicksToNextMove(isStunned());
    }
    else
    {
        setMustLeaveOilField();
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        this->resetTicks(-1);
        if (amount == 100)
            getWorld()->increaseScore(500);
        if (amount == 2)
        {
            if (isReg())
                getWorld()->increaseScore(100);
            else
                getWorld()->increaseScore(250);
        }
    }
    return true;
}

void Protester::setTicksToNextMove(bool stunned)
{
    if (stunned == false)
        m_pauseTicks = max<int>(0, 3-getWorld()->getLevel()/4);
    else
        m_pauseTicks = max<int>(50, 100-getWorld()->getLevel()*10);
}

void Protester::doSomething()
{
    if (!isAlive())
        return;
    
    if (isStunned())
    {
        free();
        resetTicks(0);
    }
    
    addTick();       
    
    if (getTicks() % (getPauseTime()+1) != 0)
        return;
    
    setTicksToNextMove(isStunned());
    resetTicks(0);
    m_ticksAfterShout++;
    m_ticksAfterPerpTurn++;
    
    
    if (mustLeave())
    {
        if (getX() == 60 && getY() == 60)
            setDead();
        else
        {
            setDirection(getWorld()->determineFirstMoveToExit(getX(), getY()));
            moveToIfPossible(getDirection());
            return;
        }
    }
    else
    {
        if (getWorld()->findNearbyFrackMan(this, 4) != nullptr && getWorld()->facingTowardFrackMan(this) && m_ticksAfterShout > 15)
        {
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            getWorld()->findNearbyFrackMan(this, 4)->annoy(2);
            m_ticksAfterShout = 0;
            return;
        }
        else
        {
            if (!isReg())
            {
                int M = 16 + getWorld()->getLevel()*2;
                if (getWorld()->determineFirstMoveToFrackMan(getX(), getY(), M) != none)
                {
                    setDirection(getWorld()->determineFirstMoveToFrackMan(getX(), getY(), M));
                    moveToIfPossible(getDirection());
                    return;
                }
            }
            if (getWorld()->lineOfSightToFrackMan(this) != none && this->seeDirectly())
            {
                setDirection(getWorld()->lineOfSightToFrackMan(this));
                moveToIfPossible(getDirection());
                moveStraightNSquares(0);
                return;
            }
            else
            {
                moveStraightNSquares(getMoveStraightNSquares()-1);
                if (getMoveStraightNSquares() <= 0)
                {
                    Direction dir[4] = {up, down, left, right};
                    while (!moveToIfPossible(getDirection()))
                        setDirection(dir[rand()%4]);
                    moveStraightNSquares(-1);
                    return;
                }
                else
                {
                    if (canTurnPerp() != none && m_ticksAfterPerpTurn > 200)
                    {
                        setDirection(canTurnPerp());
                        moveStraightNSquares(-1);
                        m_ticksAfterPerpTurn = 0;
                    }
                    if (!moveToIfPossible(getDirection()))
                        moveStraightNSquares(0);
                    return;
                }
                
            }
        }
    }
}

// Regular Protester

RegPro::RegPro(StudentWorld* wd)
: Protester(wd, IID_PROTESTER, 5)
{
}

bool RegPro::isReg() const
{
    return true;
}

// Hardcore Protester

HardPro::HardPro(StudentWorld* wd)
: Protester(wd, IID_HARD_CORE_PROTESTER, 20)
{}

bool HardPro::isReg() const
{
    return false;
}

// Activating Objects

ActivatingObject::ActivatingObject(StudentWorld* wd, int imageID, int startX, int startY, int soundToPlay, int depth = 10, bool visible = false, bool activateOnPlayer = true, bool activateOnProtester = false, bool initiallyActive = false)
: Actor(wd, imageID, startX, startY, visible, right, 1, depth), m_active(initiallyActive)
{}

void ActivatingObject::doSomething()
{
    
}

void ActivatingObject::setTicksToLive(int lifeTime)
{
    m_lifeTime = lifeTime;
}

int ActivatingObject::getLifeTime() const
{
    return m_lifeTime;
}

void ActivatingObject::activate()
{
    m_active = true;
}

bool ActivatingObject::isActive() const
{
    return m_active;
}

// Oil Barrel

Oil::Oil(StudentWorld* wd, int startX, int startY)
: ActivatingObject(wd, IID_BARREL, startX, startY, SOUND_FOUND_OIL, 2)
{}

void Oil::doSomething()
{
    if (!isAlive())
        return;
    if (!isActive() && getWorld()->findNearbyFrackMan(this, 4) != nullptr)
    {
        setVisible(true);
        activate();
        return;
    }
    if (getWorld()->findNearbyFrackMan(this, 3) != nullptr)
    {
        setDead();
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->foundOil();
    }
        
}

// Gold Nugget

Gold::Gold(StudentWorld* wd, int startX, int startY, bool visible, bool temporary, bool pickableByPlayer, bool pickableByProtester)
: ActivatingObject(wd, IID_GOLD, startX, startY, SOUND_GOT_GOODIE, 2, visible, pickableByPlayer, pickableByProtester), m_temporary(temporary), m_pickableByPlayer(pickableByPlayer), m_pickableByProtester(pickableByProtester)
{
    if (m_temporary)
        setTicksToLive(100);
}

void Gold::doSomething()
{
    if (!isAlive())
        return;
    if (!isActive() && getWorld()->findNearbyFrackMan(this, 4) != nullptr)
    {
        setVisible(true);
        activate();
        return;
    }
    if (m_pickableByPlayer && getWorld()->findNearbyFrackMan(this, 3) != nullptr)
    {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(10);
        getWorld()->findNearbyFrackMan(this, 3)->addGold();
    }
    else if (m_pickableByProtester && getWorld()->findNearbyPickerUpper(this, 3) != nullptr)
    {
        setDead();
        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        getWorld()->findNearbyPickerUpper(this, 3)->addGold();
    }
    
    if (m_temporary)
    {
        if (getLifeTime() == 0)
            setDead();
        else
            setTicksToLive(getLifeTime()-1);
    }
}

// Sonar

Sonar::Sonar(StudentWorld* wd)
: ActivatingObject(wd, IID_SONAR, 0, 60, SOUND_SONAR, 2, true)
{
    setTicksToLive(max<int>(100, 300-10*(getWorld()->getLevel())));
}

void Sonar::doSomething()
{
    if (!isAlive())
        return;
    if (getWorld()->findNearbyFrackMan(this, 3) != nullptr)
    {
        setDead();
        getWorld()->playSound(SOUND_SONAR);
        getWorld()->findNearbyFrackMan(this, 3)->addSonar();
        getWorld()->increaseScore(75);
    }
    
    if (getLifeTime() == 0)
        setDead();
    else
        setTicksToLive(getLifeTime()-1);
}

// Water

Water::Water(StudentWorld* wd, int startX, int startY)
: ActivatingObject(wd, IID_WATER_POOL, startX, startY, SOUND_GOT_GOODIE, 2, true)
{
    setTicksToLive(max<int>(100, 300-10*(getWorld()->getLevel())));
}

void Water::doSomething()
{
    if (!isAlive())
        return;
    if (getWorld()->findNearbyFrackMan(this, 3) != nullptr)
    {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->findNearbyFrackMan(this, 3)->addWater();
        getWorld()->increaseScore(100);
    }
    
    if (getLifeTime() == 0)
        setDead();
    else
        setTicksToLive(getLifeTime()-1);
}



