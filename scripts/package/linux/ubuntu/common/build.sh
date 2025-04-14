#!/bin/bash

# This file is part of libbot2.
#
# libbot2 is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# libbot2 is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with libbot2. If not, see <https://www.gnu.org/licenses/>.

# This script is not designed to be run manually. It is used when creating a
# libbot2 debian package with the script called `package` which should be in
# the same directory.
# This script clones LCM's git repository, compiles and packages LCM, and
# compiles and packages libbot2.

set -euxo pipefail

script_dir=$(cd $(dirname ${BASH_SOURCE}) && pwd)
repo_dir=$(cd ${script_dir}/../../../../.. && pwd)

if [[ $? -eq 0 ]]; then
  readonly timestamp=$(date -u +%Y%m%d)
else
  readonly timestamp=$1
fi

# Configure, compile, and package libbot2
mkdir libbot2-build
pushd libbot2-build
cmake -DPACKAGE_LIBBOT2:BOOL=ON \
      -DCMAKE_BUILD_TYPE:STRING=Release \
      -DCMAKE_CXX_FLAGS:STRING="$(dpkg-buildflags --get CXXFLAGS) $(dpkg-buildflags --get CPPFLAGS)" \
      -DCMAKE_C_FLAGS:STRING="$(dpkg-buildflags --get CFLAGS) $(dpkg-buildflags --get CPPFLAGS)" \
      -DCMAKE_SHARED_LINKER_FLAGS:STRING="$(dpkg-buildflags --get LDFLAGS)" \
      -DPYTHON_EXECUTABLE:FILEPATH=/usr/bin/python3 \
      ${repo_dir}
make
cpack -G DEB
popd
mv libbot2-build/packages/"libbot2_0.0.1.${timestamp}-1_amd64.deb" "libbot2_0.0.1.${timestamp}-1_amd64.deb"
rm -rf libbot2-build
