#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <queue>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::~StudentWorld()
{
    if (m_fm != nullptr)
        delete m_fm;
    for (vector<Dirt*>::iterator p = m_dirt.begin(); p != m_dirt.end();)
    {
        delete *p;
        p = m_dirt.erase(p);
    }
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end();)
    {
        delete *p;
        p = m_actors.erase(p);
    }
}

string StudentWorld::convertToString(unsigned int m, int numOfDigits, string ledBy)
{
    string s = to_string(m);
    for (size_t i = s.length(); i < numOfDigits; i++)
        s.insert(0, ledBy);
    return s;
}

size_t StudentWorld::getNumDirts() const
{
    return m_dirt.size();
}

bool StudentWorld::farEnough(vector<Actor*> va, int x, int y) const
{
    for (vector<Actor*>::iterator p = va.begin(); p != va.end(); p++)
        if (sqrt(((*p)->getX()-x)*((*p)->getX()-x) + ((*p)->getY()-y)*((*p)->getY()-y)) <= 6)
            return false;
    return true;
}

bool StudentWorld::withinForbiddenZone(int x, int y, int startX, int endX, int startY, int endY) const
{
    return (x >= startX && x <= endX && y >= startY && y <= endY);
}

bool StudentWorld::willNotHit(int x, int y, GraphObject::Direction dir, int& moveX, int& moveY, bool isBoulder = false, bool isFM = false) const
{
    
    int m = x;
    int n = y;
    switch (dir)
    {
        case GraphObject::up:
            n++;
            break;
        case GraphObject::down:
            n--;
            break;
        case GraphObject::left:
            m--;
            break;
        case GraphObject::right:
            m++;
            break;
        default:        // when Boulder check to see if it can fall
            break;
    }
    
    if (m < 0 || m > 60 || n < 0 || n > 60)
        return false;
    
    if (!isFM)
    {
        for (int i = 0; i < m_dirt.size(); i++)
            if (withinForbiddenZone(m_dirt[i]->getX(), m_dirt[i]->getY(), m, m+3, n, n+3))
                return false;
    }

    for (int i = 0; i < m_actors.size(); i++)
        if (!m_actors[i]->canActorsPassThroughMe() && sqrt((m_actors[i]->getX()-m)*(m_actors[i]->getX()-m) + (m_actors[i]->getY()-n)*(m_actors[i]->getY()-n)) <= 3)
        {
            if (isBoulder && m_actors[i]->getX()==x && m_actors[i]->getY()==y)
                ;
            else
                return false;
        }
    
    moveX = m;
    moveY = n;
    return true;
}

void StudentWorld::addActor(Actor *a)
{
    m_actors.push_back(a);
}


void StudentWorld::removeDirt(int x, int y)
{
    for (vector<Dirt*>::iterator p = m_dirt.begin(); p != m_dirt.end(); )
    {
        if ((*p)->getX() >= x && (*p)->getX() <= x+3 && (*p)->getY() >= y && (*p)->getY() <= y+3)
        {
            delete *p;
            p = m_dirt.erase(p);
        }
        else
            p++;
    }
}

int StudentWorld::annoyAllNearByActors(Actor *annoyer, int points, int radius)
{
    int total = 0;
    for (int i = 0; i < m_actors.size(); i++)
    {
           if (sqrt((m_actors[i]->getX()-annoyer->getX())*(m_actors[i]->getX()-annoyer->getX()) + (m_actors[i]->getY()-annoyer->getY())*(m_actors[i]->getY()-annoyer->getY())) <= radius && m_actors[i]->annoy(points))
               total++;
    }
    return total;
}

void StudentWorld::revealAllNearbyObjects(int x, int y, int radius)
{
     for (int i = 0; i < m_actors.size(); i++)
     {
        if (sqrt((m_actors[i]->getX()-x)*(m_actors[i]->getX()-x) + (m_actors[i]->getY()-y)*(m_actors[i]->getY()-y)) <= radius)
            m_actors[i]->setVisible(true);
     }
}

FrackMan* StudentWorld::findNearbyFrackMan(Actor* a, int radius) const
{
    if (sqrt((m_fm->getX()-a->getX())*(m_fm->getX()-a->getX())+(m_fm->getY()-a->getY())*(m_fm->getY()-a->getY())) <= radius)
        return m_fm;
    return nullptr;
}

Actor* StudentWorld::findNearbyPickerUpper(Actor *a, int radius) const
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->huntsFrackMan() && sqrt((m_actors[i]->getX()-a->getX())*(m_actors[i]->getX()-a->getX()) + (m_actors[i]->getY()-a->getY())*(m_actors[i]->getY()-a->getY())) <= radius)
            return m_actors[i];
    }
    return nullptr;
}

