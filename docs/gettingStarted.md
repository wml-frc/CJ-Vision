## Getting Started

### Project Setup: 

#### Structure
- CJ-Vision uses a directory structure where the program you build is seperated from the main library.

- The build will fail unless the program either detects the `Coproc` directory. Or it was built using `./gradlew build -PdebugMode` which uses it's own logic for debugging and testing

- The directory structure should look like the following

#### As is (downloaded/cloned)

```
Coproc
|
|___src
	|
	|___deploy
	|
	|___main
		|
		|___cpp
		|	Vision.cpp
		|
		|___include
			Vision.h
CJ-Vision
|
|___...
```

#### As submodule (FRC or large project)
```
MainProject
|
|___Coproc
|	|
|	|___src
|		|
|		|___deploy
|		|
|		|___main
|			|
|			|___cpp
|			|	Vision.cpp
|			|
|			|___include
|				Vision.h
|___CJ-Vision
	|
	|___...
```

This is the required directory structure when building your vision program. (deploy is optional) The names need to be correct as well for the vision lib to detect and compile it. Place your c/cpp files inside the `cpp` dir, and place your headers inside `include`. The program searches for and builds Vision.cpp/.h, this will be your entry for the program.

`Vision.cpp`
```cpp
#include "Vision.h"

void Vision::Init() {
	// Logic here will run once (Initialization)
}

void Vision::Periodic() {
	// Logic here will run periodicly (Looping)
}
```

`Vision.h`
```cpp
#ifndef VISION_H
#define VISION_H
#include "Core.h"

class Vision {
 public:
	// Initialization
	void Init();

	// Periodic Looping
	void Periodic();
};

#endif // VISION_H
```

the `deploy` directory is optional when creating the structure. And is used as an extra folder which can be deployed to the coprocessor. You can place files, configs and other necessities if you need them on the coprocessor. (e.g, pre-trained data for object detection, or custom camera configurations you need to import into your program)

### Program Functions:

#### Initialization
- CJ-Vision comes with pre-threaded functions to reduce the amount of programming for the user. Especially threading.

1. To get starting you will need to initialize the core of the library

2. Create a camera object and configure it to your needs. You can override all the defaults (`ResWidth, ResHeight, CamPort, CamName, AutoExposure, FPS`) or you can leave them as is.

3. Afterwards, call the setupVision() thread which will put your camera object on a seperate thread in the background. (it's possible to have multiple camera objects and multiple setupVision() thread calls. But has never been tested. It also may decrease the performance of your device)

```cpp
#include "Vision.h"

CJ::Camera::Cam cam;
CJ::Image inputImage;

void Vision::Init() {
	CJ::Core::init();

	/**
	 * Config is a struct. So you can use both cam.config = {640, 480, ...} or cam.config.ResWidth = 640; cam.config.ResHeight = 480; ... 
	 */

	// Defaults
	cam.config = {
		640, // Resolution Width
		480, // Resolution Height
		0, // Camera Port
		"Vision Camera", // Camera Name
		false, // Auto Exposure (overides manual below)
		50, // Manual exposure (different between cameras)
		60 // Target FPS
	};

	inputImage.name = "Input from Camera";

	CJ::Core::setupVision(&inputImage, &cam);
}

void Vision::Periodic() {
	CJ::Output::Display(&inputImage);
}
```

The above program should input an image and display it the `Core` which was initialized detects errors and starts/stops the program automatically. But you can manually check/start/stop the program using

```cpp
PROG::PROG_RUNNING(); // returns boolean (is running)
PROG::THREADS_RUNNING() // returns boolean (is threads running)
PROG::set_PROG_RUNNING(boolean); // set the program running (true or false)
PROG::set_THREADS_RUNNING(boolean); // set the threads running
```
