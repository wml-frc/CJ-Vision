## Notes
### General
- Using version system for the time being. Less confliction with people who use previous versions of the software.

- Coproc Framework exists inside of `framework/$VERSION`

- CJ-Vision files are stored inside `CJ-Vision/version/$VERSION`

- Libs (UDP_TransferNT) are stored inside `CJ-Vision/common/libs`. Pathing links need to work in both gradle and the makefile. 

### 08/09/2021
 - UDP_TransferNT really does need a changeup. Ever since WARP, i've gotten new ideas to improve efficiency of the network. Plus, windows support needs to be added. Almost purely because most users will have a windows machine and try to compile using the windows build. Even if only the linux version is run on the co-processor

## Implementations/TODO

### General
- Build logger system that logs out to files? Maybe access via an echo through a gradle task. Spdlog makes a decent lib for loggers.

- Singleton class for the core, only one instance can exist at a time, and it's used to start stop and pause the entire program.

- Layering system simillar to my game engine `OpticSiege`, push/pop layers and overalys. As it makes sense in a visual processing workflow. Each layer runs one after the other. GetImage->ProcessImage->DisplayImage, and overlays are always pushed to back of queue, one after the other. E.g, display (because not priority)

- Might look into the opencv error handles and build a wrapper around them. To make assertions for testing and checking opencv code inside the 


### 08/09/2021
- The multi threaded nature used before made the program slightly faster. But it launched too many threads, with very little benifit. Either remove them and switch to single thread. Or, run with a different method that's more benefitial to the actual speed of CV. E.g, `std::async`

- Another method i've found is both `Opencv CUDA` and more along with what FRC teams would want `OpenCV with TBB` https://www.theimpossiblecode.com/blog/faster-opencv-smiles-tbb/ Which looks quite interesting to speed up OpenCV processing.

## ISSUES
### General
- Currently the makefile does not build using the network lib on the device
- Gradle does not deploy the lib either