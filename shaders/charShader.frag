#version 460 core

out vec4 fragColor;

in vec2 fragUV;

uniform sampler2D charTexture;
uniform vec4 color;

void main() {
  fragColor = color * texture(charTexture, fragUV).x;
}