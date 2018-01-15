
import sys
from math import pi, sin
from gfx import *

pos = [64, 64]
f = 0

def myinit():
    load_spritesheet("assets/spritesheet.bmp")
    read_map("assets/map")
    load_wav("assets/sound.wav", 0)
    
def update():
    global f
    f += 1
    
    if key(K_UP): pos[1] -= 1
    if key(K_DOWN): pos[1] += 1
    if key(K_LEFT): pos[0] -= 1
    if key(K_RIGHT): pos[0] += 1
    if keydown(K_A): play_wav(0)
    if keydown(K_B): quit()
    
    cx, cy = get_cam()
    if pos[0]-cx<16: cx -= 1
    if pos[0]-cx>112: cx += 1
    if pos[1]-cy<16: cy -= 1
    if pos[1]-cy>112: cy += 1
    set_cam(cx, cy)
    
    color(BLACK)
    clear()
    
    color(GREEN)
    map()
    
    color(CYAN)
    rectfill(32, 32, 16, 8)
    color(VIOLET)
    rect(16, 16, 8, 16)
    color(ORANGE)
    circ(16, 48, 8)
    color(BLUE)
    circfill(16, 64, 8)
    color(PINK)
    for n in range(16):
        point(32, 48+(n*2))
    
    color(WHITE)
    sprite(0, pos[0], pos[1], 0.0, 0)
    
    color(YELLOW)
    text(cx, cy, "This is text!")
    
    color(RED)
    sprite(1, 96, 64, sin(f*0.05), SIZE_MEDIUM)
    
def close():
    pass
    

if __name__ == "__main__":
    init(myinit, update, close)
    mainloop()
    