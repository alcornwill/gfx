
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"


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

struct WavBuffer {
    SDL_AudioSpec spec;
    unsigned int length;
    unsigned char *buffer;    
};


static void my_audio_callback(void *userdata, unsigned char *stream, int len);


static const Vec2 g_sprite_shapes[12] = {
    {1, 1}, {2, 2}, {4, 4}, {8, 8},
    {2, 1}, {4, 1}, {4, 2}, {8, 4},
    {1, 2}, {1, 4}, {2, 4}, {4, 8}
};

static const int g_keymap[8] = {
    KM_UP,
    KM_DOWN,
    KM_LEFT,
    KM_RIGHT,
    KM_A,
    KM_B,
    KM_START,
    KM_SELECT
};

static Color g_palette[16] = {
    PAL_BLACK,  PAL_WHITE,    PAL_RED,     PAL_CYAN, 
    PAL_VIOLET, PAL_GREEN,    PAL_BLUE,    PAL_YELLOW, 
    PAL_ORANGE, PAL_BROWN,    PAL_PINK,    PAL_DK_GREY, 
    PAL_GREY,   PAL_LT_GREEN, PAL_LT_BLUE, PAL_LT_GREY
};

static SDL_AudioSpec g_spec = {
    .freq =     AUDIO_SAMPLE_RATE,
    .format =   AUDIO_FORMAT,
    .channels = AUDIO_CHANNELS,
    .samples =  AUDIO_SAMPLES,
    .callback = my_audio_callback,
    .userdata = NULL
};

static int g_quit = 0;
static int g_window_width = 512;
static int g_window_height = 512;
static SDL_Window *g_window = NULL;
static SDL_Renderer *g_renderer = NULL;
static SDL_Texture *g_spritesheet_texture = NULL; // k or nk
static SDL_Texture *g_spritesheet_texture_nk = NULL; // non color keyed...
static SDL_Texture *g_spritesheet_texture_k = NULL; // color keyed
static int g_layer = 0;
static SDL_Texture *g_layers[NUM_LAYERS] = {0};
static unsigned char g_map[MAP_LENGTH] = {0};
static int g_keys[8] = {0};
static int g_keysdown[8] = {0};
static int g_keysup[8] = {0};
static int g_frame = 0;
static float g_time = 0;
static float g_dt = 0;
static Vec2 g_cursor = {0, 0}; // text cursor
static Vec2 g_camera = {0, 0};
static int (*g_user_update)() = NULL;
static int (*g_user_close)() = NULL;
static WavBuffer g_wavbuffers[NUM_WAVBUFFER] = {0};
static unsigned char *g_audio_pos = NULL;
static unsigned int g_audio_len = 0;
static float g_volume = 1.0f;


static void my_audio_callback(void *userdata, unsigned char *stream, int len) {
    SDL_memset(stream, 0, len);

	if (g_audio_len ==0)
		return;
	
	len = len > g_audio_len ? g_audio_len : len;
	SDL_MixAudio(stream, g_audio_pos, len, SDL_MIX_MAXVOLUME * g_volume);
	
	g_audio_pos += len;
    g_audio_len -= len;
    
    // if (g_loop && g_audio_len <= 0) {
        // g_audio_len = g_wav_length;
        // g_audio_pos = g_wav_buffer;
    // }
}

void gfx_play_wav(int index) {
    WavBuffer *wav = &g_wavbuffers[index];
    g_audio_len = wav->length;
    g_audio_pos = wav->buffer;
    SDL_PauseAudio(0);
}

int gfx_get_key(int i) {
    return g_keys[i];
}

int gfx_get_keydown(int i) {
    return g_keysdown[i];
}

int gfx_get_keyup(int i) {
    return g_keysup[i];
}

void gfx_get_cam(int *x, int *y) {
    *x = g_camera.x;
    *y = g_camera.y;
}

void gfx_set_cam(int x, int y) {
    g_camera.x = x;
    g_camera.y = y;
}

void gfx_set_map(int x, int y, unsigned int value) {
    int i = (y * MAP_WIDTH) + x;
    if (i >= 0 && i < MAP_LENGTH)
        g_map[i] = value & 0xF;
}

int gfx_read_map(char *path) {
    FILE *file = NULL;
    file = fopen(path, "r");
    if (!file) {
        printf("Failed to read tilemap!\n");
        return 0;
    }
    fread(g_map, 1, MAP_LENGTH, file);
    fclose(file);
    return 1;
}

int gfx_write_map(char *path) {
    FILE *file = NULL;
    file = fopen(path, "w");
    if (!file) {
        printf(" Failed to write tilemap!\n");
        return 0;
    }
    fwrite(g_map, 1, MAP_LENGTH, file);
    fclose(file);
    return 1;
}

