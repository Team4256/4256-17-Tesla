# 4256-17-Tesla

This is the visual odometry code for the Cyborg Cats, working with the ZED camera SDK.


## Getting started

- Download the latest version of the ZED SDK on [stereolabs.com](https://www.stereolabs.com).
- Read the ZED [API documentation](https://www.stereolabs.com/developers/documentation/API/).

### Prerequisites

- Windows 7 64bits or later, Ubuntu 16.04
- [ZED SDK](https://www.stereolabs.com/developers/) and its dependencies ([CUDA](https://developer.nvidia.com/cuda-downloads)

Note that the ZED SDK (at least on Windows) comes with OpenCV 3.1, so installing that isn't required

## Build the program

Download the sample and follow these instructions:

#### Building using command-line cmake on Windows or Linux

On Linux, do:

    mkdir build
    cd build
    cmake ..
    cmake --build .

On Windows, do:

    mkdir build
    cd build
    cmake -Ax64 ..
    cmake --build .

or load the project file into Visual Studio.
