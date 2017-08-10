#version 440

in vec2 uv;
out vec4 out_color;

layout (location = 6) uniform vec4 cfilter;
layout (location = 5) uniform sampler2D tex;

void main()
{
    out_color = texture2D(tex, uv) * cfilter;
}