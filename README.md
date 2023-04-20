# Smooth Particle Hydrodynamics for Fluid Dynamics Simulation in 2D with Multiple Discretization Implementations

## Key Words

- Smooth Particle Hydrodynamics (SPH)
- Fluid Dynamics
- Two Dimensional
- Discretization

## Requirements

- c++11

## Preinstall

- I have provided all the necessary libraries in lib folder
- You must first compile each for your target distribution (note I have not included windows .DLLs)

## Install

```bash
mkdir build
cd build
cmake ..
make
```

## Run

`./SPH`

## Note

- Is designed in a 2D environment so it can run in real time.
- You can use the keyboard number key (not numpad) to change scenes
- You can do the same thing while holding SHIFT to go through the simulators
- You can do the same thing while holding CTRL to go through the spatial discretizations
- The scenes are dependent on time step and FPS so different simulators have different effects, do tweak the scene settings to obtain the desired result

## Issues Encountered

- Wanted to have freetype but could not get it to work on M1 (so I halted development) using ImGui instead

```

```
