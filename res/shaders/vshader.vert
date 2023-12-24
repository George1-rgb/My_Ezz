#version 460

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texcoord;
layout(location = 3) in vec3 vertex_tangent;
layout(location = 4) in vec3 vertex_bitangent;
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

uniform mat4 u_projectionLightMatrix;
uniform mat4 u_shadowLightMatrix;
uniform mat4 u_lightMatrix;

uniform vec4 u_lightDirection;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texcoord;
out mat3 v_tbnMatrix;
out vec4 v_lightDirection;
out vec4 v_positionLightMatrix;

mat3 transpose(in mat3 inMatrix)
{
    vec3 i0 = inMatrix[0];
    vec3 i1 = inMatrix[1];
    vec3 i2 = inMatrix[2];

    mat3 outMatrix = mat3(
                vec3(i0.x, i1.x, i2.x),
                vec3(i0.y, i1.y, i2.y),
                vec3(i0.z, i1.z, i2.z)
                );
    return outMatrix;
}

void main()
{           
    mat4 mv_matrix = u_viewMatrix * u_modelMatrix;
    gl_Position = u_projectionMatrix * mv_matrix  * vec4(vertex_position, 1.f);
    v_texcoord = vertex_texcoord;
    v_normal = normalize(vec4(mv_matrix * vec4(vertex_normal, 0.0)).xyz);
    v_position = vec4(mv_matrix * vec4(vertex_position, 1.0f)).xyz;

    vec3 tangent = normalize(vec4(mv_matrix * vec4(vertex_tangent, 0.0)).xyz);
    vec3 bitangent = normalize(vec4(mv_matrix * vec4(vertex_bitangent, 0.0)).xyz);
    vec3 normal = normalize(vec4(mv_matrix * vec4(vertex_normal, 0.0)).xyz);
    v_tbnMatrix = transpose(mat3(tangent, bitangent, normal));

    v_positionLightMatrix = u_projectionLightMatrix * u_shadowLightMatrix * u_modelMatrix * vec4(vertex_position, 1.f);

    v_lightDirection = u_viewMatrix * u_lightMatrix * u_lightDirection;
}
