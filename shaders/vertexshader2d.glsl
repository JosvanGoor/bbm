#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coords;

uniform mat4 ortho;

out vec2 uv;
out vec4 color;

void main()
{
    uv = texture_coords;
    gl_Position = ortho * vec4(position, 1.0);
}