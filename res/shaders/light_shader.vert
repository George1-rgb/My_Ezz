#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coord;


uniform mat4 mvpMatrix;


void main() 
{
    gl_Position = mvpMatrix * vec4(vertex_position*0.1f, 1.0);
}