#include <stdio.h>
#include <ctype.h>

int readNextNumber(FILE *settings){
   int x = 0;
   char value[6] = "\0\0\0\0\0";

   for (x = 0; x < 5; x++){
      value[x] = getc(settings);

      if (value[x] == EOF) break;
      if (value[x] == ' ') break;

      if (!isdigit(value[x])){
         ungetc(value[x], settings);
         return 0;
      }
   }

   return atoi(value);
}

void readSettings(Size *view, Size *draw, Size tile, Size full, Size border){
   FILE *settings = fopen("view.txt", "r");

   if (!(view->y = readNextNumber(settings))) view->y = full.y;
   if (!(view->x = readNextNumber(settings))) view->x = full.x;

   *view = limitSize(*view, border, full);

   if (view->y % tile.y) view->y += tile.y - (view->y % tile.y);
   if (view->x % tile.x) view->x += tile.x - (view->x % tile.x);

   view->y /= tile.y;
   view->x /= tile.x;

   if (!(draw->y = readNextNumber(settings))) draw->y = view->y - border.y;
   if (!(draw->x = readNextNumber(settings))) draw->x = view->x - border.x;

   *draw = limitSize(*draw, border, *view);

   fclose(settings);
}