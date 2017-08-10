#version 130

in vec2 uv;
out vec4 out_color;

uniform vec4 cfilter;
uniform sampler2D tex;

void main()
{
    out_color = texture2D(tex, uv) * cfilter;
}