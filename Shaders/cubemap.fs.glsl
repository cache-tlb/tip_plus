#version 330

in float depth;
in vec3 N;
in vec2 vUv;
in vec3 pos;

out vec4 outputF;

void main()
{
    outputF = vec4(0.5 + pos.x/2.0, 0.5 + pos.y/2.0, 0.5 + pos.z/2.0, 1.0);
}
