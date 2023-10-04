cd "$(dirname "$0")"

build(){
    cd build
    cmake ..
    make
    cd ..
}

build