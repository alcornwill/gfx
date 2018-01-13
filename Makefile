

OBJS = src/vec.c src/gfx.c
MAPEDIT_OBJ = $(OBJS) src/mapedit.c
GFXMODULE_OBJ = $(OBJS) src/gfxmodule.c
CC = gcc
INCLUDE_PATHS = -Iinclude\SDL2 -Iinclude -Isrc
LIBRARY_PATHS = -Llib
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -g
# -Wl,-subsystem,windows gets rid of the console window
#COMPILER_FLAGS += -Wl,-subsystem,windows
COMPILER_FLAGS += -Dmain=SDL_main
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
MAPEDIT_OBJ_NAME = bin/mapedit.exe
GFXMODULE_OBJ_NAME = bin/gfx.pyd
PYTHON_PATH = C:\\Python27
PYTHON = python27

all : gfx mapedit

gfx : $(GFXMODULE_OBJ)
	$(CC) -c $(GFXMODULE_OBJ) $(INCLUDE_PATHS) -I$(PYTHON_PATH)\include $(COMPILER_FLAGS) 
	$(CC) -shared vec.o gfx.o gfxmodule.o $(LIBRARY_PATHS) -L$(PYTHON_PATH)\libs $(LINKER_FLAGS) -l$(PYTHON) -o $(GFXMODULE_OBJ_NAME)

mapedit : $(MAPEDIT_OBJ)
	$(CC) $(MAPEDIT_OBJ) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(MAPEDIT_OBJ_NAME)
    
clean: .
	rm *.o