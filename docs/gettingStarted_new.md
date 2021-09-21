# Getting Started with the platform

## Project setup:
- CJ-Vision uses a directory structure where the program you build is seperated from the platform compiler and library. Compared to other, more traditional methods where your project is inside the platform directory.

- The project uses gradle as it's main front end for compiling, debugging, running and deploying the code.

- If you have cloned the project and submodules using `git submodule update --init` then you should be able to go into the `CJ-Vision` directory and give the command `./gradlew generateFramework`.

- This should build a framework for the user with a quick example for outputting a webcam. If the build fails with `ANNA_LOCK` then the framework already exists, but can be overwritten with `./gradlew generateFramework -Pforce`

- Note that you can change both the version of the frame work (2020.3.2/2021.3.1 etc...) and the version of the internal library (legacy or latest). By default it should use the latest of both these versions. However, can be changed in the `build.gradle`.

- After you have completed this setup, the following is what your direcoty should look like. 

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


