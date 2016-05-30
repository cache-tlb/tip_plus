#version 330

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat4 m_view;
uniform mat4 m_model;
uniform mat3 m_normal;


in vec4 position;
in vec3 normal;
in vec2 uv;

out vec3 N;
out float depth;
out vec2 vUv;

void main()
{
    N = normalize(m_normal * normal);
    gl_Position = m_pvm * position;
//    gl_Position.y = -gl_Position.y;
    depth = gl_Position.z;
    vUv = uv;
}
