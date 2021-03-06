#!/bin/bash

#
# Copyright (C) 2013 Jacob McIntosh <nacitar at ubercpp dot com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cd "$(dirname "$0")"

gtest_svn="http://googletest.googlecode.com/svn/trunk/"
gstyle_svn="http://google-styleguide.googlecode.com/svn/trunk/"
function die() {
  echo "$@" 1>&2
  exit 1
}

# Get 3rdparty libraries
pushd 3rdparty || die "Missing 3rdparty directory; this shouldn't happen!"
# Get googletest
gtest_dir="googletest"
if pushd "$gtest_dir"; then
  echo "Attempting to update googletest:"
  svn update || echo "Error updating googletest; ignoring."
  gtest_operation="Updated"
  popd  # $gtest_dir
else
  echo "Attempting to check out googletest:"
  svn checkout "$gtest_svn" "$gtest_dir" || die "Failed to fetch googletest."
  gtest_operation="Checked out"
fi
echo
# Get google-styleguide
gstyle_dir="google-styleguide"
if pushd "$gstyle_dir"; then
  echo "Attempting to update google-styleguide:"
  svn update || echo "Error updating google-styleguide; ignoring."
  gstyle_operation="Updated"
  popd  # $gstyle_dir
else
  echo "Attempting to check out google-styleguide:"
  if svn checkout "$gstyle_svn" "$gstyle_dir"; then
    gstyle_operation="Checked out"
  else
    echo "Failed to fetch google-styleguide; ignoring."
    gstyle_operation="Failed to check out"
  fi
fi
popd  # 3rdparty
echo

# Make a brand new native build environment
[ -d build ] && rm -rf build
if mkdir build && pushd build; then
  echo "Attempting to create native build environment."
  cmake \
      -DCMAKE_BUILD_TYPE=Release \
      ..
  popd  # build
  echo
else
  die "Failed to setup native build environment in build/"
fi

# Make a brand new native build environment using clang
[ -d clangbuild ] && rm -rf clangbuild
if mkdir clangbuild && pushd clangbuild; then
  echo "Attempting to create native build environment for clang."
  CC=clang CXX=clang++ cmake \
      -DCMAKE_BUILD_TYPE=Release \
      ..
  popd  # clangbuild
  echo
else
  die "Failed to setup native clang build environment in clangbuild/"
fi

# Make a brand new windows cross build environment
[ -d winbuild ] && rm -rf winbuild
if mkdir winbuild && pushd winbuild; then
  echo "Attempting to create cross build environment."
  cmake \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw32_toolchain.cmake \
      -DSTATIC_RUNTIME=1 \
      ..
  popd  # winbuild
else
  die "Failed to setup cross build environment in winbuild/"
fi
echo

# Summary
echo "Operations:"
echo "- $gtest_operation googletest in 3rdparty/$gtest_dir"
echo "- $gstyle_operation google-style in 3rdparty/$gstyle_dir"
echo "- Prepared native environment in build/"
echo "- Prepared clang native environment in clangbuild/"
echo "- Prepared cross environment in winbuild/"
