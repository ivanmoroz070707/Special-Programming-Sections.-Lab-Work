#!/bin/bash


set -e


mkdir -p build
cd build


cmake ..


make -j$(nproc)
cp ../*.prototxt .
cp ../*.caffemodel .
echo "Build finished successfully!"

