//
//  main.cpp
//  HW2
//
//  Created by Shirley He on 1/31/16.
//  Copyright © 2016 Shirley He. All rights reserved.
//

#include <stack>
#include <iostream>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    stack<Coord> c;
    c.push(Coord(sr,sc));
    maze[sr][sc] = '-';
    
    while (!c.empty())
    {
        Coord curr = c.top();
        c.pop();
        if (curr.r() == er && curr.c() == ec)
            return true;
        
        if (maze[curr.r()-1][curr.c()] == '.')
        {
            c.push(Coord(curr.r()-1, curr.c()));
            maze[curr.r()-1][curr.c()] = '-';
        }
        if (maze[curr.r()][curr.c()+1] == '.')
        {
            c.push(Coord(curr.r(), curr.c()+1));
            maze[curr.r()][curr.c()+1] = '-';
        }
        if (maze[curr.r()+1][curr.c()] == '.')
        {
            c.push(Coord(curr.r()+1, curr.c()));
            maze[curr.r()+1][curr.c()] = '-';
        }
        if (maze[curr.r()][curr.c()-1] == '.')
        {
            c.push(Coord(curr.r(), curr.c()-1));
            maze[curr.r()][curr.c()-1] = '-';
        }
    }
    return false;
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
