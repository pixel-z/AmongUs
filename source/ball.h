#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    int curr_x;
    int curr_y;

    // constructor
    Ball() {
        curr_x = 12;
        curr_y = 12;
    }
    void draw(float x, float y);
    void drawRectangle(float x, float y, float width, float height);
    void drawSemiCircle(float cx, float cy, float r, int num_segments);
};

class Button {
    public:
        int curr_x;
        int curr_y;
        int finish;

        Button() {
            finish = 0;
        }
    
        void draw(float r);
};

#endif // BALL_H
