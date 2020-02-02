#!/bin/sh

# Enable password-less sudo for everyone
echo "%sudo ALL=(ALL) NOPASSWD:ALL" | sudo tee -a /etc/sudoers

# Install required packages
sudo apt-get update
sudo apt-get install -y avahi-daemon libnss-mdns v4l-utils imagemagick

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
