#include "disk3d.h"

#include <iostream>
#include <cmath>
#include <vector>

#ifdef _WIN32
#include <glad/glad.h>
#else
#include <OpenGL/gl3.h>
#endif
# define M_PI           3.14159265358979323846  /* pi */

DiskPtr3D Disk3D::Make(int num_points, char plane, float height){
    return DiskPtr3D(new Disk3D(num_points, plane, height));
}

Disk3D::Disk3D(int num_points, char plane, float height){
    this->num_points = num_points;

    std::vector<float> coords;
    std::vector<int> indexes;

    float radius = 1.0f;

    //Colocando a origem no vetor

    float x0 = 0.0f;
    float y0 = 0.0f;

    coords.emplace_back(x0);
    coords.emplace_back(y0);


    for (int i = 0; i < num_points; i++){
        double teta = 2*M_PI*i/num_points;

        double xi = x0 + radius * std::cos(teta);
        float yi = y0 + radius * std::sin(teta);

        coords.push_back(xi);
        coords.push_back(yi);

        int next = i + 2;

        if (next > num_points){
            next = 1;
        }

        indexes.push_back(0);
        indexes.push_back(i + 1);
        indexes.push_back(next);            
    }

    //Create VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    //Create VBO
    GLuint id[1];

    glGenBuffers(2, id);
    glBindBuffer(GL_ARRAY_BUFFER, id[0]);
    glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), coords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE,0,0); //COORD
    glEnableVertexAttribArray(0);

    //Create EBO
    glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(int), indexes.data(), GL_STATIC_DRAW);

}

Disk3D::~Disk3D () 
{
}

void Disk3D::Draw (StatePtr )
{
  glBindVertexArray(m_vao);
  glDrawElements(GL_TRIANGLES, this->num_points * 3, GL_UNSIGNED_INT, 0);
}