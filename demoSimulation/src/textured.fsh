#version 330
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require

in vec3 vertexColor;
in vec2 texCoord;

uniform sampler2D diffuseTexture1;

out vec4 color;

void main() {
    color = texture(diffuseTexture1, texCoord).rgba;
}
