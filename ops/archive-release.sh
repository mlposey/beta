#!/bin/bash
# This script archives a release of the project for
# a 64-bit Linux environment.
#
# usage: archive-release.sh <version.h filepath>
set -e

VERSION_FILE=$1
VERSION=$(sed -rn 's/#define APP_VERSION "(.*)"$/\1/p' $VERSION_FILE)
ARCHIVE=beta-$VERSION-linux-x86_64.tar.gz

mkdir wrkdir && cd wrkdir

git clone https://github.com/mlposey/beta.git
mkdir bin
cp -r beta src

cd beta
cmake .
make -j4 beta
cd ..

mv beta/build/release/* bin/
BDIR=beta-$VERSION
mkdir $BDIR
mv bin src $BDIR/
tar -czf $ARCHIVE $BDIR
rm -rf beta $BDIR

cd ..
mv wrkdir/$ARCHIVE .
rm -rf wrkdir
