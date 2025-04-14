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

# This script test libbot2's ubuntu package

set -euxo pipefail

if [[ $? -eq 0 ]]; then
  readonly timestamp=$(date -u +%Y%m%d)
else
  readonly timestamp=$1
fi

pushd /tmp

# Install packages.
export DEBIAN_FRONTEND=noninteractive
apt-get update -qq
apt-get install --no-install-recommends -y devscripts gdebi
gdebi -n lcm_1.4.0-gabdd8a2_amd64.deb
gdebi -n "libbot2_0.0.1.${timestamp}-1_amd64.deb"

popd

# Check that files are installed.
readonly executables=(bot-wavefront-viewer bot-spy bot-rwx-viewer bot-procman-sheriff bot-procman-deputy bot-ppmsgz bot-param-tool bot-param-server bot-param-dump bot-log2mat bot-lcm-who bot-lcm-tunnel bot-lcm-logsplice bot-lcm-logfilter bot-lcmgl-viewer)

# Find missing dependency. We could look for "not found", but grepping
# "found" is easier and sufficient.
missing_libraries() {
    if [[ $(ldd "$1" | grep -c found) != 0 ]]; then
      echo "Missing libraries for $1:"
      ldd "$1" | grep found
      exit 1
    fi
}

export PATH="${PATH}:/opt/libbot2/0.0.1.${timestamp}/bin"

for ex in "${executables[@]}"; do
  if [[ -z "${ex}" ]]; then
    echo "${ex} not found."
  else
    # We cannot simply run each executable as some require inputs that we
    # don't have, so we simply check that they have all their dependencies
    # fulfilled.
    if [[ $(file "$(command -v "${ex}")" | grep -c "ELF 64-bit LSB .*, x86-64") == 1 ]]; then
      hardening-check $(command -v "${ex}") || true
      # Test that executables find all their dependencies
      missing_libraries "$(command -v "${ex}")"
    # else skip for now
    fi
  fi
done

# Test library dependencies
for i in /opt/libbot2/"0.0.1.${timestamp}"/lib/libbot2-*.so; do
  hardening-check "${i}" || true
  missing_libraries "${i}"
done

for i in /opt/libbot2/"0.0.1.${timestamp}"/lib/liblcmtypes_bot2-*.a; do
  hardening-check "${i}" || true
done

# Test python modules
export PYTHONPATH=${PYTHONPATH:+${PYTHONPATH}:}/opt/libbot2/"0.0.1.${timestamp}"/lib/python3/dist-packages:/opt/lcm/1.4.0/lib/python3/dist-packages
readonly python_modules=(lcm bot_procman bot_param bot_log2mat bot_lcmgl bot_frames bot_core)

for i in "${python_modules[@]}";do
  python3 -c "import ${i}"
done
