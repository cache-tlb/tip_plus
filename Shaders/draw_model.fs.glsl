#version 330
uniform sampler2D t_diffuse;

in float depth;
in vec3 N;
in vec2 vUv;

out vec4 outputF;

void main()
{
    float d = (depth)/1.0;
    outputF = texture2D(t_diffuse, vUv);
}
