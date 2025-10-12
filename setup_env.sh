#!/bin/bash
# This is a comment

echo "Installing dependencies..."

sudo apt update
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi gdb-multiarch -y
sudo apt install build-essential make git -y
sudo apt install openocd -y


echo "Done!"