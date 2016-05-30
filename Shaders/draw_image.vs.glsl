#version 330

in vec4 position;
in vec2 uv;

out vec2 vUv;

void main()
{
    gl_Position = position;
    vUv = uv;
}
