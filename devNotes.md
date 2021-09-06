## Notes

- Using version system for the time being. Less confliction with people who use previous versions of the software.

- Coproc Framework exists inside of `framework/$VERSION`

- CJ-Vision files are stored inside `CJ-Vision/version/$VERSION`

- Libs (UDP_TransferNT) are stored inside `CJ-Vision/common/libs`. Pathing links need to work in both gradle and the makefile. 

### ISSUES
- Currently the makefile does not build using the network lib on the device
- Gradle does not deploy the lib either