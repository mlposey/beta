#!/bin/bash
# This script builds the docker image and pushes two tags
# (latest and the current version) to the registry.
# It requires the DOCKER_USER and DOCKER_PASSWORD variables.
#
# usage: build-image.sh <version.h filepath>
set -e

VERSION_FILE=$1
CURRENT_VERSION=$(sed -rn 's/#define APP_VERSION "(.*)"$/\1/p' $VERSION_FILE)

docker login -u $DOCKER_USER -p $DOCKER_PASSWORD

if (docker pull $DOCKER_USER/beta:$CURRENT_VERSION); then
    echo "An image with this tag already exists. Skipping deployment..."
    exit 0
fi

docker build -t $DOCKER_USER/beta .
docker tag $DOCKER_USER/beta $DOCKER_USER/beta:$CURRENT_VERSION

docker push $DOCKER_USER/beta
docker push $DOCKER_USER/beta:$CURRENT_VERSION
