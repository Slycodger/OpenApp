#version 460 core
layout(location=2) in vec2 pos;
layout(location=3) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

out vec2 fragUV;

void main() {
  gl_Position = projection * view * transform * vec4(pos, -4, 1);
  fragUV = uv;
}