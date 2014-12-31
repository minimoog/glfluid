#version 150
in vec3 normalOut;
out vec4 fragColor;

uniform vec3 lightDirection;
//precision mediump float;

void main()
{
   float diffuseFactor = max(dot(normalize(normalOut), -lightDirection), 0.0);
   fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

