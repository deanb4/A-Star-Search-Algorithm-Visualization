# A* Algorithm with SFML

This project implements the A* pathfinding algorithm using **SFML** for visualization. Follow the instructions below to set up, build, and run the project.

## Tiles:
- **Blue** = Start
- **Red** = Goal
- **Black** = Obstacle

### Interactions:
- To generate obstacles, hold down the middle mouse button.
- Left-click to reposition the start position.
- Right-click to reposition the goal position.
- Press **r** to reset the grid.
- Press **Enter** to run the visualization.

## Prerequisites

### 1. Install a C++ Compiler
Ensure you have a working installation of `g++`.

### 2. Install SFML
#### Windows:
1. Download the SFML library from the [official website](https://www.sfml-dev.org/download/sfml/3.0.0/).
2. Choose the version that matches your compiler.
3. Extract the downloaded files to a directory, e.g., `C:/Users/<YourUsername>/SFML/`.

#### Linux:
1. Install SFML via your package manager:
   ```bash
   sudo apt-get install libsfml-dev

### Ensure the paths to the SFML include and lib directories are correctly specified in the Makefile:
For example:
    INCLUDE = -I"C:\Users\<YourUsername>\SFML\src\include"
    LIB = -L"C:\Users\<YourUsername>\SFML\src\lib" 

### Run the Following Command to Compile and Link the Project:
make

### To remove the generated object files and executable, run:
Make clean

### After building, run the executable from the terminal:
a_star
