#version 130

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coords;

uniform mat4 ortho; //projection(ish) matrix.
uniform mat4 transform; //transformation matrix

out vec2 uv; //tex-coords

void main()
{
    uv = texture_coords;
    gl_Position = ortho * (transform * vec4(position, 1.0));
}