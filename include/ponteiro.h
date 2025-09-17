#include <memory>
class Ponteiro;
using PonteiroPtr = std::shared_ptr<Ponteiro>;

#ifndef PONTEIRO_H
#define PONTEIRO_H

#include "shape.h"

class Ponteiro : public Shape{
    unsigned int m_vao;
    unsigned int m_ebo;

    unsigned int color;
protected:
    Ponteiro (int num_points);

public:
    static PonteiroPtr Make (int num_points);
    virtual ~Ponteiro ();
    virtual void Draw ();
};

#endif