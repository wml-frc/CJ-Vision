# Getting Started with the platform

## Project setup:
- CJ-Vision uses a directory structure where the user program built is serparated from the platform compiler and library. Compared to other, more traditional methods where your project is inside the platform directory.

- The project uses gradle as it's main front end for compiling, debugging, running and deploying the code.

- If you have cloned the project and submodules using `git submodule update --init` then you should be able to go into the `CJ-Vision` directory and use the command `./gradlew generateFramework`.

- This should build a framework next to the `CJ-Vision` directory with a small single layer example for outputting a webcam. If the build fails with `ANNA_LOCK` then the framework already exists, but can be overwritten with `./gradlew generateFramework -Pforce`

- Note that you can change both the version of the frame work (2020.3.2/2021.3.1 etc...) and the version of the internal library (legacy or latest). By default it should use the latest of both these versions. However, can be changed in the `sharedGradle/mainBuilder.gradle`.

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


## Multi Project/Submodule addition
- Note that CJ-Vision supports being inside of a multi gradle project (if you have the necessary gradle plugins)

- To do so you will need to add the following to your root project `build.gradle` unless already existing
```gradle
plugins {
	id "cpp"
	id "edu.wpi.first.GradleRIO" version "2021.3.1"
	id "google-test-test-suite"
	id "groovy"
}
```

- And in `settings.gradle` add the following to link up CJ-Vision to your main project
```gradle
// CJ-Vision
include 'CJ-Vision'
project(':CJ-Vision').projectDir = file('CJ-Vision/multiProject') // or where CJ-Vision is located
```

- Doing so will allow you to use all of CJ-Vision's tasks without needing to be inside of CJ-Vision's directory. (Useful for FRC teams who may want to deploy everything over to the robot using `./gradlew deploy`) instead of separately deploying robot code and vision code.

- Optionally if your main project/ one of your sub projects also needs the UDP_TransferNT (listed below in detail) for communication between `device1` and CJ-Vision. Then you can also append your `build.gradle` to add the library. Assuming that CJ-Vision is added as a submodule.

```gradle
model {
	components {
		udpTransferNT(NativeLibrarySpec) {
			targetPlatform wpi.platforms.roborio
			sources {
				cpp {
					exportedHeaders {
						srcDir rootProject.file('CJ-Vision/Core/common/libs/UDP_TransferNT/UDP_TransferNT/include')
					}
				}
			}
		}

		frcUserProgram(...) {
			//---
			binaries.all {
				lib library: 'udpTransferNT', linkage: 'shared' // can also be static
			}
			//---
		}
	}
}
```

## General Structure
1. Layout of Coproc

- The Coproc directory is where you write code for your Coprocessor. This directory is linked to the `CJ-Vision` directory and can access the same headers.

- Your main structure is seen inside of `src/main`. In this location you will put your compiled code (`.cpp`) inside `cpp/` and your header files (`.h`) inside `include/`

- The deploy folder is an option given to users who want other files/folders to be deployed over to the coprocessor. You can place any file type and folder structure inside here, and it will be deployed to the coprocessor with the same relative pathing. E.g `cpp/Vision.cpp`'s location for deploy is `../../deploy` and is the same on the coprocessor.

2. Compiling/Debugging Commands

- Compiling is done via gradle through CJ-Vision.

- You can run `./gradlew build` for common building of the project, `./gradlew clean build` will delete everything and build the project.

- `./gradlew runVision` can be used for running the program locally. And if setup to do so, will use your webcam for testing.

- Other commands to keep in mind are `./gradlew downloadAll` which will download all gradlerio dependencies if you are on Windows. On Unix machines this is pointless, as the project utilises makefiles and opencv packages manually.

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

- You can place most strings and variables through these. And is recommended over `std::cout/print/f`.
- You can also disable the logger completely as a release mode or for user preference via `#define DISABLE_LOGGER` before `CJ_Vision.h` is included.

5. Platform dependent code

- The project comes with definitions for different platforms including the coprocessor. Which is defined during compile time.

- These can be utilised by the user for coprocessor specific needs. Like disabling the logger for faster runtime.

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
CJ_CORE_ASSERT(i > 5); // If false, will log the error and line number to the console
```

- You can also disable the asserts via defining `#define CJ_DISABLE_ASSERTS` before including `CJ_Vision.h`

7. Images & Camera's

