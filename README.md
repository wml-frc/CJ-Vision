# ComputerJenerated-VisionTracking
Vision Tracking Sub-module library, To make vision tracking easier for other teams with pre-threaded processes and given known functions. Like X,Y center figures, distance detection, square and circle detection.

You may be wondering why it's called CJ Vision... well it was either that or Potato Vision, and i prefer self promotion. hehehehe

## Setting up Vision in your own project

- First you will need to install the submodule to your root directory. Navigate to your root dir and install via `git submodule add https://github.com/wml-frc/CJ-Vision/`. 

- If you havent already, you will need to make a vision dir, e.g `4788/src/coprocessor/`. You can build it how you like, Similar to your teamCode folder. With a cpp and include file. (For an example you can view https://github.com/CJBuchel/2020CodeBase/tree/CJ-Patch1/4788/src/coprocessor for more detail)

- Then you will need to apply the libraries `vision.gradle` with your own `src/coprocessor/build.gradle` and just add `apply from: rootProject.file('CJ-Vision/vision.gradle')` assuming you have placed the submodule in it's default location in the root dir. If you havent you will need to change the previous lines, and also edit the `CJ-Vision/vision.gradle` file inside the library. 

Change the following lines if you have placed the vision Library somewhere else.
```gradle
sources.cpp {
        source {
          srcDir 'src/main/cpp'
          srcDir rootProject.file('CJ-Vision/src/cpp')
        }
        exportedHeaders {
          srcDir 'src/main/include'
          srcDir rootProject.file('CJ-Vision/src/include')
        }
      }
```

## Using the library

- The code is still in it's prototype stage and will have improvements done over time. Also note this code is not designed to replace your own vision tracking code. Merely make the process a little easier. It's recommeded that you look through OpenCV's documentation before using this library. https://opencv.org/

- That all being said. The library comes with a few functions to start your project off with. To begin grabing the image from a camera you will need to add a cv::Mat as your origin image. Then create an instance of CJVision and call the Setup Function. 
```cpp
CJ::VisionTracking vision;

int ResWidth = 640, ResHeight = 480;
cv::Mat Image; // Origin Image

void curtin_frc_vision::run() {
	vision.SetupVision(&Image, 1, 60, ResHeight, ResWidth, 30, "TestCam", true);
}
```
The `SetupVision()` function sets up vision using the Input Image, the port number of the camera, reselution both Height and Width, The Exposure of the camera, the name of the camera and a true or false if you are tracking Retroreflective tape.

Note that while running the vision simulation on your computer, you will most likely need an external webcam plugged in via usb. Built in webcams sometimes don't work. Which is why often the port number will need to be 1 to connect to the second webcam. And for tinkerboards and Pi's you will most likely need to change the port number from 4 onwards. as ports 0-3 are taken by other processes.

Also due to a bug in Camera Servers, the exposure of the camera will not change when running locally. There is also a bug that i'm aware of where the exposure won't change when deployed to a tinkerboard. I'm working on a fix for this. In the meantime you may have to use `system("v4l2-ctl -d /dev/video4 --set-ctrl=exposure_absolute=1");` But i'm unaware if Pi's have the same issue. You may find the deafault exposure set in SetupVision() works for you. (The code is still in it's prototype stage) 


once complete just do a quick test run using the command `.\gradlew runVision` in your root dir.
If all goes well it will build perfectly fine and a window should open up. If it fails to build, you might need to recheck if the project has been setup correctly.

- The next common function to use is the main tracking functions, These will filter out specific colours using the HSV spectrum. The most basic of the two is the RetroTrack() function. You can use this as an easy mode if you are tracking Retro tape, and the colour green. You can then display the image using the included display function in a while loop. (Unlike every other function, the `Display()` function is not threaded. There are a few reasons for this, issues with the image being passed into a thread to be displayed via networktables and whatnot. But the main reason is your code needs to do something while the other threads are running. And in most cases the final product being displayed is mostly used to keep the main thread from ending in opencv.)

Below is an example using the RetroTrack and Display function.
```cpp
CJ::VisionTracking vision;

int ResWidth = 640, ResHeight = 480;

cv::Mat Image; // Origin Image
cv::Mat TrackingImage; // Imaged After it has been procesed

void curtin_frc_vision::run() {

	vision.SetupVision(&Image, 1, 60, ResHeight, ResWidth, 30, "TestCam", true);
	vision.RetroTrack(&TrackingImage, &Image, 2, 2);

  // __DESKTOP__ is defined in vision.gradle. So don't worry, it's available to use
  #ifdef __DESKTOP__ 
	std::cout << "Exposure Might be dissabled on local machine" << std::endl;
	#else
	system("v4l2-ctl -d /dev/video4 --set-ctrl=exposure_absolute=1");
	#endif


	while (true) {
		if (vision.Camera.cam.sink.GrabFrame(Image) != 0) {
      #ifdef __DESKTOP__
			vision.Output.Display("Origin Image", &Image);
			vision.Output.Display("Green Filtered Image", &TrackingImage);
			#else 
			vision.Camera.cam.output.PutFrame(TrackingImage);
			#endif
		}
	}
}
```

