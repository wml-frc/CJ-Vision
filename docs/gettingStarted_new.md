# Getting Started with the platform

## Project setup:
- CJ-Vision uses a directory structure where the user program built is seperated from the platform compiler and library. Compared to other, more traditional methods where your project is inside the platform directory.

- The project uses gradle as it's main front end for compiling, debugging, running and deploying the code.

- If you have cloned the project and submodules using `git submodule update --init` then you should be able to go into the `CJ-Vision` directory and use the command `./gradlew generateFramework`.

- This should build a framework next to the `CJ-Vision` directory with a small single layer example for outputting a webcam. If the build fails with `ANNA_LOCK` then the framework already exists, but can be overwritten with `./gradlew generateFramework -Pforce`

- Note that you can change both the version of the frame work (2020.3.2/2021.3.1 etc...) and the version of the internal library (legacy or latest). By default it should use the latest of both these versions. However, can be changed in the `build.gradle`.

- After you have completed this setup, the following is what your directory should look like. 

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


1. Layout of Coproc

- The Coproc directory is where you write code for your Coprocessor. This directory is linked to the `CJ-Vision` directory and can access the same headers.

- Your main structure is seen inside of `src/main`. In this location you will put your compiled code (`.cpp`) inside `cpp/` and your header files (`.h`) inside `include/`

- The deploy folder is an option given to users who want other files/folders to be deployed over to the coprocessor. You can place any file type and folder structure inside here, and it will be deployed to the coprocessor with the same relative pathing. E.g `cpp/Vision.cpp`'s location for deploy is `../../deploy` and is the same on the coprocessor.

2. Compiling/Debugging Commands

- Compiling is done via gradle through CJ-Vision.

- You can run `./gradlew build` for common building of the project, `./gradlew clean build` will delete everything and build the project.

- `./gradlew runVision` can be used for running the program locally. And if setup to do so, will use your webcam for testing.

- Other commands to keep in mind are `./gradlew downloadAll` which will download all gradlerio dependencies if you are on Windows. On Unix machines this is pointless, as the project utilizes makefiles and opencv packages manually.

3. Application & Layering System

- The project is devised around two main concepts, a single application with access to internal programs like loggers, networking and common properties. Along with processing layers. Filtering layers, bounding layers, networking layers etc.... Which run, one by one in a loop with onAttach, onUpdate and onDetach.

- There can only be one application class for a single project. Multiple will result in a build fail.

- The structuring for this application usually requires only the constructor (which will push layers onto the layer stack) and a deconstructor. As seen below

```cpp
class ExampleVisionApplication : public CJ::Application {
 public:
	ExampleVisionApplication() : CJ::Application("Example App") {
		CJ_PRINT_INFO("Example App created");
		pushLayer(new ExampleLayer(get()));
	}

	~ExampleVisionApplication() {
		CJ_PRINT_WARN("Test App Destroyed");
	}
};

CJ_CREATE_APPLICATION(ExampleVisionApplication)
```

- Note the `CJ_CREATE_APPLICATION(ExampleVisionApplication)` which creates the application and links it to the lib and the entry point <- only one of these should exist

- This class constructs and builds an application with the name `"Example App"` (will show up in logger for anything that originates from the App). It then pushes a processing layer `ExampleLayer` onto the Layer stack.

- A Layer can either be a default `Layer` or an `Overlay`. Layers are pushed one by one and attach/update/detach in the sequence it was added to the stack. If an Overlay is pushed, it will run after other layers have updated. E.g if Overlay1 -> Overlay2 -> Layer1 are added in that sequence to the stack. Then Layer1 will run first. Then Overlay1 and Overlay2 will then run after in that order.

