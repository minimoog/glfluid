attribute vec3 vertex;
attribute vec3 normal;
varying vec3 normalOut;

uniform mat4 worldMatrix;
uniform mat4 projectionMatrix;

void main()
{
   gl_Position = projectionMatrix * worldMatrix * vec4(vertex, 1.0);
   normalOut = (worldMatrix * vec4(normal, 0.0)).xyz;
}
