# RobotLocomotion Fork of bot2-frames

*C library that manages a set of coordinate frames*

bot2-frames reads the coordinate frame setup information from a parameter file
stored by bot2-param.

## Parameter File Format

bot2-frames assumes a specific parameter file format, an example of which is
shown below:

```
coordinate_frames {
  root_frame = "local";  # a root_frame must be defined
  body {
    relative_to = "local";
    history = 1000;                    # number of past transforms to keep
                                       # around
    update_channel = "BODY_TO_LOCAL";  # transform updates will be listened for
                                       # on this channel
    initial_transform {
      translation = [ 0, 0, 0 ];  # (x,y,z) translation vector
      quat = [ 1, 0, 0, 0 ];      # may be specified as a quaternion, rpy,
                                  # Rodrigues, or axis-angle
    }
  }
  laser {
    relative_to = "body";
    history = 0;          # if set to 0, transform will not be updated
    update_channel = "";  # ignored since history = 0
    initial_transform {
      translation = [ 0, 0, 0 ];
      rpy = [ 0, 0, 0 ];
    }
  }
  camera {
    relative_to = "body";
    history = 0;
    initial_transform {
      translation = [ 0, 0, 0 ];
      rodrigues = [ 0, 0, 0 ];
    }
  }
  # ...
}
```

## Required Dependencies

* bot2-core
* bot2-param
* C/C++ compiler that supports C++98 and C99 (Clang or GCC recommended)
* CMake 3.10 and above (build and install) or 3.5 and above (use)
* GLib and GObject 2.32 and above
* LCM 1.4 and above
* Linux or POSIX-compliant operating system (macOS Mojave 10.14 and above or
  Ubuntu 18.04 "Bionic" and above recommended)
* Make, Ninja, or Xcode

## Optional Dependencies

* bot2-lcmgl
* bot2-vis
* FreeGLUT (Linux) or GLUT (macOS)
* GTK 3.0 and above
* Java SE Development Kit 8 and above (11 and above recommended)
* OpenGL (GLVND not supported)
* pkg-config
* Python 3.6 and above

## Build and Install

```
git clone --branch drake https://github.com/RobotLocomotion/libbot2.git
mkdir libbot2-build
cd libbot2-build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/libbot2/install -DWITH_BOT_FRAMES=ON ../libbot2
make install
```

## Use

CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# ...

list(APPEND CMAKE_PREFIX_PATH /path/to/libbot2/install)
find_package(bot2-frames CONFIG REQUIRED)
```

## License

bot2-frames is free software: you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

bot2-frames is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with bot2-frames. If not, see <https://www.gnu.org/licenses/>.
