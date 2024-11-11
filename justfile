build:
    cmake -S . -B build/
    { cd build && make; }
clean:
    rm -f r-type_client
    rm -f r-type_server
