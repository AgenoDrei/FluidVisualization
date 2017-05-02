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

## Run

For help
>$ ./FluidVisualization --help

Running the application with specified inputfile
>$ ./FluidVisualization --input-file /path/to/file.dat

Running the application with specified inputfile and starting with specific algorithm
>$ ./FluidVisualization --input-file /path/to/file.dat --algorithm NameOfAlgorithm

## Keys

- W/A/S/D - Move
- N/M or PageUp/PageDown - Up/Down
- L - Toggle Algorithm
- R - Toggle Reflection
- T - Toggle Shadows
- F/G - [TextureSlicing] Increase / Decrease Number of slices
- I/O - [Ray Casting] Increase / Decrease Ray Length
