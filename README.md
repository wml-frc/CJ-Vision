## Pre-threaded asynchronous computer vision library used for easy deployment to UNIX based coprocessors.

### CJ-Vision-2.0 in development

#### Build Status
[![Build Status](https://dev.azure.com/ConnorBuchel0890/WML/_apis/build/status/wml-frc.CJ-Vision?branchName=refs%2Fpull%2F46%2Fmerge)](https://dev.azure.com/ConnorBuchel0890/WML/_build/latest?definitionId=12&branchName=refs%2Fpull%2F46%2Fmerge)
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
		|		Vision.cpp
		|
		|___include
				Vision.h
CJ-Vision
|
|___...
```


### Features:
- Pre-threaded. (wrappers around commonly used OpenCV functions are threaded upon launch)

- Asynchronous. (aynchronous structure for threaded functions)

- UNIX based Deployment. (uses GradleRIO & GradleRIO syntax for building/deploying to UNIX based systems)

- Socket Networking. (sends CV data through custom socket programming and can be received using provided header insert)

## [Local Setup](docs/local.md)
setup on local machine

## [Coprocessor Setup](docs/coproc.md)
setup on coprocessor

## [Getting Started](docs/gettingStarted.md)
getting started with the library

## [Examples](docs/examples/?)
example uses of the library

<sub><sup>readme writted by [@CJBuchel](https://github.com/CJBuchel), 28/09/20</sup></sub>
