#include <memory>
#include <glm/glm.hpp>

class Ponteiro;
using PonteiroPtr = std::shared_ptr<Ponteiro>;

#ifndef PONTEIRO_H
#define PONTEIRO_H

#include "shape.h"

class Ponteiro : public Shape{
    unsigned int m_vao;
    unsigned int m_ebo;

    glm::vec4 color;
    glm::mat4 scale;
    glm::mat4 rotation;

protected:
    Ponteiro ();

public:
    static PonteiroPtr Make ();
    virtual ~Ponteiro ();
    virtual void Draw (StatePtr st = {});
};

#endif