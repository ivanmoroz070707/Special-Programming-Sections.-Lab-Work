#!/usr/bin/env bash
set -e
if [ ! -f build/opencv_camera ]; then
echo "Binary not found. Run ./build.sh first."
exit 1
fi


./build/opencv_camera

