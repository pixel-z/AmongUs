#include "main.h"

#ifndef GRID_H
#define GRID_H

#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

class Grid {
    public:
        bool visited;
        bool path[4];

        Grid(){
            visited = false;
            for(int i = 0; i < 4; i++){
                path[i] = false;
            }
        }
};


#endif // !GRID_H