- OpenCV is utilised in this project as the main method for vision processing. A few methods and structures have been wrapped around to make the process of tracking objects easier. But raw OpenCV code is also available to the user.

- `CJ::Image` is a small image structure wrapping around the existing `cv::Mat` structure. While also providing a string for the name. And also internal vectors commonly used for line detection, contours and hulls around objects. (This can be separated, but is placed in one location for ease of use.)

- Multiple images can be created, generally used for separating certain steps in tracking. E.g, an original unchanged image, colour filtered image, contours image etc....

- The `Camera` class built in uses OpenCV's `cv::VideoCapture` class. And wraps around the concept for reduced lines in configuring the Camera. Multiple Camera classes can be created as well.

- The Camera class contains a `config`, the raw capture from opencv `cap`, an initialisation method `init()` and a capture method meant to be used in the update loop `capture(CJ::Image &image)`. Note that the capture methods requires an image, as to pass the data from the camera to the image placed as a parameter.

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

- Outputting images is generally only for the debugging stage as there are usually no screens on a coprocessor. The exception to this is using the output stream. Which will set up a MJPEG stream on a serperate thread from the main program.

- The output/display structure is a static templated verdict function with two required parameters. But expandable to as many parameters (images to output) as needed. In OpenCV a waitkey is required for the processing to be completed for the image and to output it properly. This slows down the program, but is only needed when debugging or when you want to output an image.

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

- On the coprocessor the display method is disabled and using it will do nothing. It won't have a waitkey either. It's done in this way to avoid crashing the coprocessor that cannot display a video.

- If you wish to display a video stream then you can utilise the `Stream` class inside of `Output`. This will setup a video stream on a seprate thread and write an MJPEG video to a certain port number, e.g `8080`. Note that legal ports in frc are `5800:5810`.

- The streamer class has 3 non-static methods along with the Constructor/Decondtructor.
```cpp
/**
 * Create Stream on port number
 */
Stream(int port);

/**
 * Stops threads, releases socket and deconstructs video stream
 */
~Stream();

/**
 * Start stream thread
 */
void start();

/**
 * Stop stream thread
 */
void stop();

/**
 * Output image to video stream
 */
void output(Image image);
```

- The mjpeg stream however, only runs on linux platforms. And hence when running the stream on a your local machine (unless linux) will not output any data.

- However, this should not be an issue. As your local machine can utilise display. The key difference is when deployed to the coprocessor it should be a linux platform and will run this output stream. Which users can view via a browser. I.e `cjVision.local:8080`.

- The below example uses both display and stream. And will use the mjpeg stream on the coprocessor, but use the display when running locally.

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

		_streamer.start();
		CJ_PRINT_INFO("Created Streaming Thread");
	}

	void onDetach() override {
		CJ_PRINT_WARN("Example Layer Detached");
	}

	void onUpdate() override {
		_cam.capture(_image);
		_streamer.output(_image); // Only enables itself on linux platforms. Does nothing otherwise
		CJ::Output::display(30, _image); // Only enables itself when running locally. On coprocessor, does nothing
	}

 private:
	CJ::Application &_app;
	CJ::Image &_image; // Input image reference. Getting image from outside of camera layer so it can be used externally

	CJ::Camera _cam; // Camera instance
	CJ::Output::Stream _streamer{8080}; // Create video output stream on port 8080
}
```


9. Filtering & Thresholding Image

- Filtering an image through colour and thresholding can be done with the `ColourFilter` class. Giving options for filtering out certain colours in the HSV spectrum. Or Grayscaling the image.

- The Filter class also has a built in trackbar which can be used to change the filtering of the spectrum live.

- There are four main static methods inside the `ColourFilter` class. Seen below
```cpp
/**
 * Filter using hsv
 */
static void filter(Image &input, Image &output, HSV_Options options);

/**
 * (Adds live trackbar for development)
 */
static void createFilterTrackbar(HSV_Options &options);

/**
 * Filter using gray scale
 */
static void filter(Image &input, Image &output, GrayScale_Options options);

/**
 * (Adds live trackbar for development)
 */
static void createFilterTrackbar(GrayScale_Options &options);
```

- The only difference between the first two methods and the second two methods. Is the ability to either Grayscale the image. Or HSV filter the image.

- The main filter methods have an input and an output image. It's possible to have both the input image and the output image as the same image. And it will overwrite the original data. However this can sometimes lead to bleedthrough, but is a more efficient method for storing the images if you are memory limited.

- The filter method also utlizes a structure for options. Where you can change the HSV spectrum, along with erosion, dilation, blur and binary thresholds. The default value & layout of the struct can be seen below.

```cpp
/**
 * HSV Options
 */
