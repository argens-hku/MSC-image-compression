# Image Compression

This folder contains the work done for assignment 2 of COMP 7503 in year 2017. An average compression ratio was acheived using a combination of 3 techniques, namely:

1. 5 predictors per scan line

2. Shift coding

3. Channel preprocessing

## Folder Structure

- **Documents** contain the documents given for the assignment, including the actual question describing the challenge, as well as tutorial slides giving a guideline

- **Program Template** contains most of the program, such as file i/o, gui definition, etc. To update compression algorithm, change `AppCompress.cpp` under `<ROOT_DIR>\Program Template\VPT\App`

- **Work** contains the implemented algorithm, compiled program, as well as compression result and other deliverables, including a PDF which explains the methodology.

## Getting Started

- Execute `Work/VPT.exe`
- Toggle toolbar open under `View` -> `Toolbar`
- Toggle console log open by pressing **LOG** icon on the right end of toolbar
- Open an image by pressing `File` -> `Open`
- Press **C** on toolbar for compression, OR **Y**, **U**, **V**, and **YUV** for transformation before compression

## Results

- The results can be seen in `<ROOT_DIR>\Work\Compression results.xlsx` which is a record of the compression results reported by the program in console log
- It can be compared with the given demo program under `<ROOT_DIR>\Program Template\Demo\VPT.exe` using given compression algorithm

## Recompile

- Edit `<ROOT_DIR>\Program Template\VPT\App\AppCompress.cpp`
- Open `Program Template/VPT/VPT.vcproj` using Visual Studio 2012 or later
- Build solution by clicking `Build` -> `Build Solution`
- Find the newly build `VPT.exe` under `<ROOT_DIR>\Program Template\VPT\Debug`
