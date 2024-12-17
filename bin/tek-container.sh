#!/usr/bin/env bash

docker run -it --rm -v "$(pwd):/home/project" -w /home/project epitechcontent/epitest-docker:latest /bin/bash
