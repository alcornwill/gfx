
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"

#define CURSOR_SPD 2
#define MAX_TILES 16
#define BORDER 16

Vec2 g_mapedit_cursor = {64, 64};
int g_tile_index = 0;
// int g_flag = 0;
int g_help = 0;

void paint() {
    int x = g_mapedit_cursor.x / 8;
    int y = g_mapedit_cursor.y / 8;
    gfx_set_map(x, y, g_tile_index);
}

void init() {
    // ...
}

void update() {
    // UPDATE
    if (g_keys[K_A]) {
        paint();
    }
    if (g_keys[K_B]) {
        if (g_keysdown[K_UP]) {
            g_tile_index += 1;
            g_tile_index %= MAX_TILES;
        }
        if (g_keysdown[K_DOWN]) {
            g_tile_index -= 1;
            if (g_tile_index < 0) {
                g_tile_index = MAX_TILES;
            }
        }
    } else {
        if (g_keys[K_UP]) {
            g_mapedit_cursor.y -= CURSOR_SPD;
        }
        if (g_keys[K_LEFT]) {
            g_mapedit_cursor.x -= CURSOR_SPD;
        }
        if (g_keys[K_DOWN]) {
            g_mapedit_cursor.y += CURSOR_SPD;
        } 
        if (g_keys[K_RIGHT]) {
            g_mapedit_cursor.x += CURSOR_SPD;
        }
    }
    if (g_keysdown[K_START]) {
        g_help = !g_help;
    }

    // clamp cursor to map bounds
    g_mapedit_cursor.x = CLAMP(g_mapedit_cursor.x, 0, 512);
    g_mapedit_cursor.y = CLAMP(g_mapedit_cursor.y, 0, 512);
    
    // move camera if cursor at screen bounds
    if (g_mapedit_cursor.y < g_camera.y + BORDER) {
        g_camera.y -= CURSOR_SPD;
    } else if (g_mapedit_cursor.y > g_camera.y + (SCREEN_HEIGHT - BORDER)) {
        g_camera.y += CURSOR_SPD;
    }
    if (g_mapedit_cursor.x < g_camera.x + BORDER) {
        g_camera.x -= CURSOR_SPD;
    } else if(g_mapedit_cursor.x > g_camera.x + (SCREEN_WIDTH - BORDER)) {
        g_camera.x += CURSOR_SPD;
    }
    
    // DRAW

    // clear screen
    gfx_set_color(0);
    gfx_clear();
    
    // draw tilemap
    gfx_set_color(5); // green
    gfx_draw_map_auto();

    // draw text
    gfx_set_color(7); // yellow
    char message[16] = {0};
    sprintf(message, "tile: %i", g_tile_index);
    gfx_draw_text_at(g_camera.x, g_camera.y, message);
    sprintf(message, "curs: %03i, %03i", g_mapedit_cursor.x, g_mapedit_cursor.y);
    gfx_draw_text_at(g_camera.x, g_camera.y+8, message);
    
    // draw cursor
    gfx_set_color(1); // white
    gfx_draw_line(g_mapedit_cursor.x-2, g_mapedit_cursor.y, 
                  g_mapedit_cursor.x+2, g_mapedit_cursor.y);
    gfx_draw_line(g_mapedit_cursor.x, g_mapedit_cursor.y-2, 
                  g_mapedit_cursor.x, g_mapedit_cursor.y+2 );
                  
    // draw help
    if (g_help) {
        gfx_set_color(7);
        sprintf(message, "      HELP      ");
        gfx_draw_text_at(g_camera.x, g_camera.y, message);
        sprintf(message, "MOVE:   CRS KEYS");
        gfx_draw_text_at(g_camera.x, g_camera.y+8, message);
        sprintf(message, "DRAW:          A");
        gfx_draw_text_at(g_camera.x, g_camera.y+16, message);
        sprintf(message, "TILE:  B+UP/DOWN");
        gfx_draw_text_at(g_camera.x, g_camera.y+24, message);
        sprintf(message, "HELP:      START");
        gfx_draw_text_at(g_camera.x, g_camera.y+32, message);
        sprintf(message, "SAVE:        F10");
        gfx_draw_text_at(g_camera.x, g_camera.y+40, message);
    }
}

void close() {
    // ...
}

int main( int argc, char* args[] )
{
    gfx_load(init, update, close);
    gfx_mainloop();
    return 0;
}