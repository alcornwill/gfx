
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"

#define CURSOR_SPD 2
#define MAX_TILES 16

Vec2 g_mapedit_cursor = {64, 64};
int g_tile_index = 0;

void init() {
    // ...
}

void update() {
    // UPDATE
    if (g_keysdown[K_A]) {
        g_tile_index -= 1;
        if (g_tile_index < 0) {
            g_tile_index = MAX_TILES;
        }
    }
    if (g_keysdown[K_B]) {
        g_tile_index += 1;
        g_tile_index %= MAX_TILES;
    }
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
    if (g_keys[K_START]) {
        // todo set the tile at mapedit cursor to tile_index
        int x = g_mapedit_cursor.x / 8;
        int y = g_mapedit_cursor.y / 8;
        gfx_set_map(x, y, g_tile_index);
    }
    
    if (g_mapedit_cursor.y < g_camera.y + 16) {
        g_camera.y -= CURSOR_SPD;
    } else if (g_mapedit_cursor.y > g_camera.y + (SCREEN_HEIGHT - 16)) {
        g_camera.y += CURSOR_SPD;
    }
    if (g_mapedit_cursor.x < g_camera.x + 16) {
        g_camera.x -= CURSOR_SPD;
    } else if(g_mapedit_cursor.x > g_camera.x + (SCREEN_WIDTH - 16)) {
        g_camera.x += CURSOR_SPD;
    }
    // DRAW

    //Clear screen
    gfx_set_color(0);
    gfx_clear();

    // how to abstract? ...
    // set_layer?
    // or have sprite layer, tileset layer?
    SDL_SetRenderTarget( g_renderer, g_render_texture );
    gfx_clear();
    
    // draw tilemap
    gfx_draw_map_auto();

    // draw text
    char message[16] = {0};
    sprintf(message, "tile index: %i", g_tile_index);
    gfx_draw_text_at(g_camera.x, g_camera.y, message);
    sprintf(message, "cursor: %i, %i", g_mapedit_cursor.x, g_mapedit_cursor.y);
    gfx_draw_text_at(g_camera.x, g_camera.y+8, message);
    
    // draw cursor
    gfx_set_color(7);
    gfx_draw_line( g_mapedit_cursor.x-2, g_mapedit_cursor.y, g_mapedit_cursor.x+2, g_mapedit_cursor.y );
    gfx_draw_line( g_mapedit_cursor.x, g_mapedit_cursor.y-2, g_mapedit_cursor.x, g_mapedit_cursor.y+2 );
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