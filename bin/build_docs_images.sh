#!/usr/bin/env bash

DOCKER_USERNAME=molaryy35
DOCKER_PASSWORD=$DOCKER_PASSWORD_GIT
IMAGE=doc-docusaurus

echo "$DOCKER_PASSWORD" | docker login -u "$DOCKER_USERNAME" --password-stdin

cd ../docs/docusaurus

docker buildx rm container-builder

docker buildx create \
  --name container-builder \
  --driver docker-container \
  --bootstrap --use

docker buildx build \
    --platform linux/amd64,linux/arm64 \
    -t "$DOCKER_USERNAME/$IMAGE:latest" \
    --push .
