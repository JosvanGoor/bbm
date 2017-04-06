#version 440

in vec2 uv;
out vec4 out_color;

layout (location = 6) uniform vec3 cfilter;
layout (location = 5) uniform sampler2D tex;

void main()
{
    out_color = texture2D(tex, uv) * vec4(cfilter, 1);
}