void gfx_set_layer(int layer) {
    SDL_SetRenderTarget( g_renderer, g_layers[layer]);
    g_layer = layer;
}

void gfx_set_color(int i) {
    Color c = g_palette[i];
    SDL_SetRenderDrawColor( g_renderer, c.r, c.g, c.b, c.a );
    // also modulate texture
    SDL_SetTextureColorMod( g_spritesheet_texture_nk, c.r, c.g, c.b);
    SDL_SetTextureColorMod( g_spritesheet_texture_k, c.r, c.g, c.b);
}

void gfx_set_key(int value) {
    // turn chroma key on/off (transparency)
    g_spritesheet_texture = value ? g_spritesheet_texture_k : g_spritesheet_texture_nk;
}

void gfx_clear() {
    SDL_RenderClear( g_renderer );
}

void gfx_draw_rect(int x, int y, int w, int h) {
    x -= g_camera.x;
    y -= g_camera.y;
    SDL_Rect rect = { x, y, w, h };		
    SDL_RenderDrawRect( g_renderer, &rect );
}

void gfx_draw_rect_fill(int x, int y, int w, int h) {
    x -= g_camera.x;
    y -= g_camera.y;
    SDL_Rect rect = { x, y, w, h };		
    SDL_RenderFillRect( g_renderer, &rect );
}

void gfx_draw_line(int x1, int y1, int x2, int y2) {
    x1 -= g_camera.x;
    y1 -= g_camera.y;
    x2 -= g_camera.x;
    y2 -= g_camera.y;
    SDL_RenderDrawLine( g_renderer, x1, y1, x2, y2);
}

void gfx_draw_point(int x, int y) {
    x -= g_camera.x;
    y -= g_camera.y;
    SDL_RenderDrawPoint( g_renderer, x, y);
}

