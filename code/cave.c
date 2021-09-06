#define MAP_SIZE_Y 50
#define MAP_SIZE_X 100
#define MAP_ROOM_SIZE_Y 2
#define MAP_ROOM_SIZE_X 4

struct Map {
   Area full;
   Area view;
   Area cave;
   int tiles[MAP_SIZE_Y][MAP_SIZE_X];
   Place stair;
};
typedef struct Map Map;

typedef int (*TileCondition)(Map*, Place);

Size mapSize(Map *map){
   return areaSize(map->view);
}

Area mapFullArea(Map *map){
   return map->full;
}

Area mapViewArea(Map *map){
   return map->view;
}

Area mapDrawArea(Map *map){
   return map->cave;
}

char mapAt(Map *map, Place place){
   return placeWithin(place, mapViewArea(map)) ?
          map->tiles[place.y][place.x] : '?';
}

Place mapBegin(){
   return makePlace(0, -1);
}

int mapNext(Map *map, Place *place){
   if (place->x++ < mapSize(map).x) return 1;

   place->x = 0;

   return place->y++ < mapSize(map).y;
}

void drawMap(Map *map){
   Place place = mapBegin();

   clearView();

   while (mapNext(map, &place))
      viewAdd(mapAt(map, place), place.y, place.x);

   drawView();
}

int mapCornerAt(Map *map, Place place){
   return mapAt(map, makePlace(place.y - 1, place.x)) == '#' &&
          mapAt(map, makePlace(place.y, place.x - 1)) == '#' &&
          mapAt(map, makePlace(place.y - 1, place.x - 1)) == '.' ||

          mapAt(map, makePlace(place.y - 1, place.x)) == '#' &&
          mapAt(map, makePlace(place.y, place.x + 1)) == '#' &&
          mapAt(map, makePlace(place.y - 1, place.x + 1)) == '.' ||

          mapAt(map, makePlace(place.y + 1, place.x)) == '#' &&
          mapAt(map, makePlace(place.y, place.x - 1)) == '#' &&
          mapAt(map, makePlace(place.y + 1, place.x - 1)) == '.' ||

          mapAt(map, makePlace(place.y + 1, place.x)) == '#' &&
          mapAt(map, makePlace(place.y, place.x + 1)) == '#' &&
          mapAt(map, makePlace(place.y + 1, place.x + 1)) == '.';
}

int mapInternalAt(Map *map, Place place){
   return mapAt(map, makePlace(place.y - 1, place.x)) != ' ' &&
          mapAt(map, makePlace(place.y + 1, place.x)) != ' ' &&
          mapAt(map, makePlace(place.y, place.x - 1)) != ' ' &&
          mapAt(map, makePlace(place.y, place.x + 1)) != ' ';
}

int mapEntranceAt(Map *map, Place at){
   Place place = mapBegin();

   if (mapAt(map, at) != '$' && mapAt(map, at) != '<') return 0;

   while (mapNext(map, &place))
      if (placeNear(at, place) && mapAt(map, place) == '*')
         return 1;

   return 0;
}

int mapAdd(Map *map, char tile, Place place){
   if (!placeWithin(place, mapViewArea(map))) return 0;

   map->tiles[place.y][place.x] = tile;

   return 1;
}

void mapFillArea(Map *map, Area area, char tile){
   Place place = mapBegin();

   while (mapNext(map, &place))
      if (placeWithin(place, area)) mapAdd(map, tile, place);
}

void clearMap(Map *map){
   mapFillArea(map, map->full, ' ');
   mapFillArea(map, map->view, '+');
   mapFillArea(map, map->cave, ' ');
}

void resizeMap(Map *map, Size viewSize, Size caveSize){
   map->view = makeArea(makePlace(0, 0), makePlace(viewSize.y, viewSize.x));

   if (!areaWithin(map->view, map->full)) map->view = map->full;

   map->cave = makeArea(makePlace(0, 0), makePlace(caveSize.y, caveSize.x));

   if (!areaWithin(map->cave, map->view)) map->cave = map->view;

   map->cave = centerArea(map->cave, map->view);

   map->stair = makePlace(mapSize(map).y / 2, mapSize(map).x / 2);

   clearMap(map);
}

