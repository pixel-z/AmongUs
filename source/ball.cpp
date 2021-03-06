#include "ball.h"
#include "main.h"

void Ball::drawSemiCircle(float cx, float cy, float r, int num_segments, int ball_no)
{
    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii <= num_segments; ii++)
    {
        float theta = 3.1415926f * float(ii) / float(num_segments);//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        if(ball_no == 1)    // imposter
            glColor3f(1,0,0);
        else
            glColor3f(0,1,0);
        glVertex2f(x + cx, y + cy);//output vertex
    }
    glEnd();
}

// x & y of bottom left corner
void Ball::drawRectangle(float x, float y, float width, float height, int ball_no)
{
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);
    if(ball_no == 1)    // imposter
        glColor3f(1,0,0);
    else
        glColor3f(0,1,0);

    // glColor3d(1,1,0);
    glVertex2f(x,y);
    // glColor3d(1,1,0);
    glVertex2f(x+width,y);
    // glColor3d(1,1,0);
    glVertex2f(x+width,y+height);
    // glColor3d(1,1,0);
    glVertex2f(x,y+height);
    glEnd();
    // glutSwapBuffers();
}

// x & y of bottom left corner
void Ball::draw(float x, float y, int ball_no){
    drawSemiCircle(x+3, y+3.5, 3, 20, ball_no);
    drawRectangle(x, y, 6, 4, ball_no);
    drawRectangle(x+1.5, y+4.5, 3, 1.5, ball_no);
}

void Button::draw(float r, int task_no){
    int num_segments = 20;
    float cx = this->curr_x;
    float cy = this->curr_y;

    glBegin(GL_TRIANGLE_FAN);
    for(int ii = 0; ii <= num_segments; ii++)
    {
        float theta = 2* 3.1415926f * float(ii) / float(num_segments);//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component
        
        if(task_no == 2)
            glColor3f(0.6,0,0.4);
        else
            glColor3f(0,1,1);

        glVertex2f(x + cx, y + cy);//output vertex
    }
    glEnd();    
}

void Obstacles::draw(int sign){
    // sign = 0 : obstacle
    // sign = 1 : powerup
    this->sign = sign;
    if (sign==1)
    {
        int num_segments = 20;
        float cx = this->curr_x;
        float cy = this->curr_y;
        float r = 2;

        glBegin(GL_TRIANGLE_FAN);
        for(int ii = 0; ii <= num_segments; ii++)
        {
            float theta = 2* 3.1415926f * float(ii) / float(num_segments);

            float x = r * cosf(theta);
            float y = r * sinf(theta);

            glColor3f(1.0,1,0.0);
            glVertex2f(x + cx, y + cy);
        }

        r=1;
        for(int ii = 0; ii <= num_segments; ii++)
        {
            float theta = 2* 3.1415926f * float(ii) / float(num_segments);

            float x = r * cosf(theta);
            float y = r * sinf(theta);

            glColor3f(0.0,0,0.0);
            glVertex2f(x + cx, y + cy);
        }
        glEnd();
    }
    else
    {
        int num_segments = 20;
        float cx = this->curr_x;
        float cy = this->curr_y;
        float r = 2;

        glBegin(GL_TRIANGLE_FAN);
        for(int ii = 0; ii <= num_segments; ii++)
        {
            float theta = 2* 3.1415926f * float(ii) / float(num_segments);

            float x = r * cosf(theta);
            float y = r * sinf(theta);

            glColor3f(220/255,220/255,220/255);
            glVertex2f(x + cx, y + cy);
        }

        r=0.5;
        for(int ii = 0; ii <= num_segments; ii++)
        {
            float theta = 2* 3.1415926f * float(ii) / float(num_segments);

            float x = r * cosf(theta);
            float y = r * sinf(theta);

            glColor3f(0.7,0,0.0);
            glVertex2f(x + cx, y + cy);
        }
        glEnd();
    }
}