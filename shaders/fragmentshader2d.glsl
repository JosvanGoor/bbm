#version 440

out vec4 out_color;

in vec2 uv;
uniform sampler2D tex;

void main()
{
    vec4 col = texture2D(tex, uv);
    if(col.xyz != vec3(0.0, 0.0, 0.0)) out_color = col;
    else out_color = vec4(1.0, 0.0, 0.0, 1.0);
}