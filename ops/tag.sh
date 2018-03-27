#!/bin/bash
# This script creates a tag that matches the application version
# and pushes that tag to the Git origin.
#
# usage: tag.sh <version.h filepath>
set -e

VERSION_FILE=$1
CURRENT_VERSION=$(sed -rn 's/#define APP_VERSION "(.*)"$/\1/p' $VERSION_FILE)
git fetch --tags --force
LAST_TAGGED_VERSION=$(git tag -l | tail -n 1 | sed -rn 's/^v(.*)$/\1/p')

if [ "$CURRENT_VERSION" == "$LAST_TAGGED_VERSION" ]; then
    echo "current version matches last tag. skipping tag step..."
    exit 0
fi

CANONICAL_VERSION="v$CURRENT_VERSION"

git tag -a $CANONICAL_VERSION -m $CANONICAL_VERSION
echo "tagged $CANONICAL_VERSION"
echo "pushing tag..."
git push origin $CANONICAL_VERSION
echo "done."