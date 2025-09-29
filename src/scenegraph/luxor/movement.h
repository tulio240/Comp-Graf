#include <memory>
class Movement;
using MovementPtr = std::shared_ptr<Movement>; 

#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "interpolator.h"
#include "transform.h"
#include <vector>
#include <glm/glm.hpp>

class Movement {
  float m_t;     // current time
  float m_T;     // duration (period)
  std::vector<TransformPtr> m_trl_trf;
  std::vector<TransformPtr> m_rot_trf;
  std::vector<InterpolatorPtr> m_trl_interp;
  std::vector<InterpolatorPtr> m_rot_interp;
protected:
  Movement (float T);
public:
  static MovementPtr Make (float T);
  virtual ~Movement ();
  void AddTranslation (TransformPtr trf, InterpolatorPtr interp);
  void AddRotation (TransformPtr trf, InterpolatorPtr interp);
  bool Advance (float dt, bool reverse=false);  // return true when its done
};

#endif