#!/usr/bin/env bash

build() {
    echo building R-Type with cmake...
    cmake -S . -B build/
    make -C build/
}

run() {
    echo "building R-Type..."
    make -C build/
}

clean() {
    echo "clearing R-Type..."
    cmake --build build --target clean
    rm -f r-type_client
    rm -f r-type_server
}

case "$1" in
    build)
        build
        ;;
    run)
        run
        ;;
    clean)
        clean
        ;;
    *)
        echo "Usage: $0 {build|run|clean}"
        exit 1
        ;;
esac
