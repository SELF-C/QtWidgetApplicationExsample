#version 110

varying vec3 LightIntensity;

void main(void)
{
    gl_FragColor = vec4(LightIntensity, 1.0f);
    //gl_FragColor = vec4(1,1,1, 1.0f);
}
