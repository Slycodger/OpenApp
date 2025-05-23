#version 460 core

out vec4 fragColor;

in vec2 fragUV;

uniform sampler2D charTexture;
uniform vec4 color;
uniform float cutoff;

void main() {
  if (texture(charTexture, fragUV).x > cutoff)
    fragColor = color;
  else
    fragColor = vec4(0);
}