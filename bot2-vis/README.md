# RobotLocomotion Fork of bot2-vis

*C library for visualizing data with OpenGL and GTK*

![CI](https://github.com/RobotLocomotion/libbot2/workflows/CI/badge.svg)

## Required Dependencies

* bot2-core
* C compiler that supports C99 (Clang or GCC recommended)
* CMake 3.10 and above (build and install) or 3.5 and above (use)
* FreeGLUT (Linux) or GLUT (macOS)
* GDK and GTK 3.0 and above with X11 support
* GLib and GObject 2.32 and above
* JPEG
* LCM 1.4 and above
* libpng
* Linux or POSIX-compliant operating system (macOS Mojave 10.14 and above or
  Ubuntu 18.04 "Bionic" and above recommended)
* Make, Ninja, or Xcode
* OpenGL (GLVND not supported)
* pkg-config
* POSIX mathematics library
* POSIX threads
* X window system
* zlib 1.1 and above

## Build and Install

```
git clone --branch drake https://github.com/RobotLocomotion/libbot2.git
mkdir libbot2-build
cd libbot2-build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/libbot2/install -DWITH_BOT_VIS=ON ../libbot2
make install
```

## Use

CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# ...

list(APPEND CMAKE_PREFIX_PATH /path/to/libbot2/install)
find_package(bot2-vis CONFIG REQUIRED)
```

## License

bot2-vis is free software: you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

bot2-vis is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with bot2-vis. If not, see <https://www.gnu.org/licenses/>.
