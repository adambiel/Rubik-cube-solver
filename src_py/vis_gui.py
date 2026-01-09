import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import time
from matplotlib.animation import FuncAnimation
import math

#kolory kostki (W->bialy, Y-zolty, G-zielony, B-niebieski, O-pomaranczowy, R-czerwony)

#nazwy scian (U-gora, D-dol, F-przod, B-tyl, L-lewa, R-prawa)
colour_map={
    'W': 'white',
    'Y': 'yellow',
    'G': 'green',
    'B': 'blue',
    'O': 'orange',
    'R': 'red'
}
def load_data(filename):

    # otworzenie pliku 
    with open(filename, 'r') as f:
        lines = [line.strip() for line in f if line.strip() and not line.startswith('#')]

    moves=[] #lista z kolejnymi ruchami
    i=0
    while i < len(lines): 
        if(lines[i]=="---"):
            i+=1
            continue

        cube={} #tworzenie slownika w ktorym sa listy  np dla sciany 'U' trzymamy odwzorowanie jej 3x3  

        for wall in ['U', 'D', 'F', 'B', 'L', 'R']:
            cube[wall]=[] 
            cube[wall].append(lines[i][2:].strip().split()) #pierwszy wiersz ma dodatkowo 'id' sciany i ':' 
            i+=1
            cube[wall].append(lines[i].strip().split())
            i+=1
            cube[wall].append(lines[i].strip().split())
            i+=1
        
        moves.append(cube)

    return moves

def build_cube(cube_state):
    cube = []
    # U: z=2, D: z=0, F: y=2, B:y=0, L:x=0, R:x=2
    for xi in range(3):
        for yi in range(3):
            for zi in range(3):
                # okreslamy kolory widocznych scian
                colours = {}
                # gorna/dolna
                if zi==2: colours['U'] = cube_state['U'][2-yi][xi]
                if zi==0: colours['D'] = cube_state['D'][yi][xi]
                # przednia/tylna
                if yi==2: colours['F'] = cube_state['F'][2-zi][xi]
                if yi==0: colours['B'] = cube_state['B'][zi][xi]
                # lewa/prawa
                if xi==0: colours['L'] = cube_state['L'][2-zi][yi]
                if xi==2: colours['R'] = cube_state['R'][2-zi][yi]

                cube.append((xi, yi, zi, colours))
    return cube


def draw_cubie(ax, x, y, z, colours):
    walls = []
    # gorna
    if 'U' in colours:
        U = colours['U']
        walls.append([[x,y,z+1],[x+1,y,z+1],[x+1,y+1,z+1],[x,y+1,z+1], colour_map[U]])
    # dolna 
    if 'D' in colours:
        D = colours['D']
        walls.append([[x,y,z],[x+1,y,z],[x+1,y+1,z],[x,y+1,z], colour_map[D]])
    # przednia
    if 'F' in colours:
        F = colours['F']
        walls.append([[x,y+1,z],[x+1,y+1,z],[x+1,y+1,z+1],[x,y+1,z+1], colour_map[F]])
    # tylna
    if 'B' in colours:
        B = colours['B']
        walls.append([[x,y,z],[x+1,y,z],[x+1,y,z+1],[x,y,z+1], colour_map[B]])
    # lewa
    if 'L' in colours:
        L = colours['L']
        walls.append([[x,y,z],[x,y+1,z],[x,y+1,z+1],[x,y,z+1], colour_map[L]])
    # prawa
    if 'R' in colours:
        R = colours['R']
        walls.append([[x+1,y,z],[x+1,y+1,z],[x+1,y+1,z+1],[x+1,y,z+1], colour_map[R]])

    # rysowanie wszystkich scian 
    artists = []
    for vert in walls:
        poly = Poly3DCollection([vert[:4]], facecolors=vert[4], edgecolors='black')
        ax.add_collection3d(poly)
        artists.append(poly)
    return artists


def update(frame): #funkcja update koloruje kostke od nowa na kolory z ruchu ktory przetwarza z roznych perspektyw
    for idx, ax in enumerate(axes):
        ax.cla()
        cube_state = moves[frame]
        cube = build_cube(cube_state)
        artists = []
        for x, y, z, colours in cube:
            artists += draw_cubie(ax, x, y, z, colours)
        ax.set_xlim(0,3)
        ax.set_ylim(0,3)
        ax.set_zlim(0,3)
        ax.set_axis_off()
        ax.set_box_aspect([3,3,3])
        ax.set_proj_type('ortho')
        elevs = [30, 30, -30, -30]
        azims = [-45, 135, -45, 135]
        ax.view_init(elev=elevs[idx], azim=azims[idx] + frame*3)
        ax.set_title(f"Obraz {idx+1}")
    return []

moves = load_data("test.txt")  # rozpakowanie pliku txt wynik programu 2. (plik testowy wrzuce na github)


#tworzenie figury 3D

fig = plt.figure(figsize=(10,10))
axes=[
    fig.add_subplot(2,2,1, projection='3d'),
    fig.add_subplot(2,2,2, projection='3d'),
    fig.add_subplot(2,2,3, projection='3d'),
    fig.add_subplot(2,2,4, projection='3d')
]

#animacja kostki
 
ani = FuncAnimation(fig, update, frames=len(moves), interval=1000)

#nwm jak dziala z plt.show() mozna dopisac ta wersje ale u mnie terminal wybucha jak tak probuje napisac  

#zapisywanie do pliku wizulizacja_kostki.gif

ani.save("wizualizacja_kostki.gif", writer="pillow", fps=2)