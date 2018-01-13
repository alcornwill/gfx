

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "vec.h"

#ifndef GFX_H
#define GFX_H

#define DEGREES(RAD) (RAD * (180.0 / M_PI))
#define RADIANS(DEG) (DEG * (M_PI / 180.0))

// CONFIG
#define WINDOW_NAME "GFX"
#define FPS 30
#define TICKS_PER_SECOND ((float)1000 / (float)FPS)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define SPRITE_SHEET_WIDTH 16
#define SPRITE_SHEET_HEIGHT 16
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8
#define CHAR_SIZE (CHAR_WIDTH * CHAR_HEIGHT)
#define MAP_WIDTH 64
#define MAP_HEIGHT 64
#define SPRITES_LENGTH 4096
#define TILES_LENGTH 4096
#define MAP_LENGTH 4096  
#define TILESET_INDEX (SPRITES_LENGTH / 64)
#define FONT_INDEX ((SPRITES_LENGTH + TILES_LENGTH) / 64)
#define SPRITE_SHEET_PATH "../assets/spritesheet.bmp"
#define MAP_PATH "../assets/map"
#define NUM_LAYERS 1

// KEYMAP
#define KM_UP SDL_SCANCODE_UP
#define KM_DOWN SDL_SCANCODE_DOWN
#define KM_LEFT SDL_SCANCODE_LEFT
#define KM_RIGHT SDL_SCANCODE_RIGHT
#define KM_A SDL_SCANCODE_Z
#define KM_B SDL_SCANCODE_X
#define KM_START SDL_SCANCODE_RETURN
#define KM_SELECT SDL_SCANCODE_SPACE

// KEY
#define K_UP 0
#define K_DOWN 1
#define K_LEFT 2
#define K_RIGHT 3
#define K_A 4
#define K_B 5
#define K_START 6
#define K_SELECT 7

const int g_keymap[8];

// COLOR
#define BLACK       {0,   0,   0,   255}
#define WHITE       {255, 255, 255, 255}
#define RED         {136, 0,   0,   255}
#define CYAN        {170, 255, 238, 255}
#define VIOLET      {204, 68 , 204, 0  }
#define GREEN       {0,   204, 85,  255}
#define BLUE        {0,   0,   170, 255}
#define YELLOW      {238, 238, 119, 255}
#define ORANGE      {221, 136, 85,  255}
#define BROWN       {102, 68,  0,   255}
#define PINK        {255, 119, 119, 255}
#define DK_GREY   {51,  51,  51,  255}
#define GREY        {119, 119, 119, 255}
#define LT_GREEN {170, 255, 102, 255}
#define LT_BLUE  {0,   136, 255, 255}
#define LT_GREY  {187, 187, 187, 255}

// SPRITE FLAGS
#define SIZE_SMALL 0
#define SIZE_MEDIUM 1
#define SIZE_BIG 2
#define SIZE_HUGE 3
#define SIZE_FLAGS 0x11

#define SHAPE_SQUARE 0
#define SHAPE_WIDE (1 << 2)
#define SHAPE_TALL (2 << 2)
#define SHAPE_FLAGS 0x1100

#define SCALE_X (1 << 4)
#define SCALE_Y (1 << 5)

#define FLIP_HORIZONTAL (1 << 6)
#define FLIP_VERTICAL (1 << 7)

#define ROT_NONE 11
#define ROT_90 12
#define ROT_180 13
#define ROT_270 14

// MAP MODE
#define MM_TALL
#define MM_WIDE
#define MM_SQUARE
#define MM_MAP

// hmm, user define?
#define NUM_WAVBUFFER 6

typedef struct Vec2 Vec2;
typedef struct Color Color;
typedef struct Sprite Sprite;

struct Vec2 {
    int x;
    int y;
};

struct Color {
    int r;
    int g;
    int b;
    int a;
};

const Vec2 g_sprite_shapes[12];

int g_window_width;
int g_window_height;
SDL_Window *g_window;
SDL_Renderer *g_renderer;
SDL_Texture *g_spritesheet_texture;     // pointer to either k or nk
SDL_Texture *g_spritesheet_texture_k;   // chroma keyed
SDL_Texture *g_spritesheet_texture_nk;  // non chroma keyed...
int g_layer;                        // active layer index
SDL_Texture *g_layers[NUM_LAYERS];  // holds all layers
unsigned char g_map[MAP_LENGTH];
unsigned char g_keys[8];
int g_keysdown[8];
int g_keysup[8];
int g_frame;
float g_time;
float g_dt;
Vec2 g_cursor; // text cursor
Vec2 g_camera; // offset to all coordinates
void (*user_init)();
void (*user_update)();
void (*user_close)();
// audio stuff
typedef struct WavBuffer WavBuffer;
struct WavBuffer {
    SDL_AudioSpec spec;
    unsigned int length;
    unsigned char *buffer;    
};
WavBuffer g_wavbuffers[NUM_WAVBUFFER];
unsigned char *g_audio_pos;
unsigned int g_audio_len;
float g_volume;

void gfx_load(void (*init)(), void (*update)(), void (*close)());
int gfx_init();
int load_media();
void load_wav(char *path, int index);
void init_audio();
SDL_Texture *load_texture( char *path );
void gfx_close();
int gfx_mainloop();

void play_wav(int index);
int gfx_get_key(int i);
int gfx_get_keydown(int i);
int gfx_get_keyup(int i);

void gfx_set_map(int x, int y, unsigned int value);
int gfx_read_map();
int gfx_write_map();
int gfx_map_data(int x, int y);
int gfx_map_flags(int x, int y);
void gfx_set_map_flags(int x, int y, int value);

void gfx_clear();
void gfx_set_layer(int layer);
void gfx_set_color(int i);
void gfx_set_key(int i);
void gfx_draw_rect(int x, int y, int w, int h);
void gfx_draw_rect_fill(int x, int y, int w, int h);
void gfx_draw_line(int x1, int y1, int x2, int y2);
void gfx_draw_point(int x, int y);
void gfx_draw_circ(int x0, int y0, int radius);
void gfx_draw_circ_fill(int x0, int y0, int radius);
void gfx_draw_text_at(int x, int y, char *text);
void gfx_draw_text(char *text);
void gfx_draw_char(int index, int x, int y);
void gfx_draw_sprite(int index, int x, int y, int flags);
void gfx_draw_sprite_rot(int index, int x, int y, float r, int flags);
void gfx_draw_map(int x, int y, int mx, int my);
void gfx_draw_map_auto();

#endif
