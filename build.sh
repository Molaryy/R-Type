#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 {build | clean}"
    exit 1
fi

case "$1" in
    build)
        cmake -S . -B build
        cmake --build build
        ;;
    clean)
        cmake --build build --target clean-all
        rm -rf build
        ;;
    *)
        echo "Usage: $0 {build|clean}"
        exit 1
        ;;
esac