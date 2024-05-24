#version 330
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_TexCoord;

uniform mat4 uView;
uniform mat4 uProjection;

//out vec3 v_Pos;

void main() {
	//v_Pos = a_Position;
	gl_Position = uProjection * uView * vec4(a_Position, 1.0);
}