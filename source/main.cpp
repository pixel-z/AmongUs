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
    glColor4f(1.0f, 0.0f, 0.0f, 0.0f);  //RGBA values of text color
    glRasterPos2i(10, 95);            //Top left corner of text
    const unsigned char* t = reinterpret_cast<const unsigned char *>("Tasks: "); // Since 2nd argument of glutBitmapString must be const unsigned char*
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,t);
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

    ball1.draw(ball1.curr_x,ball1.curr_y);
    // ball1.draw(12,12);

    draw_grid();

    if (startGame==1) draw_maze();
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
    if (startGame == 0) return;
    switch (key) {
        case GLUT_KEY_RIGHT:
            if (ball1.curr_x+10 < 12+10*width)
            ball1.curr_x+=10;
            break;
        case GLUT_KEY_LEFT:
            if (ball1.curr_x-10 >= 12)
            ball1.curr_x-=10;
            break;
        case GLUT_KEY_UP:
            if (ball1.curr_y+10 < 12+10*height)
            ball1.curr_y+=10;
            break;
        case GLUT_KEY_DOWN:
            if (ball1.curr_y-10 >= 12)
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
