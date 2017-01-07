#version 330
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require

in vec4 vertexColor;

out vec4 color;

void main() {
    color = vertexColor;
}