If your not wanting to track retro tape. Or your wanting some extra options. You can replace `RetroTrack()` with `CustomTrack()`. which will give you the options to change the colour detection range and the value range, cam exposure and the erosion/dilation values.
e.g for detecting green with low exposure
```cpp
vision.CustomTrack(&TrackingImage, &Image, 30, 70, 50, 255, -100, 2, 2);
```

After the basics are complete you can use some of the provided processing types currently available to speed up your vision tracking. e.g

```cpp
#include "vision.h"
#include <iostream>

CJ::VisionTracking vision;

int ResWidth = 640, ResHeight = 480;

double cx, cy;

cv::Mat Image; // Origin Image
cv::Mat ProcessingOutput; // Image in processing
cv::Mat TrackingImage; // Imaged After it has been procesed

void curtin_frc_vision::run() {

	vision.SetupVision(&Image, 1, 60, ResHeight, ResWidth, 30, "TestCam", false);
	vision.CustomTrack(&TrackingImage, &Image, 30, 70, 50, 255, 50, 2, 2);
	vision.Processing.visionHullGeneration.BoundingBox(&TrackingImage, &ProcessingOutput, &cx, &cy, 10);

  #ifdef __DESKTOP__ 
	std::cout << "Exposure Might be dissabled on local machine" << std::endl;
	#else
	system("v4l2-ctl -d /dev/video4 --set-ctrl=exposure_absolute=1");
	#endif

	while (true) {
		if (vision.Camera.cam.sink.GrabFrame(Image) != 0) {
			// Vision Outputing
			#ifdef __DESKTOP__
			vision.Output.Display("Origin Image", &Image);
			vision.Output.Display("Green Filtered Image", &TrackingImage);
			vision.Output.Display("Contour Detection", &ProcessingOutput);
			#else 
			vision.Camera.cam.output.PutFrame(Image);
			#endif
		}
	}
}
```
The above tracks green pixles at a regular exposure then detects and draws bounding boxes on everything that is green. It then sends the center values through network tables. So you can use it on your roborio and shuffleboard for autonomous.


# Getting started with your coprocessor
### 1. Grab the image:
  - [Tinkerboard Image](https://dlcdnets.asus.com/pub/ASUS/mb/Linux/Tinker_Board_2GB/20190821-tinker-board-linaro-stretch-alip-v2.0.11.img.zip)
  - [Raspberry Pi](https://downloads.raspberrypi.org/raspbian_lite_latest)
### 2. Flash the image onto a microsd card using etcher: https://etcher.io/
  - You will need to grab the .img file from the zip. The .img file is what you give to etcher.
  - After complete, insert the microSD card and startup the coprocessor. It may take a minute or two
### 3. SSH into the Coprocessor
  - On Mac/Linux, use `ssh username@hostname`
  - On Windows, download [puTTY](https://the.earth.li/~sgtatham/putty/latest/w64/putty-64bit-0.70-installer.msi) and use that.
  - Tinkerboard:
    - Hostname: `tinkerboard`, Username: `linaro`, Password: `linaro`
  - Raspberry Pi:
    - Hostname: `raspberrypi`, Username: `pi`, Password: `raspberry`

	- If your unable to ssh into the coprocessor for any reason. Like for instance, you exist at Curtin University where the network is encypted beyond the point of frustration. You can connect the tinkerboard up as a regular PC with a keyboard, mouse, screen & of course ethernet. Then login via that way and run the below command in the terminal.
### 4. Run the following command:
  - `wget -qO - https://github.com/wml-frc/CJ-Vision/blob/master/bootstrap.sh?raw=1 | bash`
  - When prompted input team number.
### 5. Your Coprocessor will now restart
### 6. Changes made
  - Hostname is now: `CJVision`, Username: `vision`, Password: `CJfrc`
  - If you want to check the status of your vision tracking. ssh into the coprocessor and run `systemctl vision`.
  - If you have an error in the vision tracking coprocessor side. run `sudo journalctl -u vision` to view output.
### 7. Run `./gradlew :TEAMNUMBER:src:coprocessor:deploy` (`./gradlew :TEAMNUMBER:src:coprocessor:deploy -Praspberry` for the Raspberry Pi) to deploy your code!
- `./gradlew :4788:src:coprocessor:` is our config, please read through your gradle files to use your own variation as mentioned in the project setup.
- If you have the code deployed to the coprocessor, you can either view it in network tables (if setup), Or you can put `CJVision:1181` to see the camera output. and increment by 1 depending on how many cameras you have.
- Something to note is the line endings under `src/resources/run.sh` in CJ-Vision must be `LF` not `CRLF`. It shouldn't be anyway if it's been pulled from github. But just check if the code is crashing on the tinkerboard/Pi. Happy Tracking :)

<sub><sup>readme written by [@CJBuchel](https://github.com/CJBuchel), 11/01/20</sup></sub>