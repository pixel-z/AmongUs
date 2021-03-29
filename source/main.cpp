#include "main.h"
#include "timer.h"
#include "ball.h"
#include "grid.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
double camera_rotation_angle = 30;

/*-------------------------------------------------------------------*/

// Dimensions of the maze
const int width=8, height=8;
int startGame = 0;
Ball ball1;
Grid grid[height][width];
Button task2;
Obstacles obs[4];
int SCORE = 0;
int exitX, exitY, exitDirection;

void removeLine(int x, int y, int direction){
    // cout<<x<<" "<<y<<" "<<direction<<endl;
    glColor3f( 0.0, 0.0, 0.0 );
    glBegin( GL_LINES );
    if (direction == UP) {
        glVertex2f( x*10+10, (y+1)*10+10 );
        glVertex2f( (x+1)*10+10, (y+1)*10+10 );
    }
    else if (direction == DOWN) {
        glVertex2f( (x)*10+10, y*10+10 );
        glVertex2f( (x+1)*10+10, y*10+10 );
    }
    else if (direction == LEFT) {
        glVertex2f( x*10+10, y*10+10 );
        glVertex2f( x*10+10, (y+1)*10+10 );
    }
    else if (direction == RIGHT) {
        glVertex2f( (x+1)*10+10, y*10+10 );
        glVertex2f( (x+1)*10+10, (y+1)*10+10 );
    }

    glEnd();
}

void draw_grid()
{
    double x;
    glLoadIdentity();
    glBegin( GL_LINES );
    
    for( x = 1 ; x < width+2 ; x++ ){
        glColor3f( 0.0, 0.0, 1.0 );
        glVertex2f( x*10, 10.0 );
        glVertex2f( x*10, height*10+10.0 );
    }
    for( x = 1 ; x < height+2; x++ ){
        glVertex2f( 10.0 , x*10 );
        glVertex2f( width*10+10.0 , x*10 );
    }

    // glColor3f(1,0,0);
    // glVertex2f( 10, 10.0 );
    // glVertex2f( 20, 10.0 );

    glEnd();
}

void draw_maze()
{
    // below loop is just to remove some lines from grid & make maze more open
    for (int y = 1; y < height-1; y++)
    {
        for (int x = 1; x < width-1; x++)
        {
            int c=0;
            for (int i = 0; i < 4; i++)
                if (grid[y][x].path[i] == false)
                    c++;

            if (c == 3)
            {
                int ran = rand() % 3;
                if (ran == 0){
                    if(grid[y][x].path[UP] == false){
                        grid[y][x].path[UP] = true;
                        grid[y+1][x].path[DOWN] = true;
                    }
                    else{
                        grid[y][x].path[DOWN] = true;
                        grid[y-1][x].path[UP] = true;
                    }
                }
                else if (ran == 1){
                    if(grid[y][x].path[DOWN] == false){
                        grid[y][x].path[DOWN] = true;
                        grid[y-1][x].path[UP] = true;
                    }
                    else{
                        grid[y][x].path[RIGHT] = true;
                        grid[y][x+1].path[LEFT] = true;
                    }
                }
                else if (ran == 2){
                    if(grid[y][x].path[RIGHT] == false){
                        grid[y][x].path[RIGHT] = true;
                        grid[y][x+1].path[LEFT] = true;
                    }
                    else{
                        grid[y][x].path[LEFT] = true;
                        grid[y][x-1].path[RIGHT] = true;
                    }
                }
            }
        }
    }
    
    // making exit
    removeLine(exitX,exitY,exitDirection);

    // remove lines of grid where there is path
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width;  x++)
        {
            if (grid[y][x].path[UP] == true)
                removeLine(x,y,UP);
            if (grid[y][x].path[DOWN] == true)
                removeLine(x,y,DOWN);
            if (grid[y][x].path[LEFT] == true)
                removeLine(x,y,LEFT);
            if (grid[y][x].path[RIGHT] == true)
                removeLine(x,y,RIGHT);
        }
    }
    // cout<<startGame<<"\n";
}