struct HSV_Options {
	int HL = 0,		// hue low
			HH = 255,	// hue high
			SL = 0,		// sat low
			SH = 255,	// sat high
			VL = 0,		// value low
			VH = 255;	// value high

	int erosionSize = 0;
	int dilationSize = 0;
	int blur = 0;
	int binaryThreshold_Low = 0;
	int binaryThreshold_High = 0;
};
```

- And the grayscale version is the same but without the HSV spectrum.
```cpp
/**
 * Gray Scale Options
 */
struct GrayScale_Options {
	int erosionSize = 0;
	int dilationSize = 0;
	int blur = 0;
	int binaryThreshold_Low = 0;
	int binaryThreshold_High = 0;
};
```

- You can also modify the values inside of options live with the `createFilterTrackbar(Options options)` method. 

- An example of filtering for a green colour with a trackbar for tuning can be seen below.
	- Note that this is a new layer, and the camera image is done in a serparate layer. Along with the `display` of the output image also done in a serparate layer after the filter layer has executed `onUpdate()`.

```cpp
class FilterLayer : public CJ::Layer {
 public:
	FilterLayer(CJ::Application &app, CJ::Image &inputImg, CJ::Image &outputImg) : Layer("Filter Layer"), _app(app), _inputImg(inputimg), _outputImg(outputImg) {
		CJ_PRINT_INFO("Filter Layer Created");
	}

	void onAttach() override {
		CJ_PRINT_INFO("Filter Layer Attached");
		_outputImg.name = "Filtered Image";

		options.HL = 50;   // hue low
		options.HH = 70;   // hue high

		options.SL = 0;    // sat low
		options.SH = 255;  // sat high

		options.VL = 0;    // value low
		options.VH = 255;  // value high

		CJ::ColourFilter::createFilterTrackbar(options); // allows live edit of options
	}

	void onDetach() override {
		CJ_PRINT_WARN("Filter Layer Detached");
	}

	void onUpdate() override {
		CJ::ColourFilter::filter(_inputImg, _outputImg, options); // filter image every update
	}

 private:
	CJ::Application &_app;
	CJ::Image &_inputImg;
	CJ::Image &_outputImg;

	CJ::ColourFilter::HSV_Options options;
};
```

- The above can be replicated for a grayscale image with grayscale options as well.

10. Contours/Line Detection

- Contours/Line detection is generally the next step after filtering an image. And is used to get proper point maps of a specified object. Which can later be used to detect shapes and center points for other wanted calculations.

- The `Contours` class provides three static methods for detecting contours. Using the same syntax and style as filter, with an input, an output. And some type of option. In this case, either threshold, or a boolean value to disable draw (will come back to this option later).

- The methods are as follows
```cpp
/**
 * Detect Edges using canny algorithm
 */
static void cannyTrack(Image &input, Image &output, int threshold);

/**
 * Detect contours (Stores inside Image)
 * Disable draw is false by deault. Speed up prograb by setting true
 */
static void detectContours(Image &input, Image &output, bool disableDraw = false);

/**
 * Creates live trackbar for contours
 */
static void createContoursTrackbar(int &threshold);
```

- The `cannyTrack` algorithm is optional, and is better for certain use cases if users need it. However, can be ignored. Instead just the `detectContors` method is really needed. The final method allows us to create a trackbar to modify the threshold value. Only useful for cannyTrack.

- In our example we will just focus on `detectContours` and continue on from the previous examples of getting an input image. Filtering it, and now detecting the contours.

- The contours values are stored inside the `image` container as a vector which was previously mentioned. So there is no method output needed. This vector on contours can later be used by the user, or by the bounding methods which will be covered later.

- We can also disable the drawing of the detectContours method. Which will not draw the contours onto the output image to be seen visually. Instead the output image will only contain the vector of contours inside of it.

- Disabling the draw dramatically increases the speed as drawing the contours onto the image requires looping through the vector of contours to draw onto the image. For debugging visually seeing the contours can be great. However, for speed in a competition. It is not needed. Only the very final output can be displayed on release. And this can be sent to your main processor (roboRio or other).

- Below is an example of using the contours method. It is almost identical to the `FilterLayer` example.

```cpp
class ContoursLayer : public CJ::Layer {
 public:
	ContoursLayer(CJ::Application &app, CJ::Image &inputImg, CJ::Image &outputImg) : Layer("Contours Layer"), _app(app), _inputImg(inputimg), _outputImg(outputImg) {
		CJ_PRINT_INFO("Contours Layer Created");
	}

