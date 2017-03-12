#version 440

layout(location = 0) in vec3 position;

out vec4 color;

void main()
{
    color = vec4(position, 1.0);
    gl_Position = vec4(position, 1.0);
}