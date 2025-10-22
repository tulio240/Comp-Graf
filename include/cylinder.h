#include <memory>
class Cylinder;
using CylinderPtr = std::shared_ptr<Cylinder>; 

#ifndef CYLINDER_H
#define CYLINDER_H

#include "disk3d.h"
#include "shape.h"

class Cylinder : public Shape {
  unsigned int m_vao;
  unsigned int m_nind; // number of incident vertices
  DiskPtr3D m_topdisk;
  DiskPtr3D m_botdisk;
protected:
  Cylinder (int nstack, int nslice);
public:
  static CylinderPtr Make (int nstack=64, int nslice=64);
  virtual ~Cylinder ();
  virtual void Draw (StatePtr st);
};
#endif
