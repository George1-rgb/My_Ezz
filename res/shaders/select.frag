#version 460

uniform float u_code;

out vec4 frag_color;

void main()
{
    frag_color = vec4(u_code, 0, 0, 0);
}