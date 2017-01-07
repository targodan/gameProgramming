#version 330
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require

layout (location = 0) in vec3 position;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

out vec4 vertexColor;

void main() {
    gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0f);

    vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}
