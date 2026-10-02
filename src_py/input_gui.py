##
# @file cube_input_gui.py
# @brief Graphical user interface module for entering the Rubik's Cube state.
# @details Allows the user to "paint" the cube grid, validates the entered data
#          (color count, completeness), and saves it to a text file.

import tkinter as tk
from tkinter import messagebox

COLORS = {
    'white':  '#FFFFFF',
    'yellow': '#FFFF00',
    'green':  '#00FF00',
    'blue':   '#0000FF',
    'red':    '#FF0000',
    'orange': '#FFA500',
    'grey':   '#D3D3D3'
}
HEX_TO_NAME = {v: k for k, v in COLORS.items()}
ENGLISH_NAME = {
    'white':  'white',
    'yellow': 'yellow',
    'green':  'green',
    'blue':   'blue',
    'red':    'red',
    'orange': 'orange',
    'grey':   'grey'
}


class InputGUI:
    ##
    # @brief Main class responsible for managing the graphical user interface.
    def __init__(self, root):
        ##
        # @brief Constructor for the InputGUI class.
        # @details Initializes the main window, state variables, and builds the interface layout.
        # @param root Main tkinter window object (tk.Tk).
        self.root = root
        self.root.title("Cube Grid")
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(1, weight=1)
        self.current_color = 'white'
        self.tiles = {}
        self.create_palette()
        self.grid_frame = tk.Frame(self.root, bg="#eeeeee", padx=25, pady=25)
        self.grid_frame.grid(row=1, column=0, sticky="nsew")
        self.create_grid()
        save_button = tk.Button(
            self.root,
            text="SAVE",
            bg="#4CAF50",
            fg="white",
            font=("Arial", 12, "bold"),
            command=self.verify_and_save
        )
        save_button.grid(row=2, column=0, pady=20, sticky="ew", padx=50)
        
    def create_palette(self):
        ##
        # @brief Creates the top bar containing the color palette and help button.
        # @details Generates a button for each color defined in COLORS (except grey).
        palette_frame = tk.Frame(self.root, pady=10, bg="#dddddd")
        palette_frame.grid(row=0, column=0, sticky="ew")
        tk.Label(
            palette_frame,
            text="Choose a color: ",
            bg="#dddddd",
            font=("Arial", 12)
        ).pack(side=tk.LEFT, padx=10)
        
        for color_name, color_hex in COLORS.items():
            if color_name == 'grey':
                continue
            button = tk.Button(
                palette_frame,
                bg=color_hex,
                width=4,
                height=2,
                command=lambda c=color_name: self.set_brush(c)
            )
            button.pack(side=tk.LEFT, padx=5)
            
        help_button = tk.Button(
            palette_frame,
            text="Help",
            bg="#2196F3",
            fg="white",
            font=("Arial", 11, "bold"),
            command=self.show_instructions
        )
        help_button.pack(side=tk.RIGHT, padx=20)
        
    def show_instructions(self):
        ##
        # @brief Displays a dialog window containing instructions for using the program.
        text = (
            "HOW TO USE THE PROGRAM:\n\n"
            "1. Select a color from the bar at the top.\n"
            "2. Click a tile on the grid to paint it.\n"
            "3. The center tiles are locked – they define the color of each face.\n"
            "4. You must paint the entire cube (there must be no grey tiles).\n"
            "5. Each color must appear on exactly 9 tiles.\n"
            "6. When you are finished, click the green SAVE button."
        )
        messagebox.showinfo("Instructions", text)
            
    def set_brush(self, color):
        ##
        # @brief Sets the currently selected "brush" color.
        # @param color Color name (key from the COLORS dictionary).
        self.current_color = color

    def create_grid(self):
        ##
        # @brief Configures the grid layout and places the cube faces.
        # @details Defines the mapping of face names (U, L, F, R, B, D) to positions (x, y).
        for col in range(12):
            self.grid_frame.columnconfigure(col, weight=1)
        for row in range(9):
            self.grid_frame.rowconfigure(row, weight=1)
            
        faces_layout = {
            'U': (3, 0),
            'L': (0, 3),
            'F': (3, 3),
            'R': (6, 3),
            'B': (9, 3),
            'D': (3, 6)
        }
        
        for face_name, (offset_x, offset_y) in faces_layout.items():
            self.create_face(face_name, offset_x, offset_y)

    def create_face(self, face_name, offset_x, offset_y):
        ##
        # @brief Creates a 3x3 grid of buttons for a single cube face.
        # @details Handles the logic for locking the center tiles.
        # @param face_name Face symbol (e.g. 'U', 'F').
        # @param offset_x Column position in the main grid.
        # @param offset_y Row position in the main grid.
        center_colors = {
            'U': 'white',
            'D': 'yellow',
            'F': 'blue',
            'B': 'green',
            'R': 'orange',
            'L': 'red'
        }
        
        for r in range(3):
            for c in range(3):
                text_on_button = ""
                color_bg = COLORS['grey']
                
                if r == 1 and c == 1:
                    text_on_button = face_name
                    color_bg = COLORS[center_colors[face_name]]
                    
                button = tk.Button(
                    self.grid_frame,
                    bg=color_bg,
                    text=text_on_button,
                    borderwidth=1,
                    relief="solid",
                    font=("Arial", 15, "bold")
                )
                
                if r != 1 or c != 1:
                    button.config(
                        command=lambda b=button: self.paint_tile(b)
                    )
                    
                button.grid(
                    row=offset_y + r,
                    column=offset_x + c,
                    sticky="nsew",
                    padx=1,
                    pady=1
                )
                
                self.tiles[(face_name, r, c)] = button
                
    def paint_tile(self, button_widget):
        ##
        # @brief Changes the background color of the clicked button.
        # @param button_widget Button object that was clicked.
        color_hex = COLORS[self.current_color]
        button_widget.configure(bg=color_hex)
        
    def verify_and_save(self):
        ##
        # @brief Validates the cube state and initiates the save operation.
        # @details Checks that there are no grey tiles and that each color appears
        #          exactly 9 times. If validation succeeds, calls save_to_file()
        #          and closes the program.
        color_counts = {
            name: 0 for name in COLORS if name != 'grey'
        }
        result = {}
        
        for (face_name, r, c), button in self.tiles.items():
            bg_color_hex = button.cget('bg')
            color_name = HEX_TO_NAME[bg_color_hex]
            
            if color_name == 'grey':
                messagebox.showerror(
                    "Error",
                    "The cube is not completely painted.\n"
                    "Please fill in all grey tiles."
                )
                return
                
            color_counts[color_name] += 1
            result[(face_name, r, c)] = color_name
            
        for color, count in color_counts.items():
            if count != 9:
                messagebox.showerror(
                    "Error",
                    f"Each color must appear exactly 9 times.\n"
                    f"{ENGLISH_NAME[color].upper()} appears {count} times."
                )
                return
                
        messagebox.showinfo(
            "Success",
            "The cube state is valid! Saving data."
        )
        
        self.save_to_file(result)
        self.root.destroy()

    def save_to_file(self, data):
        ##
        # @brief Saves the processed data to cube_input.txt.
        # @details Transforms color names into their corresponding letters and
        #          rotates the 'U' face by 180 degrees according to the algorithm requirements.
        # @param data Dictionary containing the cube state
        #              { (face, r, c): color_name }.
        colors = {
            'white': 'W',
            'yellow': 'Y',
            'green': 'G',
            'blue': 'B',
            'red': 'R',
            'orange': 'O'
        }
        
        face_order = ['U', 'D', 'F', 'B', 'L', 'R']
        
        try:
            with open("cube_input.txt", "w") as f:
                for face in face_order:
                    f.write(f"{face}: ")
                    
                    for r in range(3):
                        row_colors = []
                        
                        for c in range(3):
                            if face == 'U':
                                name = data[(face, 2-r, 2-c)]
                            else:
                                name = data[(face, r, c)]
                                
                            letter = colors[name]
                            row_colors.append(letter)
                            
                        line = " ".join(row_colors)
                        f.write(line + "\n")
                        
        except Exception as e:
            messagebox.showerror(
                "Save Error",
                f"Error: {e}"
            )


if __name__ == "__main__":
    root = tk.Tk()
    root.minsize(600, 500)
    cube_editor = InputGUI(root)
    root.mainloop()

