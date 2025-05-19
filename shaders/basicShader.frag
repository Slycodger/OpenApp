#version 460 core

layout(binding = 0, std140) uniform material {
  vec4 albedo;
  bool usesAlbedo;
};

uniform sampler2D albedoTexture;

out vec4 fragColor;

in vec2 fragUV;

void main() {
  fragColor = albedo;
  if (usesAlbedo)
    fragColor = texture(albedoTexture, fragUV);
  //fragColor = vec4(1, 0, 0, 1);
}