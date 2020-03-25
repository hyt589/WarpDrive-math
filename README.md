

# Overview
This is a vector math library that includes standard vector and matrix operations. Note that this is still WIP and features have not been fully implemented yet.

This project is developed as the foundation of a future game engine project, currently named as "WarpDrive". Other components of the game engine such as physics simulation and renderer will be dependent on this project.

# Build Instruction
Make sure at least CMake 3.0.0 is installed.

Make a build directory:
```
mkdir build
```

Then generate the necessary build files:
```
cd build
CMake -G <build option> -DCMAKE_BUILD_TYPE=<build type>
```

where build option is dependent on your OS. On Windows PC this is `"Visual Studio 15 2017"`; on Unix based system (eg. OS X and Linux) this is `"Unix Makefiles"`.

If you generated with the `"Visual Studio 15 2017"` option, this will generate a `.sln` solution file, which can be imported to Microsoft Visual Studio IDE. If you are on Mac or Linux, this will generate  a `makefile`, which will allow you to build the project by invoking the command `make`.