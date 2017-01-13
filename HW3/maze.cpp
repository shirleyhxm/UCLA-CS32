//
//  main.cpp
//  HW3(No.3)
//
//  Created by Shirley He on 2/8/16.
//  Copyright © 2016 Shirley He. All rights reserved.
//

#include "string"
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr < 0  ||  sr > nRows-1  ||  sc < 0  || sc > nCols-1  ||
        er < 0  ||  er > nRows-1  ||  ec < 0  || ec > nCols-1  ||
        maze[sr][sc] != '.'  ||  maze[er][ec] != '.')
        return false;
    
    if (sr == er && sc == ec)
        return true;
    
    maze[sr][sc] = 'o';
    if (maze[sr-1][sc]=='.')
        if(pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
            return true;
    if (maze[sr][sc+1]=='.')
        if(pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
            return true;
    if (maze[sr+1][sc]=='.')
        if(pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
            return true;
    if (maze[sr][sc-1]=='.')
        if(pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
            return true;
    return false;
}

