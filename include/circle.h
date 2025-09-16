#include <memory>
class Circle;
using CirclePtr = std::shared_ptr<Circle>;

#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape{
    unsigned int m_vao;
    unsigned int m_ebo;
protected:
    Circle ();

public:
    static CirclePtr Make ();
    virtual ~Circle ();
    virtual void Draw ();
};

#endif