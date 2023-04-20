# Smooth Particle Hydrodynamics for Fluid Dynamics Simulation in 2D with Multiple Discretization Implementations

## Description

This repository provides a unified experience for fluid simulations and quickly comparing different simulators and discretizations options and facilitates the ability to add additional simulators and spatial discretizations.

The chosen simulators are based on the work of Müller et al 2003, Ihmsen et al 2014 and Koschier et al 2019 and you can quickly go through the various simulators and see their performance. The spatial discretizations options are taken from Koschier et al 2019.

ImGui is used as an interface which allows you to quickly change and update the parameters of the selected simulator and the selected scene. Furthermore there are instructions highlighted in gray which gives the keybindings for the various keyboard shortcuts (make sure to not have the window selected).

## Cross-Platform Compatibility

- CMake was used to ensure compatibility
- Tested on Mac and multiple different Linux distributions
- Windows YOYO

## Requirements

- c++11

## Preinstall

- I have provided all the necessary libraries in lib folder
- You must first compile each for your target distribution (note I have not included Windows .DLLs)

###GLEW

```bash
cd lib/glew-2.1.0
make clean
make
```

###GLFW

```bash
cd lib/glfw-3.3.8
cmake -S . -B build
cd build
make
```

###GLM
No compilation needed

###ImGui
No compilation needed

## Install

```bash
mkdir build
cd build
cmake ..
make
```

## Run

```bash
./SPH
```

## Note

- Is designed in a 2D environment to be able to run in real time
- The scenes are dependent on time step and FPS so different simulators have different effects, do tweak the scene settings to obtain the desired effect

## Known Issues

- Window resizing does not work on Linux (thanks GLFW)

## Issues Encountered

- Wanted to have freetype but could not get it to work on M1 (so I halted development) using ImGui instead

## License

- The included libraries have their licensing attached in their respective directories
- The codebase is not allowed to be redistributed nor extended for anything besides personal use
