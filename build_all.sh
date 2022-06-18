#!/bin/sh

build_service () {

    echo "building project $1"
    rm -rf ./$1/build
    mkdir -p "$1/build"

    cd "$1/build/"

    cmake -DCMAKE_BUILD_TYPE=Release ..
    make

    cd ../../

}

build_service main-service
build_service task-service

cd ../../../
