--VERTEX--
#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 color;
void main()
{
	gl_Position = vec4(aPos, 0.5);
	color = vec4(aPos, 1.0);
};
--FRAGMENT--
#version 330 core
out vec4 FragColor;
in vec4 color;
void main()
{
	float x = fract(sin(dot(vec2(color.x, color.y), vec2(12.9898, 78.233))) * 43758.5453);
	FragColor = vec4(x, color.x, color.z, 1.0f);// color;
};
