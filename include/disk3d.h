#include <memory>
class Disk3D;
using DiskPtr3D = std::shared_ptr<Disk3D>; 

#ifndef DISK3D_H
#define DISK3D_H

#include "shape.h"

class Disk3D : public Shape {
  unsigned int m_vao;
  unsigned int m_ebo;
  unsigned int num_points;
protected:
  Disk3D (int num_points, char plane, float height);
public:
  static DiskPtr3D Make (int num_points=64, char plane = 'z', float height = 0.0);
  virtual ~Disk3D ();
  virtual void Draw (StatePtr st = {});
};
#endif