#include <memory>
class Disk;
using DiskPtr = std::shared_ptr<Disk>; 

#ifndef DISK_H
#define DISK_H

#include "shape.h"

class Disk : public Shape {
  unsigned int m_vao;
  unsigned int m_ebo;
  unsigned int num_points;
protected:
  Disk (int num_points);
public:
  static DiskPtr Make (int num_points=64);
  virtual ~Disk ();
  virtual void Draw (StatePtr st);
};
#endif