#!/usr/bin/env bash
set -e
if [ "$EUID" -ne 0 ]; then
echo "This script may require sudo for package installation. Running with sudo is recommended.\n"
fi


# Debian/Ubuntu example
sudo apt update
sudo apt install -y build-essential cmake libopencv-dev pkg-config git


echo "Preinstall finished. You can run ./build.sh"

