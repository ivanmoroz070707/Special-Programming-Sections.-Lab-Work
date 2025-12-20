#!/usr/bin/env bash
set -e
if [ "$EUID" -ne 0 ]; then
echo "This script may require sudo for package installation. Running with sudo is recommended.\n"
fi


# Debian/Ubuntu example
sudo apt update
sudo apt install -y build-essential cmake libopencv-dev pkg-config git


echo "Preinstall finished. You can run ./build.sh"


echo "Downloading DNN model files..."

# Файл архітектури (Prototxt)
wget -N https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt

# Файл ваг (Caffemodel)
wget -N https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/res10_300x300_ssd_iter_140000.caffemodel

echo "Preinstall finished. DNN models downloaded. You can run ./build.sh"
