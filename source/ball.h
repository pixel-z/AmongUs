#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    int curr_x;
    int curr_y;
    int health;

    // constructor
    Ball() {
        health = 5;
        curr_x = 12;
        curr_y = 12;
    }
    void draw(float x, float y, int ball_no);
    void drawRectangle(float x, float y, float width, float height, int ball_no);
    void drawSemiCircle(float cx, float cy, float r, int num_segments, int ball_no);
};

class Button {
    public:
        int curr_x;
        int curr_y;
        int finish;

        Button() {
            finish = 0;
        }
    
        void draw(float r, int task_no);
};

class Obstacles {
    public:
        int curr_x;
        int curr_y;
        int finish;
        int sign;

        Obstacles() {
            finish = 0;
        }

        void draw(int sign);
};

#endif // BALL_H
