#version 410

layout (location=0) in vec4 vertex;
uniform vec4 icolor;

out Data{
  vec4 color;
} v;

uniform mat4 M;

void main (void)
{
  v.color = icolor;
  gl_Position = M * vertex;
}

