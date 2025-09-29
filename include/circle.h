#include <memory>
class Circle;
using CirclePtr = std::shared_ptr<Circle>;

#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape{
    unsigned int m_vao;
    unsigned int m_ebo;
    unsigned int num_points;
protected:
    Circle (int num_points);

public:
    static CirclePtr Make (int num_points);
    virtual ~Circle ();
    virtual void Draw (StatePtr st = {});
};

#endif