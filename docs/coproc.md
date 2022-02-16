## Getting Started with your Coprocessor

1. Grab your processor image. (you can use any processor with a linux debian based distro. Below displays tinkerboards and raspberry pi's as examples)

- [Tinkerbaord Image](https://dlcdnets.asus.com/pub/ASUS/mb/Embedded_IPC/TinkerBoard_S/Tinker_Board-Debian-Stretch-V2.1.11-20200310.zip) V2.1.11
- [Raspberry Pi](https://downloads.raspberrypi.org/raspios_lite_armhf_latest) Latest

2. Flash the image: https://etcher.io (Or other imager)

- Flash the image onto an sd (or local storage depending on your device)

- In the accessable portion of the pi image (name "boot") you will need to add a file with no extention 
named `ssh` this will allow headless ssh into the Coprocessor. May apply for other devices.

3. SSH into the Coprocessor
- On Mac/Linux, use `ssh username@hostname.local` (`.local` added at end of hostname. e.g, `ssh linaro@tinkerboard.local`)
- On windows (depending on the version) you can do the same in cmd, or you can download [puTTY](https://the.earth.li/~sgtatham/putty/latest/w64/putty-64bit-0.74-installer.msi)

- Tinkerboard:
  - Hostname: `tinkerboard`, Username: `linaro`, Password: `linaro`
- Pi:
  - Hostname: `raspberrypi`, Username: `pi`, Password: `raspberry`

- The above and below assumes you are on the same Local Network as the Coprocessor with internet access.

4. Run the following command to install the packages and install OpenCV
- `sudo wget -qO - https://github.com/wml-frc/CJ-Vision/blob/master/bootstrap/bootstrap.sh?raw=1 | bash`
- This can take up to an hour depending on network & Coprocessor speed. When prompted enter your team number (if you're not apart of a team just put 0)

5. Your Coprocessor will now restart

6. Changes Made
- Hostname is now: `CJVision`, Username: `vision`, Password: `CJfrc`

7. Testing if device is setup

- Run `./gradlew deploy` in CJ-Vision dir to test if the device can be deployed to. If all went well it should read `BUILD SUCCESFUL`

- `./gradlew deploy` builds your code locally, deploys the cpp file structure to the coprocessor then builds it on the processor before setting up a service for the device and running the program.

- The first deploy will take longer than subsequent deploys due to the existing compiled code. (Only changed files are deployed and recompiled). On the very first deploy you may need to run `./gradlew deploy deploy` as some devices (linux) tend to skip the `make` command on the coprocessor. But resolves itself after the second deploy.

- `./gradlew deploy -Ppurge` purges all code and structure on the device. Deploy after this will take time, same as first time deploy.

- `./gradlew deploy -Pstop` will deploy the code but stops the vision program and does not start it back up (will need to be started manually or redeploy without `-Pstop`)

- check in [Getting Started](gettingStarted.md) to see how to start using the platform on your machine.

8. Maintenance and checking
- while ssh'd into the coprocessor you can run `systemctl status vision` to check the status (running, stopped, failed). And you can check the console/logs via `sudo journalctl -u vision`

- the `.sh` files in resources need to be deployed in LF format not CRLF. This should be configured anyway inside .gitattributes, but if you find your program failing it might be the cause.

- inodes full, a knwon bug I found while testing the software. Constant deploying and replacing of the files over and over again can fill up your inodes, and your coprocessor will display `storage is full`. When in fact it has used no storage because it replaces the existing files. It's a small issue that I've only seen twice so far and shouldn't even exist if the program is working properly. But if a fail while deploying consists. It could be a causing factor. A fix is to reimage the coprocessor, as I have no software fix for it. And seems to be linked to embedded tools https://github.com/JaciBrunning/EmbeddedTools