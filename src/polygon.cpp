#include "polygon.h"

#include <iostream>

#ifdef _WIN32
#include <glad/glad.h>
#else
#include <OpenGL/gl3.h>
#endif

PoligonalPtr Poligonal::Make(){
    return PoligonalPtr(new Poligonal());
}

Poligonal::Poligonal(){
    float coords[] = {
        -0.7f, 0.4f, //a
        0.0f, 0.3f, //b
        0.6f, 0.6f, //c
        0.5f, -0.3f, //d
        -0.5f, -0.4f, //e     
    };

    int indexes[] = {
        0, 1, 4,
        4, 1, 3,
        3, 2, 1
    };

    unsigned char color[] = {
        255, 0, 0,
        0, 0, 255,
        0, 255, 0,
        255, 255, 0,
        0, 255, 255
    };

    //Create VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    //Create VBO
    GLuint id[2];

    glGenBuffers(2, id);
    glBindBuffer(GL_ARRAY_BUFFER, id[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE,0,0); //COORD
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,id[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(color),color,GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_UNSIGNED_BYTE,GL_TRUE,0,0);  // color
    glEnableVertexAttribArray(1);

    //Create EBO
    glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

}

Poligonal::~Poligonal () 
{
}

void Poligonal::Draw ()
{
  glBindVertexArray(m_vao);
  glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
}