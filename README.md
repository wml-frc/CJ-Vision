# VisionTracking
Vision Tracking Sub-module library, To make vision tracking easier for other teams with pre-threaded processes and given known functions. Like X,Y center figures, distance detection, square and circle detection.

You may be wondering why it's called CJ Vision... well it was either that or Potato Vision, and i prefer self promotion. hehehehe

## Setting up Vision in your own project

- First you will need to install the submodule to your root directory. Navigate to your root dir and install via `git submodule add https://github.com/CJBuchel/CJ-Vision.git`. 

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

- The code is still in it's prototype stage and will have improvements done over time. Also note this code is not designed to replace your own vision tracking code. Merely make the process a little easier. It's recommeded that you look through OpenCV's documentation before using this library. `https://opencv.org/`

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

Note that while running the vision simulation on your computer, you will most likely need an external webcam plugged in via usb. Built in webcams sometimes doesn't work. Which is why often the port number will need to be 1 to connect to the second webcam. And for tinkerboards and Pi's you will most likely need to change the port number from 4 onwards. as ports 0-3 are taken by other processes.

Also due to a bug in Camera servers the exposure will not change when running locally. Most of your colour testing and contour detection can still be done locally. But it's a good idea to test on your coprocessor a few times to get the final result.


once complete just do a quick test run using the command `.\gradlew runVision` in your root dir.
If all goes well it will build perfectly fine and a window should open up. If it fails to build, you might need to recheck if the project has been setup correctly.
