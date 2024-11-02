#version 410 core

layout(location = 0) in vec3 position; 
layout(location = 1) in vec3 vertexColors; 

out vec3 v_vertexColors; 

uniform mat4 uModelMatrix; 

void main()
{
	v_vertexColors = vertexColors; 

	vec4 traslatedPosition = uModelMatrix * vec4(position, 1.0f); 
	
	gl_Position = vec4(traslatedPosition.x, traslatedPosition.y, traslatedPosition.z, 1.0f);
}

