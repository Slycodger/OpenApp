#version 460 core

out vec4 fragColor;

in vec2 fragUV;

uniform sampler2D texTarget;

void main() {
  fragColor = texture(texTarget, fragUV);
}