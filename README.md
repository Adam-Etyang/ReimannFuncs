# ReimanViz

A small C++17 macOS OpenGL starter project. It opens an OpenGL 4.1 core window
and draws a colored triangle using GLFW and the native OpenGL framework.

## Requirements

- CMake 3.20 or newer
- A C++17 compiler
- OpenGL-capable graphics drivers
- macOS and an internet connection on the first configure (CMake downloads GLFW)

Install CMake and the Xcode Command Line Tools if they are not already
available. macOS supports OpenGL up to 4.1, which this starter targets.

## Build and run

```sh
cmake -S . -B build
cmake --build build
./build/ReimanViz
```

Press **Escape** to close the window. The OpenGL version is printed to the
terminal when the application starts.
