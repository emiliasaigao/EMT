#version 330

//in vec3 v_Pos;
in vec2 v_TexCoord;
out vec4 color;

uniform sampler2D uCubeTex;

void main() {
	//color = vec4(v_Pos / 2.0 + vec3(0.5), 1.0);
	color = texture(uCubeTex, v_TexCoord);
}