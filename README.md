# Running the Project

## Install CMAKE
## Install GLEW
## Install GLUT
## Install GLM
## Install SOIL

The *Simple OpenGL Image Library*. 

We recommend getting the lib-code from (github)[https://github.com/kbranigan/Simple-OpenGL-Image-Library].
After cloning / downloading, run

>$ make<br>
>$ sudo make install

and you're done.

## Compile the code

Preferably compile the code into a separate directory, e.g.

>$ cd [FluidVisualizationMaster]<br>
>$ mkdir build && cd build<br>

Then compile via CMake and run the application

>$ cmake .. && make
>$ ./FluidVisualization

## Run

FluidVisualization FILE

## Keys

- W/A/S/D - Move
- N/M - Up/Down
- L - Toggle Algorithm
- R/T - Toggle Reflection
