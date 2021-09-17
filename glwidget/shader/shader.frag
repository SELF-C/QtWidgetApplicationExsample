#version 110 core

varying vec3 qt_Color;

void main(void)
{
    //gl_FragColor = gl_Color;
    gl_FragColor = vec4( qt_Color, 1.0 );
}
