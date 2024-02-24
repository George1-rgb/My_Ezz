#version 460

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texcoord;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texcoord;
void main()
{           
    mat4 mv_matrix = u_viewMatrix * u_modelMatrix;
    gl_Position = u_projectionMatrix * mv_matrix  * vec4(vertex_position, 1.f);
    v_texcoord = vertex_texcoord;
    v_normal = normalize(vec4(mv_matrix * vec4(vertex_normal, 0.0)).xyz);
    v_position = vec4(mv_matrix * vec4(vertex_position, 1.0f)).xyz;
}