#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textCoord;

out vec2 v_TextCoord;

void main()
{
	gl_Position = position;
	v_TextCoord = textCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TextCoord

uniform vec4 u_Colour;//uniform for the shader so that it can be set outside of the shgader instead of being hard coded in the shader itself
uniform sampler2D u_Texture

void main()
{
	vec4 texColour = texture(u_Texture, v_TextCoord);
	color = texColour;
};