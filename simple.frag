varying vec3 normalOut;

uniform vec3 lightDirection;

void main()
{
   float diffuseFactor = max(dot(normalize(normalOut), -lightDirection), 0.0);
   gl_FragColor = diffuseFactor * vec4(1.0, 1.0, 1.0, 1.0);
}
