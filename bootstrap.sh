#!/bin/sh

OPENCV_VERSION='4.2.0'

# Enable password-less sudo for everyone
echo "%sudo ALL=(ALL) NOPASSWD:ALL" | sudo tee -a /etc/sudoers

# Update & Install required packages
sudo apt-get update
sudo apt-get install -y avahi-daemon libnss-mdns v4l-utils imagemagick
sudo apt-get install -y build-essential cmake cmake-curses-gui pkg-config
sudo apt-get install -y libjpeg-dev libtiff5-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libeigen3-dev libxvidcore-dev libx264-dev libgtk2.0-dev
sudo apt-get install -y libv4l-dev v4l-utils
sudo modprobe bcm2835-v4l2
sudo apt-get install -y libatlas-base-dev gfortran

# Download and setup OpenCV
wget -nc https://github.com/opencv/opencv/archive/${OPENCV_VERSION}.zip -O opencv_source.zip
rm -rf opencv-${OPENCV_VERSION}
unzip opencv_source.zip
cd opencv-${OPENCV_VERSION}
mkdir build
cd build

# Setup CMake in build file
cmake -D CMAKE_BUILD_TYPE=RELEASE \
  -D CMAKE_INSTALL_PREFIX=/usr/local \
  -D BUILD_WITH_DEBUG_INFO=OFF \
  -D BUILD_DOCS=OFF \
  -D BUILD_EXAMPLES=OFF \
  -D BUILD_TESTS=OFF \
  -D BUILD_opencv_ts=OFF \
  -D BUILD_PERF_TESTS=OFF \
  -D INSTALL_C_EXAMPLES=OFF \
  -D INSTALL_PYTHON_EXAMPLES=OFF \
  -D ENABLE_NEON=ON \
  -D WITH_LIBV4L=ON \
    ../
make -j4
sudo make install
sudo ldconfig


# Add vision user and password
sudo adduser vision --disabled-password --gecos ""
sudo adduser vision sudo
sudo adduser vision video
sudo adduser vision audio
echo "vision:CJfrc" | sudo chpasswd

# Set hostname, user and password
echo CJvision | sudo tee /etc/hostname
echo "127.0.0.1 CJvision" | sudo tee /etc/hosts

# Turn off WiFi and HDMI for power consumption
sudo tvservice --off
sudo ifconfig wlan0 down

# Set Team #
echo Team Number?
read teamnum < /dev/tty
mkdir -p /home/vision
echo $teamnum | sudo tee /home/vision/team
sudo chown vision /home/vision/team
sudo chmod 777 /home/vision/team

# Reboot to commit changes
sudo reboot
