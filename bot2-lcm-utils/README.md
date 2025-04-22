# RobotLocomotion Fork of bot2-lcm-utils

*LCM utility programs (tunnel LCM over TCP, chop or splice log files, etc.)*

## Required Dependencies

* C/C++ compiler that supports C++98 and C99 (Clang or GCC recommended)
* CMake 3.10 and above (build and install) or 3.5 and above (use)
* GLib 2.32 and above
* LCM 1.4 and above
* Linux or POSIX-compliant operating system (macOS Mojave 10.14 and above or
  Ubuntu 18.04 "Bionic" and above recommended)
* Make, Ninja, or Xcode
* POSIX mathematics library

## Optional Dependencies

* Java SE Development Kit 8 and above (11 and above recommended)
* pkg-config
* Python 3.6 and above

## Build and Install

```
git clone --branch drake https://github.com/RobotLocomotion/libbot2.git
mkdir libbot2-build
cd libbot2-build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/libbot2/install -DWITH_BOT_LCM_UTILS=ON ../libbot2
make install
```

## Use

CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# ...

list(APPEND CMAKE_PREFIX_PATH /path/to/libbot2/install)
find_package(bot2-lcm-utils CONFIG REQUIRED)
```

## License

bot2-lcm-utils is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

bot2-lcm-utils is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License along
with bot2-lcm-utils. If not, see <https://www.gnu.org/licenses/>.
