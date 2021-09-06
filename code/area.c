struct Size {
   int y;
   int x;
};
typedef struct Size Size;

struct Place {
   int y;
   int x;
};
typedef struct Place Place;

struct Area {
   Place place_1;
   Place place_2;
};
typedef struct Area Area;

#define limit(X, LOW, HIGH) X < LOW ? LOW : X > HIGH ? HIGH : X

/* ############################### S I Z E ############################### */

Size makeSize(int y, int x){
   return (Size) {y, x};
}

Size sizeDivision(Size size, int value){
   return makeSize(size.y / value, size.x / value);
}

Size sizeAddition(Size size_1, Size size_2){
   return makeSize(size_1.y + size_2.y, size_1.x + size_2.x);
}

Size limitSize(Size size, Size low, Size high){
   return makeSize(limit(size.y, low.y, high.y),
                   limit(size.x, low.x, high.x));
}

/* ####################################################################### */


/* ############################## P L A C E ############################## */

Place makePlace(int y, int x){
   return (Place) {y, x};
}

Place noPlace(){
   return (Place) {0, 0};
}

int placeIsSet(Place *place){
   return place->y || place->x;
}

int placeNear(Place place_1, Place place_2){
   return abs(place_1.y - place_2.y) < 2 && abs(place_1.x - place_2.x) < 2;
}

int placeWithin(Place place, Area area){
   return place.y >= area.place_1.y && place.y < area.place_2.y &&
          place.x >= area.place_1.x && place.x < area.place_2.x;
}

Size placeDifference(Place place_1, Place place_2){
   return makeSize(place_1.y - place_2.y, place_1.x - place_2.x);
}

Place movePlace(Place place, Size size){
   return makePlace(place.y + size.y, place.x + size.x);
}

/* ####################################################################### */


/* ################################ A R E A ############################## */

Area makeArea(Place place_1, Place place_2){
   return (Area) {place_1, place_2};
}

Size areaSize(Area area){
   return placeDifference(area.place_2, area.place_1);
}

int areaWithin(Area area_1, Area area_2){
   return placeWithin(area_1.place_1, area_2) &&
          placeWithin(area_1.place_2, area_2);
}

Size areaDifference(Area area_1, Area area_2){
   return sizeAddition(placeDifference(area_1.place_1, area_2.place_1),
                       placeDifference(area_1.place_2, area_2.place_2));
}

Area moveArea(Area area_1, Size size){
   return makeArea(movePlace(area_1.place_1, size),
                   movePlace(area_1.place_2, size));
}

Area centerArea(Area area_1, Area area_2){
   return moveArea(area_1, sizeDivision(areaDifference(area_2, area_1), 2));
}

/* ####################################################################### */