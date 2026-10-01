# @file main.py
# @brief Main script orchestrating the application's operation.
# @details Responsible for selecting the data input mode (CLI/GUI),
#          running the C++ solver, and visualizing the results.

import argparse
import sys
import os
import subprocess
from pathlib import Path
import shutil
import tkinter as tk

# Set the base path (project directory)
BASE_DIR = Path(__file__).resolve().parent

# Add src_py to sys.path so imports work
sys.path.append(str(BASE_DIR / "src_py"))

try:
    from cli_handler import SolverCLI, DATA_DIR, INPUT_FILE, OUTPUT_FILE
    import input_gui
except ImportError as e:
    print(f"Critical error: Cannot import modules from src_py. {e}")
    sys.exit(1)

CPP_EXEC = BASE_DIR / "src_cpp" / "main"
GUI_SCRIPT = BASE_DIR / "src_py" / "vis_gui.py"

##
# @brief Runs the solver written in C++.
# @details Calls the executable, passing the cube state via stdin and saving the result to stdout.
#          Sets the working directory to src_cpp so the solver correctly finds heuristics in ../data.
# @return True if the solver completed successfully, False otherwise.
def run_cpp_solver():
    """Runs the C++ solver executable."""
    # Check the executable file
    if not CPP_EXEC.exists():
        print(f"Error: C++ executable not found at {CPP_EXEC}")
        print("Please run the setup script in the main directory:")
        print("  ./setup.sh")
        return False

    # Check heuristics
    heuristics = ["eph.txt", "eoh.txt", "cph.txt"]
    missing_heuristics = [h for h in heuristics if not (DATA_DIR / h).exists()]
    
    if missing_heuristics:
        print(f"Error: Missing heuristic files: {', '.join(missing_heuristics)}")
        print("Please run the setup script to generate them:")
        print("  ./setup.sh")
        return False
    
    if not INPUT_FILE.exists():
        print(f"Error: Cube state file not found at {INPUT_FILE}")
        return False
        
    print(f"\nRunning solver: {CPP_EXEC} < {INPUT_FILE} > {OUTPUT_FILE}")
    try:
        # The C++ code expects to be run from the src_cpp directory so ../data/ paths work
        # Pass cube_state.txt as stdin, solution_steps.txt as stdout
        with open(INPUT_FILE, 'r') as stdin_f, open(OUTPUT_FILE, 'w') as stdout_f:
            subprocess.run(
                [str(CPP_EXEC)], 
                cwd=CPP_EXEC.parent, # Set CWD to src_cpp
                stdin=stdin_f, 
                stdout=stdout_f, 
                check=True
            )
        return True
    except subprocess.CalledProcessError as e:
        print(f"Solver terminated with error code {e.returncode}")
        return False
    except OSError as e:
        print(f"Execution failed: {e}")
        return False

##
# @brief Runs the solution visualization.
# @details Copies the solver's result to the test.txt file (required by vis_gui.py)
#          and runs the visualization script.
def run_gui():
    """Runs the visualization GUI."""
    print(f"\nRunning visualization...")
    if not GUI_SCRIPT.exists():
        print(f"Error: GUI script not found at {GUI_SCRIPT}")
        return

    try:
        # Workaround for vis_gui.py, which looks for 'test.txt' in CWD (now root)
        if OUTPUT_FILE.exists():
            shutil.copy(str(OUTPUT_FILE), "test.txt")
        else:
            print(f"Warning: Solution file {OUTPUT_FILE} not found. Visualization may fail.")
    except Exception as e:
        print(f"Failed to copy solution to test.txt: {e}")

    try:
        # Ensure PYTHONPATH contains src_py for the subprocess
        env = os.environ.copy()
        env["PYTHONPATH"] = str(BASE_DIR / "src_py") + ":" + env.get("PYTHONPATH", "")
        subprocess.run([sys.executable, str(GUI_SCRIPT)], check=True, env=env)
    except Exception as e:
        print(f"Failed to run GUI: {e}")

##
# @brief Main function of the program.
# @details Parses command line arguments, controls data flow
#          between input, solver, and visualization modules.
def main():
    parser = argparse.ArgumentParser(description="Rubik's Cube Solver Orchestrator")
    parser.add_argument('--terminal', "-t", action='store_true', help="Use terminal input mode")

    args = parser.parse_args()
    
    cube_state_ready = False
    
    if args.terminal:
        cli = SolverCLI()
        print("Running terminal mode...")
        # cli.run() executes the input loop and saves to file
        cli.run()
        if INPUT_FILE.exists():
            cube_state_ready = True
    else:
        print("Running graphical mode (GUI)...")
        try:
            root = tk.Tk()
            root.minsize(600, 500)
            app = input_gui.InputGUI(root)
            root.mainloop()
            
            gui_output = Path.cwd() / "cube_input.txt"
            
            if gui_output.exists():
                print(f"GUI closed. Moving {gui_output} to {INPUT_FILE}...")
                DATA_DIR.mkdir(exist_ok=True)
                shutil.move(str(gui_output), str(INPUT_FILE))
                cube_state_ready = True
            else:
                print("GUI closed, but output file not found (maybe canceled?).")
                
        except Exception as e:
            print(f"Error running GUI: {e}")
            print("Recommended to use --terminal mode.")
        
    if cube_state_ready:
        if run_cpp_solver():
            print("Solution generated.")
            run_gui()
        else:
            print("Skipping visualization due to solver error.")

if __name__ == "__main__":
    main()
