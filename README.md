# CUDA-OpenCL-YCbCr-GaussianBlur
A collaboration to get into CUDA-Programming. The underlying example is the conversion of rgb to ycbcr and the calculation of the gaussian convolution of an image.

Collaborator: Phillip Friedel

## Usage
 - Clone repository
 - setup environment (see Setup)
 - Execution:
     - Help: ```cuda/cuda/x64/Debug/cuda.exe --help```
     - Example YCbCr-1-Image:
         - ```cuda/cuda/x64/Debug/cuda.exe --task "0" --image_name "cuda/cuda/images/dice_600x800.png"```
     - Example Gauss-1-Image:
         - ```cuda/cuda/x64/Debug/cuda.exe --task "1" --image_name "cuda/cuda/images/dice_600x800.png" --sigma 20 --filter_height 7```
     - Example YCbCr-All-Images-In-Path:
         - ```cuda/cuda/x64/Debug/cuda.exe --task "2" --image_name "cuda/cuda/images/"```
     - Example Gauss-All-Images-In-Path:
         - ```cuda/cuda/x64/Debug/cuda.exe --task "3" --image_name "cuda/cuda/images/" --sigma 5 --filter_height 1```

## Learned
 - C, C++
     - native
     - OpenCV
     - Boost
 - CUDA
## Prerequisites
 - NVIDEA Graphic shipset
## Setup
 - project is written using Visual Studio and is compile for Windows 10 (64Bit Plattform)
 - independent compilation 
     - Microsoft Visual C++ Compiler
     - Libraries to include
         - OpenCV (compiler+linker)
         - Boost (lib && program_arguments) (compiler+linker)
     - CUDA-Environment
 
## TL;DR
--
## Build with
- NVIDEA graphic card
    - 700 series
    - Compute Capability 5.0
    - CUDA 10.1
- Visual C++ 10
- nvidea nvcc 10.1
- OpenCV 4.1.1
- Boost 1.720 (lib && program_arguments)
## Acknowledgements
 - thanks to my collaborator: Phillip Friedel

