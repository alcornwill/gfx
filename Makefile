

OBJS = src/vec.c src/gfx.c
MAPEDIT_OBJ = $(OBJS) src/mapedit.c
DEMO_OBJ = $(OBJS) src/demo.c
CC = gcc
INCLUDE_PATHS = -Iinclude\SDL2 -Iinclude -Isrc
LIBRARY_PATHS = -Llib
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -g
# -Wl,-subsystem,windows gets rid of the console window
#COMPILER_FLAGS += -Wl,-subsystem,windows
COMPILER_FLAGS += -Dmain=SDL_main
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
MAPEDIT_OBJ_NAME = bin/mapedit
DEMO_OBJ_NAME = bin/demo

all : mapedit demo

mapedit : $(MAPEDIT_OBJ)
	$(CC) $(MAPEDIT_OBJ) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(MAPEDIT_OBJ_NAME)
    
demo : $(DEMO_OBJ)
	$(CC) $(BOIDS_OBJ) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(DEMO_OBJ_NAME)
    