Map makeMap(){
   Map map;

   map.full = makeArea(makePlace(0, 0), makePlace(MAP_SIZE_Y, MAP_SIZE_X));

   map.view = map.full;

   map.cave = map.full;

   clearMap(&map);

   return map;
}

Place mapFindIf(Map *map, TileCondition condition){
   Place place = mapBegin();

   while (mapNext(map, &place))
      if (condition(map, place)) return place;

   return noPlace();
}

Place mapRandomPlace(Map *map){
   return makePlace(rand() % mapSize(map).y, rand() % mapSize(map).x);
}

int mapTileExists(Map *map, char tile){
   Place place = mapBegin();

   while (mapNext(map, &place))
      if (mapAt(map, place) == tile) return 1;

   return 0;
}

Place mapFindRandom(Map *map, char tile){
   Place place = makePlace(0, 0);

   if (!mapTileExists(map, tile)) return noPlace();

   while (mapAt(map, place = mapRandomPlace(map)) != tile);

   return place;
}

int mapIsFull(Map *map){
   Place place = mapBegin();

   while (mapNext(map, &place))
      if (mapAt(map, place) == ' ') return 0;

   return 1;
}

void mapReplaceIf(Map *map, char tile, TileCondition condition){
   Place place = mapBegin();

   while (mapNext(map, &place))
      if (condition(map, place)) mapAdd(map, tile, place);
}

void mapReplaceAll(Map *map, int tile_1, int tile_2){
   Place place = mapBegin();

   while (mapNext(map, &place))
      if (mapAt(map, place) == tile_1) mapAdd(map, tile_2, place);
}

void mapSurroundAt(Map *map, Place at, char tile){
   Place place = mapBegin();

   while (mapNext(map, &place))
      if (placeNear(at, place) && mapAt(map, place) == ' ')
         mapAdd(map, tile, place);
}

void mapSurroundAll(Map *map, char tile_1, char tile_2){
   Place place = mapBegin();

   while (mapNext(map, &place))
      if (mapAt(map, place) == tile_1) mapSurroundAt(map, place, tile_2);
}

int mapAddDoor(Map *map, Place place){
   if (!placeWithin(place, mapViewArea(map))) return 0;

   if (mapAt(map, place) != '#') return 0;

   mapAdd(map, '$', place);

   mapSurroundAt(map, place, '*');

   drawMap(map);
   viewWait();

   return 1;
}

void mapAddStairUp(Map *map){
   mapAdd(map, '<', map->stair);

   mapSurroundAt(map, map->stair, '*');

   map->stair = makePlace(0, 0);

   drawMap(map);
   viewWait();
}

int mapAddStairDown(Map *map, Place place){
   int waitCount = 20;

   if (!placeWithin(place, mapViewArea(map))) return 0;

   if (mapAt(map, place) != '.') return 0;

   mapAdd(map, '>', place);

   map->stair = place;

   drawMap(map);
   while (waitCount--) viewWait();

   return 1;
}

int mapAddRoom(Map *map, int size){
   Place place = mapFindIf(map, (TileCondition) mapEntranceAt);

   if (!placeIsSet(&place)) return 0;

   while (size--){

      mapReplaceAll(map, '*', ',');

      mapSurroundAll(map, ',', '*');

      mapReplaceAll(map, ',', '.');

      mapReplaceIf(map, '#', (TileCondition) mapCornerAt);

      drawMap(map);
      viewWait();

   }

   viewWait();

   mapReplaceAll(map, '*', '#');

   drawMap(map);
   viewWait();

   return 1;
}

void mapAddRooms(Map *map){
   Place place = makePlace(0, 0);

   mapAddStairUp(map);

   mapAddRoom(map, 3);

   while (!mapIsFull(map)){

      do {
         place = mapFindRandom(map, '#');
      } while (mapCornerAt(map, place) || mapInternalAt(map, place));

      mapAddDoor(map, place);

      mapAddRoom(map, 4);

   }

   place = mapFindRandom(map, '.');

   mapAddStairDown(map, place);
}