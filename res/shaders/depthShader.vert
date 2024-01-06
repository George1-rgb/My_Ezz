#version 460
layout(location = 0) in vec3 vertex_position;
uniform mat4 u_projectionLightMatrix;
uniform mat4 u_shadowLightMatrix;
uniform mat4 u_modelMatrix;
out vec3 v_position;
void main()
{
    v_position = vec4(u_projectionLightMatrix * u_shadowLightMatrix * u_modelMatrix).xyz * vertex_position;
    gl_Position = vec4(v_position, 1.0);
}
