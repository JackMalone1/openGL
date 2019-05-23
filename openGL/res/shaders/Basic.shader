#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Colour;//uniform for the shader so that it can be set outside of the shgader instead of being hard coded in the shader itself

void main()
{
	color = u_Colour;
};