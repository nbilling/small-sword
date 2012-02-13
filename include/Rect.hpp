#ifndef RECT_HPP
#define RECT_HPP

class Rect{
public:
  int x1;
  int x2;
  int y1;
  int y2;
    
  Rect (int x, int y, int w, int h);

  ~Rect ();
    
  int center_x ();

  int center_y ();

  bool intersect (Rect* other);
};

#endif

