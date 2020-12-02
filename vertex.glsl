#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos.x / 640.0f - 1.0f, aPos.y / 360.0f + 1.0f, 0.0f, 1.0);
	TexCoord = aTexCoord;
}