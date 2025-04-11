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

FROM ubuntu:22.04
COPY ./install_prereqs /tmp/install_prereqs
RUN /tmp/install_prereqs \
  && rm -rf /var/lib/apt/lists/* \
  && rm -f /tmp/install_prereqs
