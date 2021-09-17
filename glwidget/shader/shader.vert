#version 110 core

attribute vec3 VertexPosition;
uniform vec3 Color;
uniform mat4 MVP;

varying vec3 qt_Color;

void main(void)
{
    qt_Color = Color;
    gl_Position =  MVP * vec4(VertexPosition, 1.0);
}
