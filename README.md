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
>$ mkdir build && cd build

Then compile via CMake and run the application

>$ cmake .. && make

## Run

Inside the build-directory, for help run
>$ ./FluidVisualization --help

Running the application with specified inputfile
>$ ./FluidVisualization --input-file /path/to/file.dat

Running the application with specified inputfile and starting with specific algorithm
>$ ./FluidVisualization --input-file /path/to/file.dat --algorithm NameOfAlgorithm

Running with a specified config.ini file (inside build folder):
>$ ./FluidVisualization

A config.ini file will have the following structure:

>[Main]<br>
>InputFile=/path/to/inputFile.dat<br>
>StartAlgorithm={Marching Cubes / Volume Ray Casting / Texture Slicing 3D}<br>
>Interpolation={0 / 1}
>
>[MarchingCube]<br>
>Reflection={0 / 1}<br>
>Shadow={0 / 1}
>
>[TextureSlicing3D]<br>
>Reflection={0 / 1}<br>
>NumSlices={1 / 2 / ...}
>
>[RayCasting]<br>
>Reflection={0 / 1}<br>
>Shadow={0 / 1}

where each value in curly braces may be chosen and writen plain (without braces: e.g. *Reflection=1*).
All options except for _InputFile_ are optional. 
Note that *StartAlgorithm* has the displayed Algorithm-Names as Input, NOT the ones displayed in brackets in the config file.

## Keys

- W,A,S,D - Move
- N,M or PageUp,PageDown - Up/Down
- L - Toggle Algorithm
- K - Next timestep (if multiple timesteps available)
- R - Toggle Reflection
- I,O - Increase/Decrease level of detail (Ray Casting: Ray-length, Texture Slicing: Number of slices)
- T - Toggle Shadows
- P - Toggle Light-Positioning (for direction of shadows)

# Status of Application

3 Algorithms available

- Marching Cubes
    - implements normals, reflection and shadows
- Raycasting
    - implements normals, reflection and shadows with multiple light directions
- Texture Slicing
    - implements normals and reflection
