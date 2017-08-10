#version 130

in vec3 position;
in vec2 texture_coords;

uniform mat4 ortho; //projection(ish) matrix.
uniform mat4 view; //view matrix
uniform mat4 transform; //transformation matrix

out vec2 uv; //tex-coords

void main()
{
    uv = texture_coords;
    gl_Position = ortho * (view * (transform * vec4(position, 1.0)));
}