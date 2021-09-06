#include <stdlib.h>
#include <time.h>

#include <view.h>

#include <area.c>
#include <cave.c>
#include <read.c>

int main(int argc, char *argv[]){
   Size view = makeSize(0, 0);
   Size draw = makeSize(0, 0);
   Size tile = makeSize(VIEW_TILE_SIZE_Y, VIEW_TILE_SIZE_X);
   Size full = makeSize(VIEW_SIZE_Y, VIEW_SIZE_X);
   Size border = makeSize(MAP_ROOM_SIZE_Y, MAP_ROOM_SIZE_X);
   Map map = makeMap();

   readSettings(&view, &draw, tile, full, border);

   resizeMap(&map, view, draw);

   createView(view.y, view.x);

   srand(time(0));

   while (1){
      
      clearMap(&map);
      
      mapAddRooms(&map);
      
   }

   destroyView();

   return 0;
}
