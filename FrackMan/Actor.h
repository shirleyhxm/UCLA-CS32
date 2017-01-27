#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;


class Actor : public GraphObject
{
public:
    Actor(StudentWorld* wd, int imageID, int startX, int startY, bool visible, Direction dir, double size, unsigned int depth);
    virtual ~Actor();
    virtual void doSomething() = 0;
    virtual bool annoy(unsigned int amt);
    virtual bool canActorsPassThroughMe() const;    //区分Boulder
    virtual bool canPickThingsUp() const;           //区分Agent
    virtual bool huntsFrackMan() const;             //区分Protester
    virtual void addGold();
    bool moveToIfPossible(Direction dir);
    StudentWorld* getWorld() const;
    bool isAlive() const;
    void setDead();
private:
    StudentWorld* m_world;
    bool m_alive;
};

class Agent : public Actor
{
public:
    Agent(StudentWorld* wd, int imageID, int startX, int startY, Direction dir, unsigned int hitPoints);
    virtual void doSomething();
    virtual bool annoy(unsigned int amount);
    virtual bool canPickThingsUp() const;
    virtual bool huntsFrackMan() const;
    virtual void addGold();
    unsigned int getHitPoints() const;
protected:
    int m_hitPts;
};

struct Dirt : public Actor
{
    Dirt(StudentWorld* wd, int startX, int startY);
    virtual void doSomething();
};

struct Boulder : public Actor
{
public:
    Boulder(StudentWorld* wd, int startX, int startY, bool stable);
    virtual void doSomething();
    virtual bool canActorsPassThroughMe() const;
private:
    bool m_stable;
    bool m_falling;
    unsigned int m_waitingTicks;
};

struct Squirt : public Actor
{
    Squirt(StudentWorld* wd, int startX, int startY, Direction dir);
    virtual void doSomething();
};

class FrackMan : public Agent
{
public:
    FrackMan(StudentWorld* wd);
    virtual void doSomething();
    virtual void addGold();
    virtual bool annoy(unsigned int amount);
    void addSonar();
    void addWater();
    unsigned int getGold() const;
    unsigned int getSonar() const;
    unsigned int getWater() const;
    unsigned int getHealth() const;
private:
    int m_water;
    int m_sonar;
    int m_gold;
    int m_hlth;
};

struct Protester : public Agent
{
public:
    Protester(StudentWorld* wd, int imageID, unsigned int hitPoints);
    virtual void doSomething();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsFrackMan() const;
    void setTicksToNextMove(bool stunned);
    void setMustLeaveOilField();
    int getPauseTime() const;
    bool mustLeave() const;
    void moveStraightNSquares(int nSqrs);
    int getMoveStraightNSquares() const;
    void setStunned();
    bool isStunned() const;
    void free();
    void resetTicks(int startTicks);
    int getTicks() const;
    void addTick();
    bool seeDirectly();
    virtual bool isReg() const = 0;
    GraphObject::Direction canTurnPerp();
private:
    bool m_mustLeave;
    int m_pauseTicks;
    int m_moveStraightNSquares;
    bool m_stunned;
    int m_ticks;
    int m_ticksAfterShout;
    int m_ticksAfterPerpTurn;
};

struct RegPro : public Protester
{
public:
    RegPro(StudentWorld* wd);
    virtual bool isReg() const;
};

struct HardPro : public Protester
{
    HardPro(StudentWorld* wd);
    virtual bool isReg() const;
};

struct ActivatingObject : public Actor
{
public:
    ActivatingObject(StudentWorld* wd, int imageID, int startX, int startY, int soundToPlay, int depth, bool visible, bool activateOnPlayer, bool activateOnProtester, bool initiallyActive);
    virtual void doSomething();
    void setTicksToLive(int lifeTime);
    int getLifeTime() const;
    void activate();
    bool isActive() const;
private:
    int m_lifeTime;
    bool m_active;
};

struct Oil : public ActivatingObject
{
    Oil(StudentWorld* wd, int startX, int startY);
    virtual void doSomething();
};

struct Gold : public ActivatingObject
{
public:
    Gold(StudentWorld* wd, int startX, int startY, bool visible, bool temporary, bool pickableByPlayer, bool pickableByProtester);
    virtual void doSomething();
private:
    bool m_temporary;
    bool m_pickableByPlayer, m_pickableByProtester;
};

struct Sonar : public ActivatingObject
{
    Sonar(StudentWorld* wd);
    virtual void doSomething();
};

struct Water : public ActivatingObject
{
    Water(StudentWorld* wd, int startX, int startY);
    virtual void doSomething();
};




#endif // ACTOR_H_
