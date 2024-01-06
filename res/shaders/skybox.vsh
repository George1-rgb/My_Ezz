#version 460
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texcoord;
layout(location = 2) in vec3 v_normal;
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;
out vec2 v_Texcoord;

void main()
{
    mat4 viewMatix = u_viewMatrix;
    viewMatix[3][0] = 0;
    viewMatix[3][1] = 0;
    viewMatix[3][2] = 0;
    mat4 mv_matrix = viewMatix * u_modelMatrix;
    gl_Position = u_projectionMatrix * mv_matrix * vec4(v_position, 1.0);
    v_Texcoord = v_texcoord;

}