GLuint LoadTexture( const char * filename )
{
  GLuint texture;
  int width, height;
  unsigned char * data;

  FILE * file;
  file = fopen( filename, "rb" );

  if ( file == NULL ) return 0;
  width = 1441;
  height = 980;
  data = (unsigned char *)malloc( width * height * 3 );
  fread( data, width * height * 3, 1, file );
  fclose( file );

  for(int i = 0; i < width * height ; ++i)
  {
    int index = i*3;
    unsigned char B,R;
    B = data[index];
    R = data[index+2];

    data[index] = R;
    data[index+2] = B;
  }

  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
  free( data );

  return texture;
}

vector<pair<pair<int,int>,int>> neighbours(int x, int y)
{
    vector<pair<pair<int,int>,int>> next;
    if (x+1<width && grid[y][x+1].visited == false) //right
        next.push_back({{x+1,y}, RIGHT});
    if (x-1>=0 && grid[y][x-1].visited == false)    // left
        next.push_back({{x-1,y}, LEFT});
    if (y+1<height && grid[y+1][x].visited == false)    // up
        next.push_back({{x,y+1}, UP});
    if (y-1>=0 && grid[y-1][x].visited == false)    //down
        next.push_back({{x,y-1}, DOWN});
    return next;
}

// generates maze
void dfs(int x,int y)
{
    // cout<<x<<" "<<y<<endl;
    if (x>= width || y>= height || x<0 || y<0) return;

    grid[y][x].visited = true;

    vector<pair<pair<int,int>,int>> next = neighbours(x,y);

    int len = next.size();
    while(len)
    {
        if(len == 0) break;
        int ran = rand() % len;
        
        if (next[ran].second == RIGHT)
        {
            grid[y][x].path[RIGHT] = true;
            grid[y][x+1].path[LEFT] = true;
        }
        else if (next[ran].second == LEFT)
        {
            grid[y][x].path[LEFT] = true;
            grid[y][x-1].path[RIGHT] = true;
        }
        else if (next[ran].second == UP)
        {
            grid[y][x].path[UP] = true;
            grid[y+1][x].path[DOWN] = true;
        }
        else if (next[ran].second == DOWN)
        {
            grid[y][x].path[DOWN] = true;
            grid[y-1][x].path[UP] = true;
        }

        dfs(next[ran].first.first,next[ran].first.second);

        next = neighbours(x,y);
        len = next.size();
    }

    return;
}

void HUD()
{
    // Score
    glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
    glRasterPos2i(10, 100);     //Top left corner of text
    string str = "Score: " + to_string(SCORE);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)str.c_str()); // 2nd argument must be const unsigned char*
    
    // Health
    glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
    glRasterPos2i(80, 95);     //Top left corner of text
    string health = "Health: " + to_string(ball1.health);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)health.c_str()); // 2nd argument must be const unsigned char*
    
    // Task2
    if (task2.finish == 0) glColor4f(0.6f,0.0f,0.4f, 0.0f);
    else glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
    glRasterPos2i(10, 95);
    string t2 = "Task2";
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char *)t2.c_str());
}

void collision()
{
    // cout<<ball1.curr_x<<" "<<task2.curr_x<<"\n";
    if (task2.finish == 0 && ball1.curr_x+3 == task2.curr_x && ball1.curr_y+3 == task2.curr_y)
    {
        task2.finish = 1;
            for (int i = 0; i < 4; i++)
            {
                obs[i].curr_x = (rand() % 8)*10+15;
                obs[i].curr_y = (rand() % 8)*10+15;
            }
    }
    if(task2.finish == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            if (obs[i].finish == 0 && ball1.curr_x+3 == obs[i].curr_x && ball1.curr_y+3 == obs[i].curr_y)
            {
                obs[i].finish = 1;
                if(obs[i].sign == 1) SCORE += 100;
                else {
                    SCORE -= 50;
                    ball1.health--;
                }
            }   
        }
    }
}

