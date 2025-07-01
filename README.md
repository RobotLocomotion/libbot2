# RobotLocomotion Fork of libbot2

The libbot2 project provides a set of libraries, tools, and algorithms that are
designed to facilitate robotics research. Among others, these include
convenience functions for maintaining coordinate transformations, tools for
parameter management and process administration, and a 3D visualization library.
Initially developed at MIT for the 2007 DARPA Grand Challenge, libbot2 is
designed to provide a core set of capabilities for robotics applications and has
been utilized on a number of different robotic vehicles.

## Sub-Projects

libbot2 is a collection of several sub-projects. Some sub-projects may depend on
others.

### bot2-core

C library with some simple but useful routines and a set of core message types.

### bot2-frames

C library that manages a set of coordinate frames.

### bot2-lcm-utils

LCM utility programs (tunnel LCM over TCP, chop or splice log files, etc.)

### bot2-lcmgl

C library and viewer for transmitting and rendering OpenGL commands over LCM.

### bot2-param

C library that manages parameter files.

### bot2-procman

Process management tools for controlling many processes on one or many
workstations.

### bot2-vis

C library for visualizing data with OpenGL and GTK.

## Dependencies

See individual sub-projects for required and optional dependencies.

### Provisioning Scripts

Run one of the following scripts to install the required and optional
dependencies for all sub-projects to your system:

#### macOS Using Homebrew

[scripts/setup/mac/install_prereqs](https://github.com/RobotLocomotion/libbot2/tree/drake/scripts/setup/mac)

#### Ubuntu 18.04 "Bionic"

[scripts/setup/linux/ubuntu/bionic/install_prereqs](https://github.com/RobotLocomotion/libbot2/tree/drake/scripts/setup/linux/ubuntu/bionic)

#### Ubuntu 20.04 "Focal"

[scripts/setup/linux/ubuntu/focal/install_prereqs](https://github.com/RobotLocomotion/libbot2/tree/drake/scripts/setup/linux/ubuntu/focal)


You will also need to manually install LCM from <https://lcm-proj.github.io/>.

## Build and Install

```
git clone --branch drake https://github.com/RobotLocomotion/libbot2.git
mkdir libbot2-build
cd libbot2-build
export CMAKE_POLICY_VERSION_MINIMUM=3.5
cmake -DCMAKE_INSTALL_PREFIX=/path/to/libbot2/install ../libbot2
make install
```

## Use

CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# ...

list(APPEND CMAKE_PREFIX_PATH /path/to/libbot2/install)
find_package(libbot2 CONFIG REQUIRED)
```

## License

libbot2 is free software: you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

libbot2 is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with libbot2. If not, see <https://www.gnu.org/licenses/>.
