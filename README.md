# OptiCue

This repository sets up the development environment to be used with the OptiCue project. This provides a simple method of setting up the cross compiler for Raspberry Pi and provides a sample program to test the installation.

## Installation (Ubuntu)

1. Update system:
   ```sh
   sudo apt-get update
   sudo apt-get upgrade
   ```
   
2. Install prerequisites:
   ```sh
   sudo apt-get install git cmake make
   ```
   
3. Clone the repository:
   ```sh
   sudo git clone https://github.com/slippmann/OptiCue.git
   ```
   
4. Run script to clone Raspberry Pi libraries:
   ```sh
   cd OptiCue/
   sudo ./git_libs.sh
   ```
   * This script will also compile the wiringPi library
   
5. Test installation:
   ```sh
   cd projects/Sample/
   sudo ./build.sh
   ```
   * If the build is successful, the libraries were properly installed and compiled
