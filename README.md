# CJ-Vision

## Layered computer vision platform with C++ UDP networking for data transfer and embedded tools for easy deployment to UNIX-based coprocessors.

#### Build Status
[![Build Status](https://dev.azure.com/ConnorBuchel0890/WML/_apis/build/status/wml-frc.CJ-Vision?branchName=master)](https://dev.azure.com/ConnorBuchel0890/WML/_build/latest?definitionId=12&branchName=master)

### Features:
- Complete framework allows you to build programs on top of the framework without the need for prior programming or other frameworks. E.g., FRC timed robot skeleton.

- Common OpenCV wrapper functions

- Allows local debugging with webcam

- Your choice in coprocessor, RPI, Jetson, Tinker Board, etc...

- Uses layering system for programmable design

- UNIX-based deployment (uses Embedded tools for building/deploying to UNIX based systems)

- Socket networking (sends CV data through custom socket programming)

# Local Setup

## Installing

### Windows
1. Download Java (JDK 11 for best support "https://www.oracle.com/java/technologies/javase-jdk11-downloads.html")

2. Once complete, download or clone the project either into existing code (using the lib as a submodule) or download as is.
Then inside the root dir of the lib run `gradlew generateFramework build`
If all downloaded correctly, it should read `BUILD SUCCESSFUL`

### Linux
1. Linux requires the build-essentials (GCC, CMake, etc... along with OpenCV)

2. If you are running a Debian-based machine, then an easier option is to use the included bootstrap for local machines. Use `sudo wget -qO - https://github.com/wml-frc/CJ-Vision/blob/master/bootstrap/bootstrapLocal.sh?raw=1 | bash`

3. Alternatively you can install it manually
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

4. OpenCV will also need to be installed on your local machine to run this program. You can use the included script.
```shell
# Install OpenCV
sudo wget -qO - https://github.com/wml-frc/CJ-Vision/blob/master/bootstrap/openCV.sh?raw=1 | bash
```

- Or you can install it manually from OpenCV's official page https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html


# Coprocessor
## [Coprocessor Setup](docs/coproc.md)
Get started with the coprocessors.

## [Getting Started](docs/gettingStarted.md)
Get started with the library.

<sub><sup>readme written by [@CJBuchel](https://github.com/CJBuchel), 21/09/21</sup></sub>
