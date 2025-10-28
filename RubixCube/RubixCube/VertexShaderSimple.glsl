#version 330

uniform mat4 transformation; // uniform variables act as constants, at least for the duration of a draw call

layout(location = 0) in vec3 position;

void main()
{
	gl_Position = transformation * vec4(position, 1.0f); // Ausgabeposition von Vertex
}