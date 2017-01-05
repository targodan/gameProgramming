#version 330
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require

in vec4 position;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

void main() {
    gl_Position = projectionMatrix * modelViewMatrix * position;
}
