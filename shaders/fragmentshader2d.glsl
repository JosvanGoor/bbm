#version 440

out vec4 out_color;

in vec4 color;

void main()
{
    out_color = vec4(normalize(color.xyz), 1.0);
}