bool StudentWorld::facingTowardFrackMan(Actor *a) const
{
    if (a->getX() == m_fm->getX() && a->getY() == m_fm->getY())
        return true;
    switch (a->getDirection())
    {
        case GraphObject::up:
            return m_fm->getY() > a->getY();
            break;
        case GraphObject::left:
            return m_fm->getX() < a->getX();
            break;
        case GraphObject::down:
            return m_fm->getY() < a->getY();
            break;
        case GraphObject::right:
            return m_fm->getX() > a->getX();
            break;
        default:
            return false;
            break;
    }
}

GraphObject::Direction StudentWorld::lineOfSightToFrackMan(Actor* a) const
{
    if (m_fm->getX() == a->getX())
    {
        if (m_fm->getY() < a->getY())
            return GraphObject::down;
        else if (m_fm->getY() > a->getY())
            return GraphObject::up;
        else
            return GraphObject::none;
    }
    if (m_fm->getY() == a->getY())
    {
        if (m_fm->getX() < a->getX())
            return GraphObject::left;
        return GraphObject::right;
    }
    return GraphObject::none;
}

GraphObject::Direction StudentWorld::determineFirstMoveToExit(int a, int b)
{

    for (int i = 0; i < 61; i++)
        for (int j = 0; j < 61; j++)
                maze[i][j] = -1;
    
    queue<Coord> toDo;
    toDo.push(Coord(60,60));
    maze[60][60] = 0;
   
    int m,n = 0;
    
    while (!toDo.empty())
    {
        Coord curr = toDo.front();
        toDo.pop();
        const int y = curr.y();    // r
        const int x = curr.x();    // c
        
        // up
        if (willNotHit(x, y, GraphObject::up, m, n) && maze[y+1][x] == -1)
        {
            toDo.push(Coord(x,y+1));
            maze[y+1][x] = maze[y][x]+1;
        }
        
        // down
        if (willNotHit(x, y, GraphObject::down, m, n) && maze[y-1][x] == -1)
        {
            toDo.push(Coord(x,y-1));
            maze[y-1][x] = maze[y][x]+1;
        }
        
        // left
        if (willNotHit(x, y, GraphObject::left, m, n) && maze[y][x-1] == -1)
        {
            toDo.push(Coord(x-1,y));
            maze[y][x-1] = maze[y][x]+1;
        }
        
        // right
        if (willNotHit(x, y, GraphObject::right, m, n) && maze[y][x+1] == -1)
        {
            toDo.push(Coord(x+1,y));
            maze[y][x+1] = maze[y][x]+1;
        }
    }

    if (willNotHit(a, b, GraphObject::up, m, n) && maze[b+1][a] < maze[b][a])
        return GraphObject::up;
    if (willNotHit(a, b, GraphObject::right, m, n) && maze[b][a+1] < maze[b][a])
        return GraphObject::right;
    if (willNotHit(a, b, GraphObject::down, m, n) && maze[b-1][a] < maze[b][a])
        return GraphObject::down;
    if (willNotHit(a, b, GraphObject::left, m, n) && maze[b][a-1] < maze[b][a])
        return GraphObject::left;
    return GraphObject::none;
}

GraphObject::Direction StudentWorld::determineFirstMoveToFrackMan(int a, int b, int M)
{
    for (int i = 0; i < 61; i++)
        for (int j = 0; j < 61; j++)
            maze[i][j] = -1;
    
    queue<Coord> toDo;
    toDo.push(Coord(m_fm->getX(),m_fm->getY()));
    maze[m_fm->getY()][m_fm->getX()] = 0;
    
    int m,n = 0;
    
    while (!toDo.empty())
    {
        Coord curr = toDo.front();
        toDo.pop();
        const int y = curr.y();    // r
        const int x = curr.x();    // c
        
        // up
        if (willNotHit(x, y, GraphObject::up, m, n) && maze[y+1][x] == -1)
        {
            toDo.push(Coord(x,y+1));
            maze[y+1][x] = maze[y][x]+1;
        }
        
        // down
        if (willNotHit(x, y, GraphObject::down, m, n) && maze[y-1][x] == -1)
        {
            toDo.push(Coord(x,y-1));
            maze[y-1][x] = maze[y][x]+1;
        }
        
        // left
        if (willNotHit(x, y, GraphObject::left, m, n) && maze[y][x-1] == -1)
        {
            toDo.push(Coord(x-1,y));
            maze[y][x-1] = maze[y][x]+1;
        }
        
        // right
        if (willNotHit(x, y, GraphObject::right, m, n) && maze[y][x+1] == -1)
        {
            toDo.push(Coord(x+1,y));
            maze[y][x+1] = maze[y][x]+1;
        }
    }
    if (maze[b][a] <= M)
    {
        if (willNotHit(a, b, GraphObject::up, m, n) && maze[b+1][a] < maze[b][a])
            return GraphObject::up;
        if (willNotHit(a, b, GraphObject::right, m, n) && maze[b][a+1] < maze[b][a])
            return GraphObject::right;
        if (willNotHit(a, b, GraphObject::down, m, n) && maze[b-1][a] < maze[b][a])
            return GraphObject::down;
        if (willNotHit(a, b, GraphObject::left, m, n) && maze[b][a-1] < maze[b][a])
            return GraphObject::left;
    }
    return GraphObject::none;
}

