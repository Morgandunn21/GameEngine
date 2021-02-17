#version 430

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform vec4 c;

void main(void)
{	
	color = c;
}