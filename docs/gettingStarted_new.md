# Getting Started with the platform

## Project setup:
- CJ-Vision uses a directory structure where the program you build is seperated from the platform compiler and library. Compared to other, more traditional methods where your project is inside the platform directory.

- The project uses gradle as it's main front end for compiling, debugging, running and deploying the code.

- If you have cloned the project and submodules using `git submodule update --init` then you should be able to go into the `CJ-Vision` directory and use the command `./gradlew generateFramework`.

- This should build a framework next to the `CJ-Vision` directory with a quick example for outputting a webcam. If the build fails with `ANNA_LOCK` then the framework already exists, but can be overwritten with `./gradlew generateFramework -Pforce`

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

- The Coproc directory is where you write code for your Coprocessor, and uses the tools and methods inside of CJ-Vision.

- Your main structure is seen inside of `src/main`. In this location you will put your compiled code (`.cpp`) inside `cpp/` and your header files (`.h`) inside `include/`

- The deploy folder is an option given to users who want other files/folders to be deployed over to the coprocessor. You can place any file type and folder structure inside here, and it will be deployed to the coprocessor with the same relative pathing. E.g `cpp/Vision.cpp` location for deploy is `../../deploy` and is the same on the coprocessor.

2. Compiling/Debugging Commands

- Compiling is done via gradle through CJ-Vision.

- You can run `./gradlew build` for common building of the project, `./gradlew clean build` will delete everything and build the project.

- `./gradlew runVision` can be used for running the program locally. And if setup to do so, will use your webcam for testing.

- Other commands to keep in mind are `./gradlew downloadAll` which will download all gradlerio dependencies if you are on Windows. On Unix machines this is pointless, as the project utilizes makefiles and opencv packages manually.

3. Application & Layering System

- The project is devised around two main concepts, a single application with access to internal programs like loggers, networking and common properties. Along with processing layers. Filtering layers, boinding layers, networking layers etc.... Which run one by one in a loop with onAttach, onUpdate and onDetach.

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

- A Layer can either be a default `Layer` or an `Overlay`. Layers are pushed one by one and attach/update/detach in the sequence it was added to the stack. If an Overlay is pushed, it will run after other layers have updated. E.g if Overlay1 -> Overlay2 -> Layer1 are added in that sequence to the stack. Then Layer1 will run first, after Overlay1 and Overlay2 will then run in that order.

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

- You can push/pop layers using the following functions from the application class
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

- You can also access functions of the layer `begin(), end(), rbegin(), rend()`. where `r` means reverse.

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

