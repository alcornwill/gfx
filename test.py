
import sys
from math import pi, sin
import gfx

pos = [64, 64]
f = 0

def init():
    gfx.load_spritesheet("assets/spritesheet.bmp")
    gfx.read_map("assets/map")
    gfx.load_wav("assets/sound.wav", 0)
    
def update():
    global f
    f += 1
    
    if gfx.key(0): pos[1] -= 1
    if gfx.key(1): pos[1] += 1
    if gfx.key(2): pos[0] -= 1
    if gfx.key(3): pos[0] += 1
    if gfx.keydown(4): gfx.play_wav(0)
    
    cx, cy = gfx.get_cam()
    if pos[0]-cx<16: cx -= 1
    if pos[0]-cx>112: cx += 1
    if pos[1]-cy<16: cy -= 1
    if pos[1]-cy>112: cy += 1
    gfx.set_cam(cx, cy)
    
    gfx.color(0)
    gfx.clear()
    
    gfx.color(5)
    gfx.map()
    
    gfx.color(3)
    gfx.rectfill(32, 32, 16, 8)
    gfx.color(4)
    gfx.rect(16, 16, 8, 16)
    gfx.color(8)
    gfx.circ(16, 48, 8)
    gfx.color(14)
    gfx.circfill(16, 64, 8)
    gfx.color(10)
    for n in range(16):
        gfx.point(32, 48+(n*2))
    
    gfx.color(1)
    gfx.sprite(0, pos[0], pos[1], 0.0, 0)
    
    gfx.color(7)
    gfx.text(cx, cy, "This is text!")
    
    gfx.color(2)
    gfx.sprite(1, 96, 64, sin(f*0.05), 1)
    
def close():
    pass
    

if __name__ == "__main__":
    gfx.init(init, update, close)
    gfx.mainloop()
    