	void onAttach() override {
		CJ_PRINT_INFO("Contours Layer Attached");
		_outputImg.name = "Contour Image";
	}

	void onDetach() override {
		CJ_PRINT_WARN("Contours Layer Detached");
	}

	void onUpdate() override {
		CJ::Contours::detectContours(_inputImg, _outputImg); // disable fraw is false. (it will draw the contours onto the output image and we can display it later)
	}

 private:
	CJ::Application &_app;
	CJ::Image &_inputImg;
	CJ::Image &_outputImg;
};
```

11. Bounding & Final Imaging

- After detecting the contours, the contours vector is available to use by the user via `_outputImg.contours` which provides a vector of points `std::vector<std::vector<cv::Point>> contours;`. These values are free to use in shape detection, blob detection and other methods or means that the user needs.

- The most common of which is to draw a convex hull (https://learnopencv.com/convex-hull-using-opencv-in-python-and-c/)

- Then draw a bounding box around this hull before finally providing center values (x,y coordinates) of where the object is relative to the screen.

- While this is the most common next steps after drawing contours, there are lots of methods users will use to detect objects. And some may not want to go through these similar steps. However, a `Bound` class is provided to those who do want such steps. Which will utilize the vector inside the image and optionally draw the hull, bounding box and center points onto the image. Along with returning two center `BoundingPoints`.

- There are only two methods inside this class. And similar to the contour detection. We can also disable the drawing of these methods to speed up processing. As drawing the images requires looping through multiple vectors.

- The following is the available bounding points structure, and the drawing methods.
```cpp
struct BoundingPoints {
	double center_x = 0, center_y = 0;
};

/**
 * bind hull around contours (convex hull)
 * Use disable draw to stop drawing
 */
static void drawConvexHull(Image &input, Image &output, bool disableDraw = false);

/**
 * Draw bounding box
 * Define size threshold to draw box around blob/hull
 * Use disable draw to stop drawing (only outputs bounding points)
 */
static BoundingPoints drawBoundingBox(Image &input, Image &output, bool disableDraw = false);
```

- A full example can be seen below.

```cpp
class BoundingLayer : public CJ::Layer {
 public:
	ContoursLayer(CJ::Application &app, CJ::Image &inputImg, CJ::Image &outputImg) : Layer("Bounding Layer"), _app(app), _inputImg(inputimg), _outputImg(outputImg) {
		CJ_PRINT_INFO("Bounding Layer Created");
	}

	void onAttach() override {
		CJ_PRINT_INFO("Bounding Layer Attached");
		_outputImg.name = "Bounding Image";
	}

	void onDetach() override {
		CJ_PRINT_WARN("Bounding Layer Detached");
	}

	void onUpdate() override {
		CJ::Bound::drawConvexHull(_inputImg, _contourImg); // draw hull onto _contourImg
		object_xy = CJ::Bound::drawBoundingBox(_contourImg, _outputImg); // draw bounding box and get center points

		// We can use these points and send them over the network. Or do something internally with them. For now, we will just output them to the console
		CJ_PRINT_INFO("Points (x,y): " + std::to_string(object_xy.center_x) + "," + std::to_string(object_xy.center_y));
	}

 private:
	CJ::Application &_app;
	CJ::Image &_inputImg;
	CJ::Image _contourImg;
	CJ::Image &_outputImg;

	CJ::BoundingPoints object_xy;
};
```

12. Networking

### Networking 
- This library uses a custom UDP networking library to send data between devices (Due to issues implementing Network Tables on coprocessors like jetsons and tinkerboards in a stable manner). The networking is written in pure C++ headers using simple networking wrappers around winsock and unix sockets. It provides a custom configuration (ip, port setting, etc...) for sending data between machines.

- The library submodule docs can be viewed here https://github.com/wml-frc/UDP_TransferNT. You need to have the library on both the coprocessor and the roborio in order to send and receive between the two devices. It is a simple header library, which makes it an easy implementation for FRC code. As there is no compiling or linking required. Just include the headers and or place them in a directory which is seen by the rest of your src files.
