#version 460

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texcoord;

uniform float u_borderWidth;
uniform float u_aspectRatio;

out vec4 frag_color;

void main()
{
frag_color = vec4(0.0);
   float maxX = 1.0 - u_borderWidth;
   float minX = u_borderWidth;
   float maxY = maxX / u_aspectRatio;
   float minY = minX / u_aspectRatio;

   if (v_texcoord.x >= maxX && v_texcoord.x <= minX &&
       v_texcoord.y <= maxY && v_texcoord.y >= minY) 
       {
         frag_color = vec4(0.5, 0.5, 0.5, 1.0f);
       }
}