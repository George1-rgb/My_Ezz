#version 460

in vec3 v_position;
out vec4 frag_color;
void main(void)
{
    float depth = v_position.z / 1.0;
    depth = depth * 0.5 + 0.5;

    float v1 = depth * 255.0;
    float f1 = fract(v1);
    float vn1 = floor(v1) / 255.0;

    float v2 = f1 * 255.0;
    float f2 = fract(v2);
    float vn2 = floor(v2) / 255.0;

    float v3 = f2 * 255.0;
    float f3 = fract(v3);
    float vn3 = floor(v3) / 255.0;

    frag_color = vec4(vn1, vn2, vn3, f3);

}

