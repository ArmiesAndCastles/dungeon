#include <sdl2\sdl.h>

#define VIEW_SIZE_Y 900
#define VIEW_SIZE_X 1600
#define VIEW_TILE_SIZE_Y 30
#define VIEW_TILE_SIZE_X 30

void createView(int sizeY, int sizeX);
void destroyView();
void viewWait();
void clearView();
void viewAdd(char symbol, int y, int x);
void drawView();