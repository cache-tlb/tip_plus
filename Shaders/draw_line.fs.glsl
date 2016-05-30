#version 330

uniform vec3 line_color;

out vec4 outputF;

void main()
{
    outputF = vec4(line_color, 1.0);
}