- The strucure for a layer is seen as follows
```cpp
class ExampleLayer : public CJ::Layer {
 public:
	ExampleLayer(CJ::Application &app) : Layer("Example Layer"), _app(app) {
		CJ_PRINT_INFO("Example Layer Created");
	}

	void onAttach() override {
		CJ_PRINT_INFO("Example Layer Attached");
	}

	void onDetach() override {
		CJ_PRINT_WARN("Example Layer Detached");
	}

	void onUpdate() override {
		CJ_PRINT_INFO("Example Layer update");
		_app.setRunning(false); // Sets app to no longer running. Stops the program after one cycle.
	}

 private:
	CJ::Application &_app;
};
```

- Where onAttach runs when the Example layer is placed on the stack. onUpdate runs every loop. And onDetach runs when the layer is taken off the stack (will also run at the end of the program)

- A few methods can be accessed from the application. These can be seen throughout the examples for setting the app to no longer run or to push and pop layers. A list of these main Application methods can be seen below.

```cpp
/**
 * Statically get application
 */
static Application &get();

/**
 * setter for application & layers
 */
void setRunning(bool status);
void setLayersRunning(bool status);

/**
 * getter for application and layers
 */
bool getRunning();
bool getLayersRunning();

/**
 * Get App name
 */
std::string getName();
```

- You can also push/pop layers along with other methods, using the following from the application class
```cpp
/**
 * Push layer onto stack (places it after the last layer pushed)
 */
void pushLayer(Layer *layer);

/**
 * Push overlay onto stack (places overlay at the end of the stack after all layers)
 * Overlays are always after layers. And are placed acordingly
 */
void pushOverlay(Layer *overlay);

/**
 * Delete a layer (shuffles stack afterwards)
 */
void popLayer(Layer *layer);

/**
 * Delete overlay (shuffles stack afterwards)
 */
void popOverlay(Layer *overlay);
```


4. Logging

