build:
    cmake -S . -B build
    cmake --build build
clean:
    cmake --build build --target clean-all
    rm -f r-type_client
    rm -f r-type_server
