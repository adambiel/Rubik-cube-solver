import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter
from mpl_toolkits.mplot3d.art3d import Poly3DCollection

COLOUR_MAP = {
    "W": "white", "Y": "gold", "G": "limegreen",
    "B": "royalblue", "O": "darkorange", "R": "crimson",
}

def load_data(filename):
    with open(filename, "r", encoding="utf-8") as f:
        lines = [x.strip() for x in f if x.strip() and not x.startswith("#")]

    states = []
    i = 0
    while i < len(lines):
        if lines[i] == "---":
            i += 1
            continue

        cube = {}
        for face in ("U", "D", "F", "B", "R", "L"):
            cube[face] = [lines[i][2:].strip().split()]
            i += 1
            cube[face].append(lines[i].split())
            i += 1
            cube[face].append(lines[i].split())
            i += 1
        states.append(cube)

    return states


def build_cube(cube_state):
    cube = []

    for x in range(3):
        for y in range(3):
            for z in range(3):
                c = {}

                if z == 2:
                    c["U"] = cube_state["U"][2-y][x]
                if z == 0:
                    c["D"] = cube_state["D"][2-y][2-x]
                if y == 2:
                    c["F"] = cube_state["F"][2-z][2-x]
                if y == 0:
                    c["B"] = cube_state["B"][2-z][x]
                if x == 0:
                    c["L"] = cube_state["L"][2-z][2-y]
                if x == 2:
                    c["R"] = cube_state["R"][2-z][y]

                cube.append((x, y, z, c))

    return cube


def draw_cubie(ax, x, y, z, colours):
    faces = []

    if "U" in colours:
        faces.append(([[x,y,z+1],[x+1,y,z+1],[x+1,y+1,z+1],[x,y+1,z+1]], colours["U"]))
    if "D" in colours:
        faces.append(([[x,y,z],[x+1,y,z],[x+1,y+1,z],[x,y+1,z]], colours["D"]))
    if "F" in colours:
        faces.append(([[x,y+1,z],[x+1,y+1,z],[x+1,y+1,z+1],[x,y+1,z+1]], colours["F"]))
    if "B" in colours:
        faces.append(([[x,y,z],[x+1,y,z],[x+1,y,z+1],[x,y,z+1]], colours["B"]))
    if "L" in colours:
        faces.append(([[x,y,z],[x,y+1,z],[x,y+1,z+1],[x,y,z+1]], colours["L"]))
    if "R" in colours:
        faces.append(([[x+1,y,z],[x+1,y+1,z],[x+1,y+1,z+1],[x+1,y,z+1]], colours["R"]))

    for vertices, sticker in faces:
        ax.add_collection3d(
            Poly3DCollection(
                [vertices],
                facecolors=COLOUR_MAP[sticker],
                edgecolors="#202020",
                linewidths=0.8,
            )
        )


def draw_cube(ax, state):
    ax.cla()

    for x, y, z, colours in build_cube(state):
        draw_cubie(ax, x, y, z, colours)

    ax.set_xlim(0, 3)
    ax.set_ylim(0, 3)
    ax.set_zlim(0, 3)
    ax.set_box_aspect((1, 1, 1))
    ax.set_proj_type("ortho")
    ax.view_init(elev=25, azim=-55)
    ax.set_axis_off()


moves = load_data("test.txt")

# A single large view works much better in a GIF than four small views.
fig = plt.figure(figsize=(7, 7), facecolor="#f7f7f7")
ax = fig.add_subplot(111, projection="3d")
fig.subplots_adjust(left=0, right=1, bottom=0, top=0.84)


def update(frame):
    draw_cube(ax, moves[frame])

    if frame == 0:
        subtitle = "Initial configuration"
    elif frame == len(moves) - 1:
        subtitle = "Solved"
    else:
        subtitle = f"Solution step {frame} / {len(moves)-1}"

    fig.suptitle(
        "Rubik's Cube Solver",
        fontsize=20,
        fontweight="bold",
        y=0.94,
    )
    ax.set_title(subtitle, fontsize=12, pad=8)

    return []


# Hold the initial and final configurations a little longer.
if len(moves) > 1:
    frames = [0, 0] + list(range(1, len(moves) - 1)) + [len(moves)-1] * 3
else:
    frames = [0] * 3

animation = FuncAnimation(
    fig,
    update,
    frames=frames,
    interval=650,
    repeat=False,
    blit=False,
)

animation.save(
    "visualization.gif",
    writer=PillowWriter(fps=2),
    dpi=120,
)

plt.show()
