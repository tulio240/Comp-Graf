#include "circle.h"

#include <iostream>
#include <cmath>
#include <vector>

#ifdef _WIN32
#include <glad/glad.h>
#else
#include <OpenGL/gl3.h>
#endif

CirclePtr Circle::Make(){
    return CirclePtr(new Circle());
}

Circle::Circle(){
    int num_points = 40;
    
    std::vector<float> coords(80);
    std::vector<int> indexes(120);

    float radius = 0.7f;

    //Colocando a origem no vetor

    float x0 = 0.0f;
    float y0 = 0.0f;

    coords.emplace_back(x0);
    coords.emplace_back(y0);


    for (int i = 0; i < 40; i++){
        float teta = 2*3.14*i/num_points;

        std::cout << "Teta" << teta << std::endl;

        float xi = x0 + radius * std::cos(teta);
        float yi = y0 + radius * std::sin(teta);

        std::cout << "xi" << teta << std::endl;
        std::cout << "yi" << teta << std::endl << '\n';

        coords.emplace_back(xi);
        coords.emplace_back(yi);

        indexes.emplace_back(0);
        indexes.emplace_back(i * 2);
        indexes.emplace_back(i * 2 + 1);
    }

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
    glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), coords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE,0,0); //COORD
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,id[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(color),color,GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_UNSIGNED_BYTE,GL_TRUE,0,0);  // color
    glEnableVertexAttribArray(1);

    //Create EBO
    glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(int), indexes.data(), GL_STATIC_DRAW);

}

Circle::~Circle () 
{
}

void Circle::Draw ()
{
  glBindVertexArray(m_vao);
  glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_INT, 0);
}