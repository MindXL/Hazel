// Basic Texture Shader

#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TextureCoordinates;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

out vec2 v_TextureCoordinates;

void main()
{
	v_TextureCoordinates = a_TextureCoordinates;
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec2 v_TextureCoordinates;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TextureCoordinates);
}