void gfx_draw_circ(int x0, int y0, int radius)
{
    x0 -= g_camera.x;
    y0 -= g_camera.y;
    int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
        SDL_RenderDrawPoint( g_renderer, x0 + x, y0 + y);
        SDL_RenderDrawPoint( g_renderer, x0 + y, y0 + x);
        SDL_RenderDrawPoint( g_renderer, x0 - y, y0 + x);
        SDL_RenderDrawPoint( g_renderer, x0 - x, y0 + y);
        SDL_RenderDrawPoint( g_renderer, x0 - x, y0 - y);
        SDL_RenderDrawPoint( g_renderer, x0 - y, y0 - x);
        SDL_RenderDrawPoint( g_renderer, x0 + y, y0 - x);
        SDL_RenderDrawPoint( g_renderer, x0 + x, y0 - y);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void gfx_draw_circ_fill(int x0, int y0, int radius)
{
    x0 -= g_camera.x;
    y0 -= g_camera.y;
    int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
        SDL_RenderDrawLine( g_renderer, x0 + x, y0 + y, x0 - x, y0 + y);
        SDL_RenderDrawLine( g_renderer, x0 + y, y0 + x, x0 - y, y0 + x);
        SDL_RenderDrawLine( g_renderer, x0 - x, y0 - y, x0 + x, y0 - y);
        SDL_RenderDrawLine( g_renderer, x0 - y, y0 - x, x0 + y, y0 - x);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

static void draw_char(int index, int x, int y) {
    x = x - g_camera.x;
    y = y - g_camera.y;
    
    int sx = (index % SPRITE_SHEET_WIDTH) * 8;
    int sy = (index / SPRITE_SHEET_WIDTH) * 8;
    
    SDL_Rect src_rect = {sx, sy, 8, 8};
    SDL_Rect dst_rect = {x, y, 8, 8};
    
    if (SDL_RenderCopy( g_renderer, g_spritesheet_texture, &src_rect, &dst_rect) != 0) {
        printf("Could not draw char! SDL Error: %s\n", SDL_GetError());
    }
}

void gfx_draw_text_at(int x, int y, char *text) {
    g_cursor.x = x;
    g_cursor.y = y;
    
    char c = 0;
    int i = 0;
    while ((c = text[i]) != 0) {
        draw_char(FONT_INDEX + c, g_cursor.x, g_cursor.y);
        g_cursor.x += 8;
        ++i;
    }
}

void gfx_draw_text(char *text) {
    // todo interpret newlines
    char c = 0;
    int i = 0;
    while ((c = text[i]) != 0) {
        draw_char(FONT_INDEX + c, g_cursor.x, g_cursor.y);
        
        // hmm, this doesn't really work with camera
        g_cursor.x += 8;
        if (g_cursor.x >= SCREEN_WIDTH) {
            g_cursor.x %= SCREEN_WIDTH;
            g_cursor.y += 8;
            if (g_cursor.y >= SCREEN_HEIGHT) {
                g_camera.y -= 8;
            }
        }
        ++i;
    }
}

void gfx_draw_sprite(int index, int x, int y, float r, int flags) {
    x -= g_camera.x;
    y -= g_camera.y;
    
    int sx = (index % SPRITE_SHEET_WIDTH) * 8;
    int sy = (index / SPRITE_SHEET_WIDTH) * 8;
    
    int size = (flags & SIZE_FLAGS);
    int shape = (flags & SHAPE_FLAGS) >> 2;
    Vec2 dim = g_sprite_shapes[shape * 4 + size];
    
    int sclx = flags & SCALE_X ? 2 : 1;
    int scly = flags & SCALE_Y ? 2 : 1;
    
    SDL_RendererFlip flip;
    if (!(flags & FLIP_HORIZONTAL) && !(flags & FLIP_VERTICAL))
        flip = SDL_FLIP_NONE;
    else if (flags & FLIP_HORIZONTAL)
        flip = SDL_FLIP_HORIZONTAL;
    if (flags & FLIP_VERTICAL)
        flip |= SDL_FLIP_VERTICAL;
    
    SDL_Rect src_rect = {sx, sy, dim.x * 8, dim.y * 8};
    SDL_Rect dst_rect = {x, y, sclx * dim.x * 8, scly * dim.y * 8};
    if (SDL_RenderCopyEx( g_renderer, g_spritesheet_texture, &src_rect, &dst_rect, DEGREES(r), NULL, flip) != 0) {
        printf("Could not draw sprite! SDL Error: %s\n", SDL_GetError());
    }
}

int gfx_map_data(int x, int y) {
    return g_map[y * MAP_WIDTH + x] & 0x0F;
}

int gfx_map_flags(int x, int y) {
    return (g_map[y * MAP_WIDTH + x] & 0xF0) >> 4;
}

void gfx_set_map_flags(int x, int y, int value) {
    int color = gfx_map_data(x, y);
    g_map[y * MAP_WIDTH + x] = color | (value << 4);
}

static void draw_map(int x, int y, int mx, int my) {
    // draws map of fixed size 128x128 (1 screen)
    int w = 16;
    int h = 16;
    
    for (int i=0; i<w; ++i) {
        for (int j=0; j<h; ++j) {
            int sx = mx + i;
            int sy = my + j;
            if (sx < 0 || sx >= MAP_WIDTH || sy < 0 || sy >= MAP_HEIGHT) continue;
            int map_index = gfx_map_data(sx, sy) + TILESET_INDEX;
            draw_char(map_index, x + i * 8, y + j * 8);
        }
    }
}

void gfx_draw_map() {
    // draws whole map (nice if had more control...)
    int screenx = (g_camera.x / SCREEN_WIDTH) % 4;
    int screeny = g_camera.y / SCREEN_HEIGHT;
    
    // draw the 4 visible screens
    draw_map(screenx * 128, screeny * 128, screenx * 16, screeny * 16);
    draw_map((screenx+1) * 128, screeny * 128, (screenx+1) * 16, screeny * 16);
    draw_map(screenx * 128, (screeny+1) * 128, screenx * 16, (screeny+1) * 16);
    draw_map((screenx+1) * 128, (screeny+1) * 128, (screenx+1) * 16, (screeny+1) * 16);
}

static int init_sdl() {
    // init SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return 0;
	}
    
    // init audio
    if (SDL_AudioInit("directsound") < 0) 
    {
        printf("SDL audio failed to initialize! SDL Error: %s\n", SDL_GetError());
        return 0;
    }
    
    if (SDL_OpenAudio(&g_spec, NULL) < 0){
        printf("Couldn't open audio: %s\n", SDL_GetError());
        return 0;
    }
    // todo validate spec
	
    // set point sampling
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0" ))
    {
        printf("Point texture filtering not enabled! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    // create window
    g_window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g_window_width, g_window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(!g_window)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return 0;
    }
    
    // create renderer for window
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if(!g_renderer)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    // init SDL_Image
    IMG_Init(0);
    // // initialize PNG loading
    // int imgFlags = IMG_INIT_PNG;
    // if(!(IMG_Init(imgFlags) & imgFlags))
    // {
        // printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        // exit(1);
    // }
    
    return 1;
}

void gfx_load_wav(char *path, int index) {
    // load the wav file 'path' into wavbuffer index 'index'
    WavBuffer *wav = &g_wavbuffers[index];
    if (SDL_LoadWAV(path, &wav->spec, &wav->buffer, &wav->length) == NULL) {
        printf("Could not open %s: %s\n", path, SDL_GetError());
    }

    // (can't we use g_spec?)
    wav->spec.callback = my_audio_callback;
    wav->spec.userdata = NULL;
    wav->spec.samples = AUDIO_SAMPLES;
}

static int init_layers() {
    // initialize layers (render textures)
    for (int i=0; i<NUM_LAYERS; ++i) {
        g_layers[i] = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
        if (!g_layers[i]) {
            printf("Failed to initialize layer %d! SDL Error: %s\n", i, SDL_GetError());
            return 0;
        }
    }
    return 1;
}
    
int gfx_load_spritesheet(char *path)
{
    // NOTE don't call twice, doesn't free existing... (todo)
    
	SDL_Surface *surface = IMG_Load(path);
	if(!surface)
	{
		printf("Unable to load spritesheet! SDL_image Error: %s\n", IMG_GetError());
        return 0;
	}
    
    
    // hmm, even indexed image isn't loaded as indexed...
    // printf("Palette info:\n");
    // SDL_Palette *palette = surface->format->palette;
    // if (!palette) {
        // printf("No palette\n");
    // } else {
        // for (int i=0; i<palette->ncolors; ++i) {
            // SDL_Color *c = &palette->colors[i];
            // printf("%i, %i, %i, %i\n", c->r, c->g, c->b, c->a);
        // }
    // }
    
    // create texture from surface pixels
    g_spritesheet_texture_nk = SDL_CreateTextureFromSurface(g_renderer, surface);
    if(!g_spritesheet_texture_nk)
    {
        printf("Unable to create texture from spritesheet surface! SDL Error: %s\n", SDL_GetError());
        return 0;
    }
    
    // do chroma keyed version...
    // color key image 
    SDL_SetColorKey(surface, 1, SDL_MapRGB( surface->format, 0, 0, 0 )); // NOTE color key = black
    g_spritesheet_texture_k = SDL_CreateTextureFromSurface(g_renderer, surface);
    if(!g_spritesheet_texture_k)
    {
        printf("Unable to create texture from spritesheet surface! SDL Error: %s\n", SDL_GetError());
        return 0;
    }
    
    // free surface
    SDL_FreeSurface(surface);
    
    
    // init stuff
    gfx_set_color(0);
    gfx_set_key(0);
    
    return 1;
}

static void gfx_close()
{
	//Free loaded image
	SDL_DestroyTexture(g_spritesheet_texture_nk);
	g_spritesheet_texture_nk = NULL;
    SDL_DestroyTexture(g_spritesheet_texture_k);
	g_spritesheet_texture_k = NULL;
    g_spritesheet_texture = NULL;
    
    // free render textures
    for (int i=0; i<NUM_LAYERS; ++i) {
        SDL_Texture *tex = g_layers[i];
        SDL_DestroyTexture(tex);
        g_layers[i] = NULL;
    }
    
    // free wav buffers
    for (int i=0; i<NUM_WAVBUFFER; ++i) {
        WavBuffer *wav = &g_wavbuffers[i];
        SDL_FreeWAV(wav->buffer);
    }
    
	//Destroy window	
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	g_renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int gfx_load(int (*update)(), int (*close)()) {
    // initialize gfx
    g_user_update = update;
    g_user_close = close;
    
    // init SDL
    if (!init_sdl()) {
        printf("Failed to initialize!\n");
        return 0;
    }
    
    // SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");
    
    // SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    // SDL_RenderSetViewport(g_renderer, &rect);
    
    // init layers
    if (!init_layers()) {
        printf("Failed to initialze!\n");
        return 0;
    }
    
    // init stuff
    gfx_set_layer(0);
    
    return 1;
}

void gfx_quit() {
    g_quit = 1;
}

int gfx_mainloop() {
    int success = 1;

    //Event handler
    SDL_Event e;
    unsigned int frameTicks = 0;

    //While application is running
    while( !g_quit )
    {
        ++g_frame;
        g_time = SDL_GetTicks() / 1000.0f;
        int ticks = SDL_GetTicks();
        g_dt = (ticks - frameTicks) / 1000.0f;
        frameTicks = ticks;
        
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            // user requests quit
            if( e.type == SDL_QUIT ) {
                g_quit = 1;
            }
            if (e.type == SDL_WINDOWEVENT) {
                switch (e.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        g_window_width = e.window.data1;
                        g_window_height = e.window.data2;
                    break;
                }
            }
            if (e.type == SDL_KEYUP) {
                switch(e.key.keysym.scancode) {
                    case KM_UP:
                        g_keys[K_UP] = 0;
                        g_keysup[K_UP] = 1;
                        break;
                    case KM_DOWN:
                        g_keys[K_DOWN] = 0;
                        g_keysup[K_DOWN] = 1;
                        break;
                    case KM_LEFT:
                        g_keys[K_LEFT] = 0;
                        g_keysup[K_LEFT] = 1;
                        break;
                    case KM_RIGHT:
                        g_keys[K_RIGHT] = 0;
                        g_keysup[K_RIGHT] = 1;
                        break;
                    case KM_A:
                        g_keys[K_A] = 0;
                        g_keysup[K_A] = 1;
                        break;
                    case KM_B:
                        g_keys[K_B] = 0;
                        g_keysup[K_B] = 1;
                        break;
                    case KM_START:
                        g_keys[K_START] = 0;
                        g_keysup[K_START] = 1;
                        break;
                    case KM_SELECT:
                        g_keys[K_SELECT] = 0;
                        g_keysup[K_SELECT] = 1;
                        break;
                    default:
                        break;
                }
            }
            if (e.type == SDL_KEYDOWN) {
                switch( e.key.keysym.scancode ) { 
                    case SDL_SCANCODE_F11:
                        // reset window size
                        g_window_width = 512;
                        g_window_height = 512;
                        SDL_SetWindowSize(g_window, g_window_width, g_window_height);
                        break;
                    case SDL_SCANCODE_F10:
                        // write map
                        //gfx_write_map();
                        break;
                    case KM_UP:
                        g_keys[K_UP] = 1;
                        g_keysdown[K_UP] = 1;
                        break;
                    case KM_DOWN:
                        g_keys[K_DOWN] = 1;
                        g_keysdown[K_DOWN] = 1;
                        break;
                    case KM_LEFT:
                        g_keys[K_LEFT] = 1;
                        g_keysdown[K_LEFT] = 1;
                        break;
                    case KM_RIGHT:
                        g_keys[K_RIGHT] = 1;
                        g_keysdown[K_RIGHT] = 1;
                        break;
                    case KM_A:
                        g_keys[K_A] = 1;
                        g_keysdown[K_A] = 1;
                        break;
                    case KM_B:
                        g_keys[K_B] = 1;
                        g_keysdown[K_B] = 1;
                        break;
                    case KM_START:
                        g_keys[K_START] = 1;
                        g_keysdown[K_START] = 1;
                        break;
                    case KM_SELECT:
                        g_keys[K_SELECT] = 1;
                        g_keysdown[K_SELECT] = 1;
                        break;
                    default:
                        break;
                }
            }
        }

        // user update callback
        if (!g_user_update()) {
            g_quit = 1;
            success = 0;
        }
        
        SDL_SetRenderTarget(g_renderer, NULL);
        int dim = MIN(g_window_width, g_window_height);
        // would multiply dim by aspect ratio if had one
        int x = 0, y = 0;
        if (g_window_width < g_window_height) {
            // vertical letterboxes
            y = (g_window_height - g_window_width) / 2;
        }
        if (g_window_height < g_window_width) {
            // horizontal letterboxes
            x = (g_window_width - g_window_height) / 2;
        }
        SDL_Rect rect = {x, y, dim, dim};
        for (int i=0; i<NUM_LAYERS; ++i)
            SDL_RenderCopy( g_renderer, g_layers[i], NULL, &rect );

        //Update screen
        SDL_RenderPresent(g_renderer);
        
        // clear the window buffer
        gfx_set_color(0);
        gfx_clear();
        
        // restore render target
        gfx_set_layer(g_layer);
        
        // reset keysdown
        g_keysdown[0] = 0;
        g_keysdown[1] = 0;
        g_keysdown[2] = 0;
        g_keysdown[3] = 0;
        g_keysdown[4] = 0;
        g_keysdown[5] = 0;
        g_keysdown[6] = 0;
        g_keysdown[7] = 0;
        
        // reset keysup
        g_keysup[0] = 0;
        g_keysup[1] = 0;
        g_keysup[2] = 0;
        g_keysup[3] = 0;
        g_keysup[4] = 0;
        g_keysup[5] = 0;
        g_keysup[6] = 0;
        g_keysup[7] = 0;
        
        // sleep
        int end = SDL_GetTicks();
        float delay = TICKS_PER_SECOND - (end - ticks);
        if (delay > 0)
            SDL_Delay( delay );
    }

	//Free resources and close SDL
    if (!g_user_close()) 
        success = 0;
	gfx_close();
    
    return success;
}
