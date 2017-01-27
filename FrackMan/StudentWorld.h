#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GraphObject.h"
#include <string>
#include <vector>
using namespace std;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir), m_oilLeft(0), m_Ticks(0), m_nPro(1)
	{
    }
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    string convertToString(unsigned int m, int numOfDigits, string ledBy);
    size_t getNumDirts() const;
    bool farEnough(vector<Actor*> va, int x, int y) const;
    bool withinForbiddenZone(int x, int y, int startX, int endX, int startY, int endY) const;
    bool willNotHit(int x, int y, GraphObject::Direction dir, int& moveX, int& moveY, bool isBoulder, bool isFM) const;
    void addActor(Actor* a);
    void removeDirt(int x, int y);
    int annoyAllNearByActors(Actor* annoyer, int points, int radius);
    void revealAllNearbyObjects(int x, int y, int radius);
    FrackMan* findNearbyFrackMan(Actor* a, int radius) const;
    Actor* findNearbyPickerUpper(Actor* a, int radius) const;
    bool facingTowardFrackMan(Actor* a) const;
    GraphObject::Direction lineOfSightToFrackMan(Actor* a) const;
    // bool isNearFrackMan(Actor* a, int radius) const;
    GraphObject::Direction determineFirstMoveToExit(int x, int y);
    GraphObject::Direction determineFirstMoveToFrackMan(int x, int y, int M);
    void foundOil();
private:
    struct Coord
    {
        Coord(int x, int y): m_x(x), m_y(y)  {}
        int x() const {return m_x;}
        int y() const {return m_y;}
        int m_x;
        int m_y;
    };
    vector<Dirt*> m_dirt;
    vector<Actor*> m_actors;
    FrackMan* m_fm;
    unsigned int m_oilLeft;
    unsigned int m_nPro;
    unsigned int m_Ticks;
    int maze[61][61]; 
};

#endif // STUDENTWORLD_H_
