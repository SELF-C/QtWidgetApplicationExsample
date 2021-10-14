#version 110

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;

varying vec3 LightIntensity;

struct LightInfo {
    vec4 Position;
    vec3 Ld;
};
uniform LightInfo Light;

struct MaterialInfo {
    vec3 Kd;
};
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main(void)
{
    vec3 tnorm = normalize( NormalMatrix * VertexNormal );
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition, 1.0f);
    vec3 s = normalize(vec3(Light.Position - eyeCoords));

    LightIntensity = Light.Ld * Material.Kd * max( dot( s, tnorm ), 0.0f );
    gl_Position = MVP * vec4(VertexPosition, 1.0f);
}
