#version 460 core
layout(location=2) in vec2 pos;
layout(location=3) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

uniform int zIndex;

out vec2 fragUV;

void main() {
  gl_Position = projection * view * transform * vec4(pos, zIndex - 255, 1);
  //gl_Position.z = 1 - (float(zIndex) / 125.25f);
  fragUV = uv;
}