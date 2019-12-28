# VisionTracking
Vision Tracking Sub-module library, To make vision tracking easier for other teams with pre-threaded processes and given known functions. Like X,Y center figures, distance detection, square and circle detection.

You may be wondering why it's called CJ Vision... well it was either that or Potato Vision, and i prefer self promotion. hehehehe

## Setting up Vision in your own project

- First you will need to install the submodule to your root directory. Navigate to your root dir and install via `git submodule add https://github.com/CJBuchel/CJ-Vision.git`. 

- If you havent already, you will need to make a vision dir, e.g `4788/src/coprocessor/`. You can build it how you like, Similar to your teamCode folder. With a cpp and include file. (For an example you can view https://github.com/CJBuchel/2020CodeBase/tree/CJ-Patch1/4788/src/coprocessor for more detail)

- Then you will need to apply the libraries `vision.gradle` with your own `src/coprocessor/build.gradle` and just add `apply from: rootProject.file('CJ-Vision/vision.gradle')` assuming you have placed the submodule in it's default location in the root dir. If you havent you will need to change the previous lines, and also edit the `CJ-Vision/vision.gradle` file inside the library. 

Change the following lines if you have placed the vision Library somewhere else.
```
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
