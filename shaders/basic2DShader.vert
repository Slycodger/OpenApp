#version 460 core
layout(location=2) in vec2 pos;
layout(location=3) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

uniform int zIndex;

uniform float p;

out vec2 fragUV;

void main() {
  mat4 t;
  t[0] = vec4(1, 0, 0, 0);
  t[1] = vec4(0, 1, 0, 0);
  t[2] = vec4(0, 0, 1, 0);
  t[3] = vec4(p, 0, 0, 1);
  gl_Position = projection * view * transform * vec4(pos, zIndex, 1);
  //gl_Position.z = 1 - (float(zIndex) / 125.25f);
  fragUV = uv;
}