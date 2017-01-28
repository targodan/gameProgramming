#version 330
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinate;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 vertexColor;
out vec2 texCoord;

void main() {
    gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0f);
    vertexColor = normal;
    texCoord = textureCoordinate;
}
