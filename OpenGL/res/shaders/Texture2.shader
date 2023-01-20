#shader vertex
#version 450 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexSlot;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexSlot;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * a_Position;
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexSlot = a_TexSlot;
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexSlot;

uniform sampler2D u_Texture[2];

void main()
{
	int index = int(v_TexSlot);

	color = index == 0 ? v_Color : texture(u_Texture[index], v_TexCoord);
	//color = vec4(v_TexCoord, v_TexSlot, 1.0f);
	//color = v_Color;
};