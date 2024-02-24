#version 460

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

layout(location = 0) in vec3 vertex_position;

void main()
{           
     mat4 mv_matrix = u_viewMatrix * u_modelMatrix;
    gl_Position = u_projectionMatrix * mv_matrix  * vec4(vertex_position, 1.f);
}