int StudentWorld::init()
{
    m_fm = new FrackMan(this);
    
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 60; j++)
        {
            if (i < 30 || i > 33 || j < 4)
                m_dirt.push_back(new Dirt(this, i, j));
        }
    }
    
   // m_actors.push_back(new Water(this, 0, 10));

    int x = rand() % 61 , y = rand() % 57;
    for (int o = 0; o < min<int>(2 + getLevel(), 20); o++)
    {
        while (!farEnough(m_actors, x, y) || withinForbiddenZone(x, y, 27, 33, 4, 59))
        {
            x = rand() % 61;
            y = rand() % 57;
        }
        m_actors.push_back(new Oil(this, x, y));
        m_oilLeft++;
    }
    
    for (int g = 0; g < max<int>(5-getLevel()/2, 2); g++)
    {
        while (!farEnough(m_actors, x, y) || withinForbiddenZone(x, y, 27, 33, 4, 59))
        {
            x = rand() % 61;
            y = rand() % 57;
        }
        m_actors.push_back(new Gold(this, x, y, false, false, true, false));
    }
    
    y = rand() % 37;
    for (int b = 0; b < min<int>(getLevel()/2 + 2, 6); b++)
    {                                                           // && withinMineShaft(x, y)???
        while (!farEnough(m_actors, x, 20+y) || withinForbiddenZone(x, y, 27, 33, 4, 59))
        {
            x = rand() % 61;
            y = rand() % 37;
        }
        m_actors.push_back(new Boulder(this, x, 20+y, true));
        removeDirt(x, 20+y);
    }
    
    m_actors.push_back(new RegPro(this));

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_Ticks++;
    
    // Update text
    setGameStatText("Scr: " + convertToString(getScore(), 6, "0") + "  Lvl: " + convertToString(getLevel(), 2, " ") + "  Lives: " + convertToString(getLives(), 1, "") + "  Hlth: " + convertToString(m_fm->getHealth(), 3, " ") + "%  Wtr: " + convertToString(m_fm->getWater(), 2, " ") + "  Gld: " + convertToString(m_fm->getGold(), 2, " ") + "  Sonar: " + convertToString(m_fm->getSonar(), 2, " ") + "  Oil Left: " + convertToString(m_oilLeft, 2, " "));
    
    // Ask all actors to do something
    m_fm->doSomething();
    
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); p++)
        (*p)->doSomething();
    
    // Check who is dead
    if (!m_fm->isAlive())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end(); )
    {
        if (!(*p)->isAlive())
        {
            if ((*p)->huntsFrackMan())
                m_nPro--;
            delete *p;
            p = m_actors.erase(p);
        }
        else
            p++;
    }
    
    // Check if win
    if(m_oilLeft == 0)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    // Add protesters
    if (m_Ticks >= max<unsigned int>(25, 200-getLives()) && m_nPro < min<double>(15, 2+getLevel()*1.5))
    {
        if (rand()%100 < min<unsigned int>(90, getLevel()*10+30))
            m_actors.push_back(new HardPro(this));
        else
            m_actors.push_back(new RegPro(this));
        m_nPro++;
        m_Ticks = 0;
    }
    
    // Add goodies
    if (rand()%(getLevel()*25+300) == 1)
    {
        if (rand()%5 == 1)
            m_actors.push_back(new Sonar(this));
        else
        {
            int x = 0;
            int y = 0;
            bool notAddable = true;
            vector<Dirt*>::iterator p;
            while (notAddable)
            {
                x = rand()%61;
                y = rand()%61;
                for (p = m_dirt.begin(); p != m_dirt.end(); p++)
                {
                    if (withinForbiddenZone((*p)->getX(), (*p)->getY(), x, x+3, y, y+3))
                        break;
                }
                if (p == m_dirt.end())
                    notAddable = false;
            }
            m_actors.push_back(new Water(this, x, y));
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_fm;
    m_fm = nullptr;
    for (vector<Dirt*>::iterator p = m_dirt.begin(); p != m_dirt.end();)
    {
        delete *p;
        p = m_dirt.erase(p);
    }
    for (vector<Actor*>::iterator p = m_actors.begin(); p != m_actors.end();)
    {
        delete *p;
        p = m_actors.erase(p);
    }
    m_nPro = 0;
    m_Ticks = 0;
}


void StudentWorld::foundOil()
{
    m_oilLeft--;
}
