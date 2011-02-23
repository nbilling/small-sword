class Rect{
public:
  int x1;
  int x2;
  int y1;
  int y2;
    
  Rect (int x, int y, int w, int h) {
    x1 = x;
    y1 = y;
    x2 = x + w;
    y2 = y + h;
  }

  ~Rect () {}
    
  int center_x () {
    return ((x1 + x2) / 2);
  }

  int center_y () {
    return ((y1 + y2) / 2);
  }

  bool intersect (Rect* other) {
        return (x1 <= other->x2 
                && x2 >= other->x1 
                && y1 <= other->y2 
                && y2 >= other->y1);
  }
};

