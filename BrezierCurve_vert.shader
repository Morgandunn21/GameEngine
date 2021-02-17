#version 430

layout (location=0) in vec2 position;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform vec4 c;

void main(void)
{	
	gl_Position = proj_matrix * mv_matrix * vec4(position,0,1);
}