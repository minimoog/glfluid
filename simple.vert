#version 150

in vec3 vertex;
in vec3 normal;
out vec3 normalOut;

uniform mat4 worldMatrix;
uniform mat4 projectionMatrix;

void main()
{
   gl_Position = /* projectionMatrix * worldMatrix */ vec4(vertex, 1.0);
   normalOut = (worldMatrix * vec4(normal, 0.0)).xyz;
}
