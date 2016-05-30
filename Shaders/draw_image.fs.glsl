#version 330

uniform sampler2D t_diffuse;
uniform bool flip_y;

in vec2 vUv;

out vec4 outputF;

void main()
{
    if (flip_y) {
        outputF = texture2D(t_diffuse, vec2(vUv.x, 1.0 - vUv.y));
    } else {
        outputF = texture2D(t_diffuse, vUv);
    }
    // outputF = vec4(vUv, 0.5, 1.0);
}
