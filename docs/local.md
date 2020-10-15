# Local Setup

## Installing

### Windows
1. Download Java (JDK 11 for best support "https://www.oracle.com/java/technologies/javase-jdk11-downloads.html")

2. Once complete, download or clone the project either into existing code (using the lib as a submodule) or download as is.
Then inside the root dir of the lib run `gradlew build -PdebugMode`
If all is downloaded correctly, it should read `BUILD SUCCESSFUL`

### Linux
1. Linux requires the build essentials (gcc, cmake etc. For the project to run, and it also requires OpenCV)

- Linux is the same intended platform as the target Coprocessor and is the most tested platform for this lib. You can utilize a modified bootstrap script for the install.
`sudo wget -qO - https://github.com/CJBuchel/CJ-Vision/blob/master/bootstrap/bootstrapLocal.sh?raw=1 | bash`

- Or you can install it manually using the below scripts

	In your terminal run:
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


- Finally you will also need to install opencv. You may be able to download using the following script used for coprocessors,
```shell
# Install OpenCV
sudo wget -qO - https://github.com/CJBuchel/CJ-Vision/blob/master/bootstrap/openCV.sh?raw=1 | bash
```

- If this doesn't work and or is not supported. You can check OpenCV's documentation for your needed download "https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html"

2. After installing the necessary packages and installs. Clone or download the project either as a submodule into existing code. Or as is.

- Then inside of the root dir run `./gradlew build -PdebugMode`
If all is setup correctly it should read `BUILD SUCCESSFUL`

### Mac

- Not yet tested
