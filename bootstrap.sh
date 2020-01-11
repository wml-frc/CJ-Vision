#!/bin/sh

# Enable password-less sudo for everyone
echo "%sudo ALL=(ALL) NOPASSWD:ALL" | sudo tee -a /etc/sudoers

# Install required packages
sudo apt-get update
sudo apt-get install -y avahi-daemon libnss-mdns v4l-utils imagemagick

# These two arn't technically required. But just in case you use a function not supported from windows to linux
sudo apt-get install libqt4-dev
sudo apt-get install libgtk2.0-dev
sudo apt-get install libopencv-*

# Add vision user and password
sudo adduser vision --disabled-password --gecos ""
sudo adduser vision sudo
sudo adduser vision video
sudo adduser vision audio
echo "vision:CJfrc" | sudo chpasswd

# Set hostname, user and password
echo CJvision | sudo tee /etc/hostname
echo "127.0.0.1 CJvision" | sudo tee /etc/hosts

# Set Team #
echo Team Number?
read teamnum < /dev/tty
mkdir -p /home/vision
echo $teamnum | sudo tee /home/vision/team
sudo chown vision /home/vision/team
sudo chmod 777 /home/vision/team

# Reboot to commit changes
sudo reboot