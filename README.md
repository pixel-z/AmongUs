# Among Us

Replica of the game Among-Us in 2D using OpenGL 3+.
Libraries used:
- glad
- glew
- glfw 
- glut (Utility Toolkit)

Make sure u have these libraries installed.

# Instructions
```
sudo apt -get  update
sudo apt -get install libglu1-mesa-dev freeglut3-dev mesa-common-dev

mkdir build
cd build
cmake ..
make
```

## Technicalities
- For maze generation, recursive backtracking is used (dfs).
- For imposter movement, it tries to use the shortest path to find the player (bfs).
- Classes and Object based approach is used. 
    1. `Ball` = represents character & imposters class
        - ball1 & imposter are the objects used.
    2. `Obstacles` = represents the coins and bombs which are created when task2 is finished.
        - obs[4] are objects used.
    3. `Button` = represents task buttons
        - task1 & task2 are the button objects.
    4. `Grid` = represents the 2d matrix of maze & the paths in it.
        - grid is object.

- When Task1 is completed then imposter vanishes.
- When Task2 is completed then coins and bombs are created randomly on the grid.
- Only one exit exists. Exit can only be used when all tasks are completed.
- Coins = SCORE + 100
- Bombs     
    = SCORE - 50    
    = Health - 2
- Timer of 30 seconds is implemented. Within this time game needs to be completed.

## Keys
- __enter__ =  to generate maze
- __q__ = to quit
- __UP__ = move UP
- __DOWN__ = move DOWN
- __RIGHT__ = move RIGHT
- __LEFT__ = move LEFT