void display ()
{
    glClearColor( 1, 1, 1, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT );
    glColor3f( 0.0, 1.0, 1.0 );

    GLuint texture;
    texture = LoadTexture( "../source/textures/back2.bmp" );

    // int view_Left = 10, view_Right = 90, view_Bottom = 10, view_Up = 90;
    int view_Left = 0, view_Right = 100, view_Bottom = 0, view_Up = 110;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(view_Left, view_Bottom);

    glTexCoord2f(1, 0);
    glVertex2f(view_Right, view_Bottom);

    glTexCoord2f(1, 1);
    glVertex2f(view_Right, view_Up);

    glTexCoord2f(0, 1);
    glVertex2d(view_Left, view_Up);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    HUD();

    collision();

    ball1.draw(ball1.curr_x,ball1.curr_y);
    // ball1.draw(12,12);

    draw_grid();
    if (startGame == 0)
    {
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        glRasterPos2i(40, 47);            //Top left corner of text
        const unsigned char* t = reinterpret_cast<const unsigned char *>("Press enter to start");
        glutBitmapString(GLUT_BITMAP_HELVETICA_18,t);
    }
    
    if (startGame==1) 
    {
        draw_maze();
        if(task2.finish == 0) task2.draw(2);
        else
        {
            for (int i = 0; i < 2; i++)
                if(obs[i].finish==0) obs[i].draw(1);
            for (int i = 2; i < 4; i++)
                if(obs[i].finish==0) obs[i].draw(0);
        }
    }
    
    glFlush();
}

void key (unsigned char key, int x, int y)
{
    switch (key) {
        case 'q':
            exit(0);
            break;
        case 13:
            startGame = 1;
            break;
    }
    display();
}

void movement (int key, int x, int y )
{
    int gridX = (ball1.curr_x-12)/10;
    int gridY = (ball1.curr_y-12)/10;
    // cout<<gridX<<" "<<gridY<<endl;
    if (startGame == 0) return;
    switch (key) {
        case GLUT_KEY_RIGHT:
            // if (ball1.curr_x+10 < 12+10*width)
            if (grid[gridY][gridX].path[RIGHT] == true)
                ball1.curr_x+=10;
            break;
        case GLUT_KEY_LEFT:
            // if (ball1.curr_x-10 >= 12)
            if (grid[gridY][gridX].path[LEFT] == true)
                ball1.curr_x-=10;
            break;
        case GLUT_KEY_UP:
            // if (ball1.curr_y+10 < 12+10*height)
            if (grid[gridY][gridX].path[UP] == true)
                ball1.curr_y+=10;
            break;
        case GLUT_KEY_DOWN:
            // if (ball1.curr_y-10 >= 12)
            if (grid[gridY][gridX].path[DOWN] == true)
                ball1.curr_y-=10;
            break;
    }

    display();
}

// sets initial start of character
void choose_start()
{
    int initialX = rand()%(width);
    int initialY = rand()%(height);
    dfs(initialX, initialY);

    // exit position
    int ran = rand() % 8;
    if(ran/2 == 0){
        if(ran%2==0) 
        exitX = 0, exitY = height-1, exitDirection = UP;
        else 
        exitX = 0, exitY = height-1, exitDirection = LEFT;
    }
    else if (ran/2 == 1){
        if(ran%2==0)
        exitX = width-1, exitY = height-1, exitDirection = UP;
        else 
        exitX = width-1, exitY = height-1, exitDirection = RIGHT;
    }
    else if (ran/2 == 2){
        if(ran%2==0)
        exitX = 0, exitY = 0, exitDirection = LEFT;
        else
        exitX = 0, exitY = 0, exitDirection = DOWN;
    }
    else{
        if(ran%2==0)
        exitX = width-1, exitY = 0, exitDirection = RIGHT;
        else
        exitX = width-1, exitY = 0, exitDirection = DOWN;
    }
}

void init ()
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    
    gluOrtho2D( 0, 100, 0, 110 );
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv) {
    srand(time(0));

    cout<<"Width of grid: "<< width<<"\n";
    cout<<"Height of grid: "<< height<<"\n";

    choose_start();
    task2.curr_x = (rand() % 8)*10+15;
    task2.curr_y = (rand() % 8)*10+15;
    ball1.curr_x = (rand() % 8)*10+12;
    ball1.curr_y = (rand() % 8)*10+12;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize ( 700, 700 );
    glutInitWindowPosition(100,100);
    glutCreateWindow("Among Us");

    init();

    glutDisplayFunc(display);   // glutDisplayFunc whenever your window must be redrawn
    glutKeyboardFunc(key);
    glutSpecialFunc(movement);
    glutMainLoop(); // glutMainLoop calls your glutDisplayFunc callback over and over
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
