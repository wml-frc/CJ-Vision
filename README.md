# CJ-Vision

## Layered computer vision platform with C++ UDP networking for data transfer and embedded tools for easy deployment to UNIX-based processors.

#### Build Status
[![Build Status](https://dev.azure.com/ConnorBuchel0890/WML/_apis/build/status/wml-frc.CJ-Vision?branchName=master)](https://dev.azure.com/ConnorBuchel0890/WML/_build/latest?definitionId=12&branchName=master)

### Features:
- Platform to build Computer Vision (CV) applications in C++ on

- Common OpenCV wrapper functions

- Allows local debugging with webcam

- Your choice in coprocessor, RPI, Jetson, Tinker Board, etc...

- Uses layering system for programmable design

- Debian UNIX-based deployment (uses Embedded tools for building/deploying to Debian UNIX-based systems)

- Socket networking (sends CV data through custom socket programming https://github.com/wml-frc/UDP_TransferNT)

- Stream MJPEG Video to browser or MJPEG Viewer

# Local Setup

## Installing

### All platforms
1. Clone or download the project.

### Windows
1. Download Java (JDK 11 for best support https://www.oracle.com/java/technologies/javase-jdk11-downloads.html)

2. "Desktop development with C++" from VS required for local debugging https://visualstudio.microsoft.com/downloads/

2. Once complete, download or clone the project either into existing code (using the lib as a submodule) or download as is.
Then inside the root dir of the lib run `gradlew generateFramework build`
If all downloaded correctly, it should read `BUILD SUCCESSFUL`

### Linux
1. Linux requires the build-essentials (GCC, CMake, etc... along with OpenCV)

2. If you are running a Debian-based machine, then an easier option is to use the included bootstrap for local machines. Use `sudo wget -qO - https://github.com/wml-frc/CJ-Vision/blob/master/bootstrap/bootstrapLocal.sh?raw=1 | bash`

1. Alternatively you can install it manually
```shell
# Update & Install required packages
sudo apt-get update
sudo apt-get install -y avahi-daemon libnss-mdns v4l-utils imagemagick
sudo apt-get install -y build-essential cmake cmake-curses-gui pkg-config
sudo apt-get install -y libjpeg-dev libtiff5-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libeigen3-dev libxvidcore-dev libx264-dev libgtk2.0-dev
sudo apt-get install -y libv4l-dev v4l-utils
sudo modprobe bcm2835-v4l2
sudo apt-get install -y libatlas-base-dev gfortran
```

2. OpenCV will also need to be installed on your local machine to run this program. (Bootstrap script installs it for you. Skip this step if used).
- You can install it manually from OpenCV's official page https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html

### Mac
1. Download Xcode from the app store https://apps.apple.com/au/app/xcode/id497799835?mt=12

2. You can utilize the bootstrap script using `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/wml-frc/CJ-Vision/cj-vision-2.0/bootstrap/bootstrapLocal_mac.sh)"`

1. Or you can install OpenCV manually https://medium.com/@jaskaranvirdi/setting-up-opencv-and-c-development-environment-in-xcode-b6027728003


# Coprocessor
## [Coprocessor Setup](docs/coproc.md)
Get started with the coprocessors.

## [Getting Started](docs/gettingStarted.md)
Get started with the library.

<sub><sup>readme written by [@CJBuchel](https://github.com/CJBuchel), 14/10/21</sup></sub>
