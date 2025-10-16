#include <memory>
class Cilinder;
using CilinderPtr = std::shared_ptr<Cilinder>; 

#ifndef CILINDER_H
#define CILINDER_H

#include "shape.h"

class Cilinder : public Shape {
  unsigned int m_vao;
  unsigned int m_nind; // number of incident vertices
protected:
  Cilinder (int nstack, int nslice);
public:
  static CilinderPtr Make (int nstack=64, int nslice=64);
  virtual ~Cilinder ();
  virtual void Draw (StatePtr st);
};
#endif
