#include <memory>
class Poligonal;
using PoligonalPtr = std::shared_ptr<Poligonal>;

#ifndef POLYGON_H
#define POLYGON_H

#include "shape.h"

class Poligonal : public Shape{
    unsigned int m_vao;
    unsigned int m_ebo;
protected:
    Poligonal ();

public:
    static PoligonalPtr Make ();
    virtual ~Poligonal ();
    virtual void Draw (StatePtr st) override;
};

#endif