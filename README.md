# OpenGL Tutorial
I Have created this directory for anyone who wants to learn the basics of OpenGL.
I Wrote the code in Visual Code in a Linux Ubuntu environment, so the environment is all configured and setup for Linux users on that end 😄.
Windows users have Visual Studio so don't worry about it. I Used CMake to build all the mini projects (directories) and it's super easy to learn and use so it's another thing you can learn from this directory, it should work on any operating system, if not open an issue and i'll fix it.
Anyway the executable is included already so no need for all of the compilation and linking headache.

## Clone Repository
From the terminal run one of the options:<br>
- HTTPS: ``git clone https://github.com/daniel752/opengl-tutorial.git``<br>
- SSH: ``git@github.com:daniel752/opengl-tutorial.git``

## Build And Run Executable
### Build
- From the root directory for that specific tutorial ([path]/opengl-tutorial/draw-shape/, [path]/opengl-tutorial/init-window/, etc) run:<br> 
``mkdir build && cd build``<br>
- After build directory was created run:<br>
``cmake ..``<br>
- After that cmake will generate a Makefile in build/ directory,from build/ directory run:<br>
 ``make``<br>
## Run
From the 'build/' directory run:<br>
``./main.o``
