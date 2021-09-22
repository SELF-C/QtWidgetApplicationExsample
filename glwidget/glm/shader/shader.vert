#version 110 core

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;

varying vec3 LightIntensity;

uniform vec4 LightPosition;
uniform vec3 Kd;
uniform vec3 Ld;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main(void)
{
    vec3 tnorm = normalize( NormalMatrix * VertexNormal );
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition, 1.0f);
    vec3 s = normalize(vec3(LightPosition - eyeCoords));

    LightIntensity = Ld * Kd * max( dot( s, tnorm ), 0.0f );
    gl_Position = MVP * vec4(VertexPosition, 1.0f);
}
