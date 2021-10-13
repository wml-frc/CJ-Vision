#!/bin/sh

#
# Install OpenCV for Pi/Tinker
#

printf '\e[1;32m'
printf "\nUpdating/Upgrading Machine\n"
printf '\e[1;0m'

sudo apt-get -y update
sudo apt-get -y upgrade

# Install Libraries
printf '\e[1;32m'
printf "\nInstalling Libraries\n"
printf '\e[0m'
sudo apt-get -y install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get -y install python3.5-dev python3-numpy libtbb2 libtbb-dev
sudo apt-get -y install libjpeg-dev libpng-dev libtiff5-dev libjasper-dev libdc1394-22-dev libeigen3-dev libtheora-dev libvorbis-dev libxvidcore-dev libx264-dev sphinx-common libtbb-dev yasm libfaac-dev libopencore-amrnb-dev libopencore-amrwb-dev libopenexr-dev libgstreamer-plugins-base1.0-dev libavutil-dev libavfilter-dev libavresample-dev
sudo apt-get -y install libopencv-dev

# Get opencv
printf '\e[1;32m'
printf "\nInstalling OpenCV\n"
printf '\e[0m'
cd /opt
sudo git clone https://github.com/opencv/opencv.git
sudo git clone https://github.com/opencv/opencv_contrib.git

# Cmake setup/build opencv
printf '\e[1;32m'
printf "\nSetting up Cmake\n"
printf '\e[0m'
cd opencv
sudo mkdir release
cd release
sudo cmake -D BUILD_TIFF=ON -D OPENCV_GENERATE_PKGCONFIG=ON -D WITH_CUDA=OFF -D ENABLE_AVX=OFF -D WITH_OPENGL=OFF -D WITH_OPENCL=OFF -D WITH_IPP=OFF -D WITH_TBB=ON -D BUILD_TBB=ON -D WITH_EIGEN=OFF -D WITH_V4L=OFF -D WITH_VTK=OFF -D BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/opt/opencv_contrib/modules /opt/opencv/
sudo make -j4
sudo make install
sudo ldconfig

# Output CV version
printf '\e[1;32m'
printf "\nOpenCV Version\n"
printf '\e[0m'
pkg-config --modversion opencv
pkg-config --modversion opencv4
