#include <stdlib.h>
#include <sdl2\sdl.h>
#include <view.h>
#include <time.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

static int tableSizeY = 10;
static int tableSizeX = 30;

static SDL_Texture *viewMakeTexture(char name[]){
   return SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(name));
}

void createView(int sizeY, int sizeX){
   SDL_Init(SDL_INIT_VIDEO);

   window = SDL_CreateWindow("SDL",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             sizeX * VIEW_TILE_SIZE_X,
                             sizeY * VIEW_TILE_SIZE_Y,
                             0);

   renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
   SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
   SDL_RenderClear(renderer);
   SDL_RenderPresent(renderer);

   texture = viewMakeTexture("data\\view.bmp");
}

void destroyView(){
   SDL_DestroyTexture(texture);

   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);

   SDL_Quit();
}

void viewWait(){
   SDL_Event event;
   clock_t waitClock = clock();

   while (clock() - waitClock < CLOCKS_PER_SEC / 30){
      SDL_PollEvent(&event);

      if (event.type != SDL_KEYDOWN) continue;

      if (event.key.keysym.sym == SDLK_ESCAPE){destroyView(); exit(0);}

      if (event.key.keysym.sym == SDLK_SPACE){
         do {SDL_PollEvent(&event);} while (event.type != SDL_KEYUP);
         do {SDL_PollEvent(&event);} while (event.type != SDL_KEYDOWN);

         if (event.key.keysym.sym == SDLK_ESCAPE){destroyView(); exit(0);}

         do {SDL_PollEvent(&event);} while (event.type != SDL_KEYUP);
      }
   }
}

void clearView(){
   SDL_RenderClear(renderer);
}

void viewAdd(char symbol, int y, int x){
   SDL_Rect place = {0, 0, 0, 0};
   SDL_Rect tile = {0, 0, 0, 0};

   place.y = y * VIEW_TILE_SIZE_Y;
   place.x = x * VIEW_TILE_SIZE_X;
   place.h = VIEW_TILE_SIZE_Y;
   place.w = VIEW_TILE_SIZE_X;

   tile.y = (int) symbol / tableSizeX * VIEW_TILE_SIZE_Y;
   tile.x = (int) symbol % tableSizeX * VIEW_TILE_SIZE_X;
   tile.h = VIEW_TILE_SIZE_Y;
   tile.w = VIEW_TILE_SIZE_X;

   SDL_RenderCopy(renderer, texture, &tile, &place);
}

void drawView(){
   SDL_RenderPresent(renderer);
}