- CJ-Vision uses [spdlog](https://github.com/gabime/spdlog) for it's colour coded logging system. And can be accessed via these macros
```cpp
CJ_PRINT(...)
CJ_PRINT_INFO(...)
CJ_PRINT_ERROR(...)
CJ_PRINT_WARN(...)
CJ_PRINT_TRACE(...)
```

- You can place most strings and variables through these. And is recommened over `std::cout/print/f`.
- You can also disable the logger completely as a release mode or for user preference via `#define DISABLE_LOGGER` before `CJ_Vision.h` is included.

5. Platform dependent code

- The project comes with definitions for different platforms including the coprocessor. Which is defined during compile time.

- These can be utilized by the user for coprocessor specific needs. Like disabling the logger for faster runtime.

- A list of these Platform defs is seen below
```cpp
CJ_PLATFORM_WINDOWS // Windows
CJ_PLATFORM_LINUX // Linux
CJ_PLATFORM_IOS // IOS Device
CJ_PLATFORM_MACOS // Macbook/Mac OS device
COPROC // Coprocessor Device
```

e.g
```cpp
#ifdef COPROC
#define DISABLE_LOGGER
#endif
```

6. Error Handling

- Assert macros are added to the project for advanced error handling based on user input (true or false). It can also define line number of issue and log the issue in the cli
```cpp
CJ_ASSERT(...)
```

- e.g
```cpp
// Currently accepts at least the condition and one additional parameter (the message) being optional
CJ_CORE_ASSERT(i > 5); // If false, will log the error and line number to the cosole
```

- You can also disable the asserts via defining `#define CJ_DISABLE_ASSERTS` before including `CJ_Vision.h`

7. Images & Camera's

- OpenCV is utilized in this project as the main method for vision processing. A few methods and structures have been wrapped around to make the process of tracking objects easier. But raw OpenCV code is also available to the user.

- `CJ::Image` is a small image structure wrapping around the existing `cv::Mat` structure. While also providing a string for the name. And also internal vectors commonly used for line detection, contours and hulls around objects. (This can be seperated, but is placed in one location for ease of use.)

- Multiple images can be created, generally used for seperating certain steps in tracking. E.g, an original unchanged image, colour filtered image, contours image etc....

- The `Camera` class built in uses OpenCV's `cv::ViceoCapture` class. And wraps around the concept for reduced lines in configuring the Camera. Multiple Camera classes can be created as well.

- The Camera class contains a `config`, the raw capture from opencv `cap`, an initialization method `init()` and a capture method meant to be used in the update loop `capture(CJ::Image &image)`. Note that the capture methods requires an image, as to pass the data from the camera to the image placed as a parameter.

- The config structure has default values but can be overridden. The structure is seen below

```cpp
struct Config {
	int port = 0;
	int apiID = cv::CAP_ANY;
	int fps = 30;
	int resWidth = 640;
	int resHeight = 480;
	float exposure = 0.1;
	bool autoExposure = true;
	std::string name = "Cam";

	float cap_prop_autoExpose = 0.75; // default value to set camera for auto exposure ON
	float cap_prop_manualExpose = 0.25; // default value to set camera for auto exposure OFF
};
```


- An example (using a camera Layer) for using the Camera on port 0, then passed to an origin image is seen below.
- The image is taken externally from the Layer so it can be used in other layers. Similar to the `&app` reference.

```cpp
class CameraLayer : public CJ::Layer {
 public:
	CameraLayer(CJ::Application &app, CJ::Image &image) : Layer("Camera Layer"), _app(app), _image(image) {
		CJ_PRINT_INFO("Example Layer created");
	}

	void onAttach() override {
		_image.name = "Input Image"; // Set name for image. (used mainly for debugging)
		_cam.config.port = 0; // Port number (normally webcams are 0)
		_cam.config.name = "Input Camera"; // Set name for camera

		if (_cam.init() != 0) { // Initialize the camera. And stop the program if it fails.
			_app.setRunning(false);
		}
		CJ_PRINT_INFO("Camera Created");
	}

	void onDetach() override {
		CJ_PRINT_WARN("Example Layer Detached");
	}

	void onUpdate() override {
		_cam.capture(_image);
	}

 private:
	CJ::Application &_app;
	CJ::Image &_image; // Input image reference. Getting image from outside of camera layer so it can be used externally

	CJ::Camera _cam; // Camera instance
}
```

8. Output

- Outputting images is generally only for the debugging stage as there are usually no screens on a coprocessor. The exception to this would be setting up a system to send the images over the network.

- The output/display structure is a static templated verdict function with two required parameters. But expandable to as many paramaters (images to output) as needed. In OpenCV a waitkey is required for the processing to be completed for the image and to output it properly. This slows down the program, but is only needed when debugging or when you want to output an image.

- The output function does this automatically, but you need to specify the length of time in ms you want the program to wait.

- An example taken from before but with the output can be seen below.

```cpp
class CameraLayer : public CJ::Layer {
 public:
	CameraLayer(CJ::Application &app, CJ::Image &image) : Layer("Camera Layer"), _app(app), _image(image) {
		CJ_PRINT_INFO("Example Layer created");
	}

	void onAttach() override {
		_image.name = "Input Image"; // Set name for image. (used mainly for debugging)
		_cam.config.port = 0; // Port number (normally webcams are 0)
		_cam.config.name = "Input Camera"; // Set name for camera

		if (_cam.init() != 0) { // Initialize the camera. And stop the program if it fails.
			_app.setRunning(false);
		}
		CJ_PRINT_INFO("Camera Created");
	}

	void onDetach() override {
		CJ_PRINT_WARN("Example Layer Detached");
	}

	void onUpdate() override {
		_cam.capture(_image);
		CJ::Output::display(30, _image); // Can input more images. E.g display(30, image1, image2, ...)
	}

 private:
	CJ::Application &_app;
	CJ::Image &_image; // Input image reference. Getting image from outside of camera layer so it can be used externally

	CJ::Camera _cam; // Camera instance
}
```

