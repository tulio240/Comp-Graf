#version 410

in Data {
    vec4 color;
} v;

out vec4 fcolor;

void main (void)
{
  fcolor = v.color;
}
