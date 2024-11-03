#version 410 core

layout(location = 0) in vec3 position; 
layout(location = 1) in vec3 vertexColors; 

out vec3 v_vertexColors; 

uniform mat4 uTranslation; 
uniform mat4 uPerspective; 

void main()
{
	v_vertexColors = vertexColors; 

	vec4 newPosition = uPerspective * uTranslation * vec4(position, 1.0f); 
	
	gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w);
}

