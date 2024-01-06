#version 460
uniform sampler2D u_diffuseMap;
in vec2 v_Texcoord;

out vec4 frag_color;
void main()
{

    frag_color = texture2D(u_diffuseMap, v_Texcoord);

}
