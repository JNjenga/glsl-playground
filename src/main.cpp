#include "ogl.h"
#include "util.h"

#include <iostream>
#include <fstream>

#define GLEW_STATIC 
#include <GL/glew.h>

#define GLFW_STATIC
#include <GLFW/glfw3.h>

const int WIDTH  = 800;
const int HEIGHT = 640;

const std::string read_from_file(const char * file_name)
{
	std::string buffer;

	std::ifstream file(file_name);
	file.seekg(0, std::ios::end);

	buffer.resize(file.tellg());
	file.seekg(0);
	file.read(&buffer[0], buffer.size());

	return buffer;
}

uint32_t create_shader_program()
{
	auto _create_shader = [](uint32_t shader_type, const char * source){

		uint32_t _shader = glCreateShader(shader_type);

		glShaderSource(_shader, 1, &source, NULL);
		glCompileShader(_shader);

		// TODO : Check source/compile errors

		return _shader;
	};

	const char *vsource ="#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 0.5);\n"
		"   color = vec4(aPos, 1.0);\n"
		"}\0";

	const char *fsource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec4 color;\n"
		"void main()\n"
		"{\n"
		"   FragColor = color;\n"
		"}\n\0";

	uint32_t vshader = _create_shader(GL_VERTEX_SHADER, vsource);
	uint32_t fshader = _create_shader(GL_FRAGMENT_SHADER, fsource);
	uint32_t program = glCreateProgram();

	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	glLinkProgram(program);

	glDeleteShader(vshader);
	glDeleteShader(fshader);

	return program;
}

uint32_t create_rec_vao()
{
	float vertices[] = {
		0.5f, -0.5f, 0.0f, // Bottom right
		-0.5f, -0.5f, 0.0f, // Bottom left
		-0.5f, 0.5f, 0.0f, // Top left

		-0.5f, 0.5f, 0.0f, // Top left
		0.5f, -0.5f, 0.0f, // Bottom right
		0.5f, 0.5f, 0.0f, // Top right 
	};

	uint32_t vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return vao;
}

int main()
{
	{
		int glfwErr = glfwInit();
		if(glfwErr != GLFW_TRUE)
		{
			std::cout << "ERROR [GLFW] : Init failed\n";
			return -1;
		}
	}

	glfwDefaultWindowHints();

	GLFWwindow * win = glfwCreateWindow(WIDTH, HEIGHT, "GLSL Playground", NULL, NULL);

	if(win == NULL)
	{
		// Print error
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(win);

	if(glewInit() != GLEW_OK)
	{
		// Print error
		glfwTerminate();
		return -1;
	}

	auto pid = create_shader_program();
	glUseProgram(pid);

	auto vao = create_rec_vao();

	while(!glfwWindowShouldClose(win))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(win);
	}

	return 0;
}
