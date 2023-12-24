#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coord;
layout(location = 3) in vec3 vertex_tangent;
layout(location = 4) in vec3 vertex_bitangent;

uniform mat4 modelViewMatrix;
uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;
uniform int current_frame;

out vec2 tex_coord_Smile;
out vec2 tex_coord_Quads;
out vec3 frag_position_eye;
out vec3 frag_normal_eye;

void main() 
{
    tex_coord_Smile = texture_coord;
    tex_coord_Quads = texture_coord + vec2(current_frame/1000.0f, current_frame/1000.0f);
    frag_normal_eye = normalMatrix * vertex_normal;
    frag_position_eye = vec3(modelViewMatrix * vec4(vertex_position, 1.f));
    gl_Position = mvpMatrix * vec4(vertex_position, 1.f);
}