#include <memory>
class Shape;
using ShapePtr = std::shared_ptr<Shape>; 

#ifndef SHAPE_H
#define SHAPE_H

class Shape {
protected:
  Shape () {}
public:
  virtual ~Shape () {}
  virtual void Draw () = 0;
};

#endif
