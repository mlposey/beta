#!/bin/bash
# This script archives a release of the project for
# a 64-bit Linux environment.
# It accepts a single argument: the semantic version.
#
# usage: sh archive-release.sh <version>

ARCHIVE=beta-$1-linux-x86_64.tar.gz

mkdir wrkdir && cd wrkdir

git clone https://github.com/mlposey/beta.git
mkdir bin
cp -r beta src

cd beta
cmake .
make -j4 beta
cd ..

mv beta/build/release/* bin/
tar -czf $ARCHIVE bin src
rm -rf beta src bin

cd ..
mv wrkdir/$ARCHIVE .
rm -rf wrkdir
