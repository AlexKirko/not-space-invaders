#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 tex_pos;

out vec2 v_tex_pos;

void main()
{
	gl_Position = vec4(position.x, position.y, 0.0, 1.0);
	v_tex_pos = tex_pos;
}
