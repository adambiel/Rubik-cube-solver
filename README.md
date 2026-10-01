# Rubik's Cube Solver – PWI Project (Team 4)

This project features a program designed to solve a Rubik's Cube. 

## My Contribution
As a contributor to this team project, my primary focus was on the backend algorithmic engine, state representation, and cross-module debugging. My specific responsibilities included:
* **Core Cube Mechanics:** Implemented the foundational `cube` class, which handles internal state representation, strict move execution logic, and I/O operations for reading and saving cube states.
* **Pattern Databases & Heuristics:** Developed the standalone programs responsible for generating the pattern databases required to efficiently guide the solving algorithm.
* **Performance Optimization:** Integrated Lehmer coding for efficient permutation indexing, significantly optimizing heuristic lookup times and memory usage.
* **C++ Engine Entry Point:** Designed the core C++ command-line controller (`main.cpp`), featuring distinct execution modes for standard IPC solving, interactive manual scrambling, and automated benchmarking with the state validation.
* **Algorithmic Refinement:** Debugged and fine-tuned the core two-phase **IDA\*** search algorithm to ensure reliable and valid solution generation.
* **Cross-Language Debugging:** Troubleshot and resolved synchronization and rendering issues within the Python-based 3D visualization module, ensuring accurate animated playback of the solution steps.

---
## 1. Prerequisites
- **OS:** Linux
- **Python:** 3.x with the following libraries: `tkinter`, `matplotlib`, `argparse`
- **Compiler:** G++ (for C++ code)
- **Documentation (Optional):** Doxygen

## 2. Setup and Compilation
After cloning the repository, run the configuration script. This will compile the C++ solver and generate the necessary heuristic tables (this process may take a few minutes).

```bash
./setup.sh
```

## 3. Running the Application
The main entry point for the application is the `main.py` script located in the root directory.

### Command-Line Interface (CLI) Mode
The fastest way to test the solver. This mode requires manual input of the cube's face colors.
```bash
python3 main.py --terminal
```
The program will prompt you to enter the state of all 6 faces (U, D, F, B, L, R). 
- **Input format:** A sequence of 9 characters (e.g., `WWWWWWWWW`) for each face.
- **Allowed colors:** `W` (White), `Y` (Yellow), `B` (Blue), `G` (Green), `R` (Red), `O` (Orange).

### Graphical User Interface (GUI) Mode
Launches an interactive window with a grid where you can click to select and set colors.
```bash
python3 main.py
```

## 4. Test Generation
The `src_cpp` directory contains a tool for generating valid, scrambled cube states for testing purposes. 
*(Note: Ensure the main project has been compiled prior to running this).*

To compile and run the generator:
```bash
g++ -Isrc_cpp/include src_cpp/gen_test_case.cpp src_cpp/cube.o -o gen_test_case
./gen_test_case
```
## 5. Automated Testing

After running the `setup.sh` script, you can automatically test the solver's performance on random (but valid) cube states using the following command:

```bash
./src_cpp/main --test
```

## 6. Documentation
Technical code documentation is generated using Doxygen.
```bash
doxygen Doxyfile
cd documentation/latex && make
```
The resulting PDF file will be available at: `documentation/latex/refman.pdf`

## 7. Move Notation
A detailed description of the move notation used in this project (e.g., what `F`, `R'`, `U2` mean) can be found in the following file: 
[MOVES.md](MOVES.md)

---

## Project Workflow

1. **Input Layer (Python):** 
   The user inputs the initial state of the cube via an interactive grid or the terminal. The program validates the input (e.g., checking if the exact number of required colored tiles is present). Once validated, it generates a `.txt` input file containing the digital representation of the cube's state.

2. **Algorithmic Core (C++):** 
   The C++ solver reads the generated file, processes the data, and executes the IDA* solving algorithm. The computed sequence of moves is then saved to a separate `.txt` output file.

3. **Visualization & Output (Python):** 
   The program reads the list of moves from the output file and presents step-by-step instructions for solving the cube. Both the input and output processes support standard input/output (stdin/stdout) redirection.

## Team Information
**Members [Python & C++]:** 359949, 359409, 351683, 361008, 360678, 331060

## Repository Structure
```text
/Rubik-cube-solver
│
├── main.py                 # Main orchestrator script (launches CLI/GUI and Solver)
├── setup.sh                # Installation script (C++ compilation and heuristics generation)
├── README.md               # Project documentation
├── Doxyfile                # Doxygen configuration
├── Refman.pdf              # Generated documentation  
│
├── /data                   # Data exchange folder (temporary files)
│   ├── cube_state.txt      # Cube state (Input for C++)
│   ├── solution_steps.txt  # Solution steps (Output from C++)
│   └── *.txt               # Generated heuristic tables
│
├── /src_cpp                # C++ Source Code (IDA* Solver)
│   ├── main.cpp            # Loads data, runs the solver
│   ├── cube.cpp / .h       # Cube logic and move mechanics
│   ├── ida_star.cpp / .h   # IDA* Algorithm implementation
│   ├── gen_test_case.cpp   # Generator for test cube states
│   └── *.sh files          # Compilation scripts (called by setup.sh)
│
├── /src_py                 # Python Source Code (GUI/CLI)
│   ├── input_gui.py        # Data input window
│   ├── vis_gui.py          # 3D Visualization (Matplotlib)
│   └── cli_handler.py      # Terminal handler
│
└── /documentation          # Documentation generated by Doxygen
```
