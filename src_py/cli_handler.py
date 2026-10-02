## @file cli_handler.py
#  @brief Command-line interface (CLI) handler module.
#  @details Responsible for collecting user input, formatting it according to
#           the specification required by the C++ engine, and handling the process.

import sys
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent.parent
DATA_DIR = BASE_DIR / "data"
INPUT_FILE = DATA_DIR / "cube_state.txt"
OUTPUT_FILE = DATA_DIR / "solution_steps.txt"

# Definition of face order and allowed colors
FACES_ORDER = ['U', 'D', 'F', 'B', 'L', 'R']
VALID_COLORS = {'W', 'Y', 'G', 'B', 'O', 'R'}

##
# @brief Class responsible for CLI communication and integration.
class SolverCLI:
    
    ##
    # @brief Initializes the solver with a default timeout.
    def __init__(self, timeout_sec=5.0):
        self.timeout = timeout_sec

    ##
    # @brief Formats a raw string into the format required by the .txt file.
    # @details Converts "WWWWWWWWW" into "U: W W W W W W W W W".
    # @param face Face identifier (e.g. 'U').
    # @param stickers A string containing 9 color characters.
    # @return A formatted string representing one face.
    def _format_face_string(self, face, stickers):        
        rows = [stickers[i:i+3] for i in range(0, 9, 3)]
        formatted_rows = [" ".join(list(row)) for row in rows]
        
        # The first line contains the "X: " prefix
        result = f"{face}: {formatted_rows[0]}\n\n"
        result += f"{formatted_rows[1]}\n\n"
        result += f"{formatted_rows[2]}"
        
        return result

    ##
    # @brief Interactively collects the cube state from the user.
    # @details Prompts the user for each of the 6 faces separately,
    #          validating the length and characters.
    # @return A formatted string ready to be written to a file,
    #         or None if the input is interrupted.
    def get_user_input(self):
        print("\n=== CUBE STATE INPUT ===\n")
        print("Allowed colors: B - Blue, G - Green, O - Orange, R - Red, W - White, Y - Yellow")
        
        # Mapping face -> (Color name, Expected center color, Perspective instructions)
        face_info = {
            'U': ("White", 'W', "The top edge is adjacent to the Blue (F) face"),
            'D': ("Yellow", 'Y', "The top edge is adjacent to the Blue (F) face"),
            'F': ("Blue", 'B', "The top edge is adjacent to the White (U) face"),
            'B': ("Green", 'G', "The top edge is adjacent to the White (U) face"),
            'L': ("Red", 'R', "The top edge is adjacent to the White (U) face"),
            'R': ("Orange", 'O', "The top edge is adjacent to the White (U) face")
        }
        
        try:
            while True:
                formatted_parts = []
                all_stickers = ""
                
                for face in FACES_ORDER:
                    color_name, expected_center, orientation_desc = face_info.get(
                        face, ("Unknown", None, "No instructions")
                    )
                    
                    print(f"\nFace {face} [{color_name}]")
                    print(f"Perspective: {orientation_desc}")
                    
                    while True:
                        raw_input = input(
                            f"Enter 9 characters for face {face}: "
                        ).strip().upper()
                        
                        # Validation 1: Length
                        if len(raw_input) != 9:
                            print(
                                f"Error: Exactly 9 characters are required. "
                                f"Received {len(raw_input)}."
                            )
                            continue
                        
                        # Validation 2: Character validity
                        invalid_chars = set(raw_input) - VALID_COLORS
                        if invalid_chars:
                            print(f"Error: Invalid characters: {invalid_chars}")
                            continue
                        
                        # Validation 3: Center sticker
                        if raw_input[4] != expected_center:
                            print(
                                f"Error: The center sticker (character #5) "
                                f"must be {color_name} ({expected_center})."
                            )
                            continue
                        
                        formatted_parts.append(
                            self._format_face_string(face, raw_input)
                        )
                        all_stickers += raw_input
                        break
                
                # Validation 4: Color count
                counts = {c: all_stickers.count(c) for c in VALID_COLORS}
                incorrect_counts = {
                    c: count for c, count in counts.items() if count != 9
                }
                
                if incorrect_counts:
                    print("\nERROR: Invalid number of colors on the cube!")
                    print("Each color must occur exactly 9 times.")
                    print(f"Found: {counts}")
                    print("Please enter the cube state again.\n")
                    continue
                
                full_cube_state = "\n\n".join(formatted_parts)
                return full_cube_state

        except KeyboardInterrupt:
            sys.exit(0)

    ##
    # @brief Saves the formatted cube state to a file.
    # @param cube_string The prepared data string.
    # @return True if the file was saved successfully.
    def save_to_file(self, cube_string):
        try:
            DATA_DIR.mkdir(exist_ok=True)
            with open(INPUT_FILE, 'w') as f:
                f.write(cube_string)
            print(f"\nInput file saved: {INPUT_FILE}")
            return True
        except IOError as e:
            print(f"Error writing file: {e}")
            return False
    
    ##
    # @brief Main application loop.
    def run(self):
        cube_data = self.get_user_input()
        if cube_data:
            if self.save_to_file(cube_data):
                # The self.run_cpp_solver() call will be added here
                print("OK")

if __name__ == "__main__":
    cli = SolverCLI()
    cli.run()

