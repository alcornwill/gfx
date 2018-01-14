

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#ifndef GFX_H
#define GFX_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define DEGREES(RAD) (RAD * (180.0 / M_PI))
#define RADIANS(DEG) (DEG * (M_PI / 180.0))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define CLAMP(A, B, C) (MAX(MIN(A, C), B))

// CONFIG
#define WINDOW_NAME "GFX"
#define FPS 30
#define TICKS_PER_SECOND (1000 / (float)FPS)
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
#define NUM_WAVBUFFER 24 // supports loading up to 24 wav files
#define AUDIO_SAMPLE_RATE 44100
#define AUDIO_FORMAT AUDIO_S16
#define AUDIO_CHANNELS 2
#define AUDIO_SAMPLES 1024 // smaller == less latency

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

// COLOR
#define BLACK    0
#define WHITE    1
#define RED      2
#define CYAN     3
#define VIOLET   4
#define GREEN    5
#define BLUE     6
#define YELLOW   7
#define ORANGE   8
#define BROWN    9
#define PINK     10
#define DK_GREY  11
#define GREY     12
#define LT_GREEN 13
#define LT_BLUE  14
#define LT_GREY  15

#define PAL_BLACK     {0,   0,   0,   255}
#define PAL_WHITE     {255, 255, 255, 255}
#define PAL_RED       {136, 0,   0,   255}
#define PAL_CYAN      {170, 255, 238, 255}
#define PAL_VIOLET    {204, 68 , 204, 0  }
#define PAL_GREEN     {0,   204, 85,  255}
#define PAL_BLUE      {0,   0,   170, 255}
#define PAL_YELLOW    {238, 238, 119, 255}
#define PAL_ORANGE    {221, 136, 85,  255}
#define PAL_BROWN     {102, 68,  0,   255}
#define PAL_PINK      {255, 119, 119, 255}
#define PAL_DK_GREY   {51,  51,  51,  255}
#define PAL_GREY      {119, 119, 119, 255}
#define PAL_LT_GREEN  {170, 255, 102, 255}
#define PAL_LT_BLUE   {0,   136, 255, 255}
#define PAL_LT_GREY   {187, 187, 187, 255}

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



typedef struct Vec2 Vec2;
typedef struct Color Color;
typedef struct WavBuffer WavBuffer;


void gfx_load(void (*init)(), void (*update)(), void (*close)());
void gfx_mainloop();

int gfx_get_key(int i);
int gfx_get_keydown(int i);
int gfx_get_keyup(int i);

void gfx_load_wav(char *path, int index);
void gfx_play_wav(int index);

int gfx_read_map();
int gfx_write_map();
int gfx_map_data(int x, int y);
int gfx_map_flags(int x, int y);
void gfx_set_map(int x, int y, unsigned int value);
void gfx_set_map_flags(int x, int y, int value);

void gfx_clear();
void gfx_set_layer(int layer);
void gfx_set_color(int i);
void gfx_set_key(int i);
void gfx_get_cam(int *x, int *y);
void gfx_set_cam(int x, int y);
void gfx_draw_rect(int x, int y, int w, int h);
void gfx_draw_rect_fill(int x, int y, int w, int h);
void gfx_draw_line(int x1, int y1, int x2, int y2);
void gfx_draw_point(int x, int y);
void gfx_draw_circ(int x0, int y0, int radius);
void gfx_draw_circ_fill(int x0, int y0, int radius);
void gfx_draw_text_at(int x, int y, char *text);
void gfx_draw_text(char *text);
void gfx_draw_char(int index, int x, int y);
void gfx_draw_sprite(int index, int x, int y, float r, int flags);
void gfx_draw_map();

#endif
