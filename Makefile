

CC = gcc
INCLUDE_PATHS = -Iinclude\SDL2 -Iinclude -Isrc
LIBRARY_PATHS = -Llib
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -g
# -Wl,-subsystem,windows gets rid of the console window
#COMPILER_FLAGS += -Wl,-subsystem,windows
COMPILER_FLAGS += -Dmain=SDL_main
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
PYTHON_PATH = C:\\Python27
PYTHON = python27

all : gfx mapedit

gfx : src/gfx.c src/gfxmodule.c
	$(CC) -c src/gfx.c src/gfxmodule.c $(INCLUDE_PATHS) -I$(PYTHON_PATH)\include $(COMPILER_FLAGS) 
	$(CC) -shared gfx.o gfxmodule.o $(LIBRARY_PATHS) -L$(PYTHON_PATH)\libs $(LINKER_FLAGS) -l$(PYTHON) -o bin/gfx.pyd

mapedit : src/gfx.c src/mapedit.c
	$(CC) src/gfx.c src/mapedit.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o bin/mapedit.exe
    
clean: .
	rm *.o