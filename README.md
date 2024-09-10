# Path Tracer

This is a simple path tracer that renders a single scene using path tracing algorithms to simulate realistic lighting.

## Features
- Basic path tracing with realistic lighting effects
- Renders one predefined scene

## Screenshot
![Path Tracer Output](assets/Screenshot1.png)  
_A screenshot of the rendered scene._

## Installation Guide

### Prerequisites
- A C++20 or higher compatible compiler
- [CMake](https://cmake.org/) version 3.2 or higher
- [Conan](https://conan.io/) package manager for dependencies

### Build Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/adidvar/PathTracing.git
   cd PathTracing
   cmake -S . -B build
   cd build
   conan install .
   cmake .
   cmake --build .
   ```

## To do

 - Implement support for multiple scene files
 - Integrate an Octree for optimized scene management
 - Add multithreading for faster rendering