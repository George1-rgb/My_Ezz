#version 460
layout (location = 0) in vec3 a_position;
uniform highp mat4 u_projectionLightMatrix;
uniform highp mat4 u_shadowLightMatrix;
uniform highp mat4 u_modelMatrix;
out highp vec3 v_position;
void main(void)
{
    v_position = vec4(u_projectionLightMatrix * u_shadowLightMatrix * u_modelMatrix).xyz * a_position;
    gl_Position = vec4(v_position, 1.0);
}
