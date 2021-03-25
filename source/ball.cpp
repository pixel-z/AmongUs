#include "ball.h"
#include "main.h"

void Ball::drawSemiCircle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii <= num_segments; ii++)
    {
        float theta = 3.1415926f * float(ii) / float(num_segments);//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        glColor3f(1,1,0);
        glVertex2f(x + cx, y + cy);//output vertex
    }
    glEnd();
}

// x & y of bottom left corner
void Ball::drawRectangle(float x, float y, float width, float height)
{
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS);
    glColor3d(1,1,0);
    glVertex2f(x,y);
    glColor3d(1,1,0);
    glVertex2f(x+width,y);
    glColor3d(1,1,0);
    glVertex2f(x+width,y+height);
    glColor3d(1,1,0);
    glVertex2f(x,y+height);
    glEnd();
    // glutSwapBuffers();
}

// x & y of bottom left corner
void Ball::draw(float x, float y){
    drawSemiCircle(x+3, y+3.5, 3, 20);
    drawRectangle(x, y, 6, 4);
    drawRectangle(x+1.5, y+4.5, 3, 1.5);
}

