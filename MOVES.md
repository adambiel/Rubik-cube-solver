# Rubik's Cube Move Notation

This file describes the standard notation used by the program to output moves (solutions). This is known as Singmaster notation.

## Faces
The cube has 6 faces, denoted by letters corresponding to their English names:
- **U** (Up) – Top face (White)
- **D** (Down) – Bottom face (Yellow)
- **F** (Front) – Front face (Blue)
- **B** (Back) – Back face (Green)
- **L** (Left) – Left face (Red)
- **R** (Right) – Right face (Orange)

*Note: The colors in parentheses are the default colors used in this project, assuming a standard orientation (White on top, Blue in front).*

## Move Types

Each single letter denotes a 90-degree turn of that face **clockwise** (when looking directly at the face).

### 1. Basic Move (e.g., `F`, `R`, `U`)
A 90-degree turn of a face clockwise.
- **F** – Front clockwise
- **U** – Up clockwise (leftwards when looking from the front)

### 2. Prime Move (e.g., `F'`, `R'`, `U'`)
A 90-degree turn of a face **counter-clockwise**.
Often denoted by an apostrophe (e.g., `F'`) or the letter `i`.
**In this program, these moves are denoted by the letter `p` (for "prime"), e.g., `Up`, `Fp`, `Rp`.**
- **F'** (Fp) – Front counter-clockwise
- **U'** (Up) – Up counter-clockwise (rightwards when looking from the front)

### 3. Double Move (e.g., `F2`, `R2`, `U2`)
A 180-degree turn of a face (two 90-degree moves).
Direction does not matter (the result is the same).
- **F2** – Front face by 180 degrees

## Example
- **R U R' U'** – Right up, Up left, Right down, Up right.
