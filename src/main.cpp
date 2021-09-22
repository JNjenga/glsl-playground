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

bool
read_from_file(const char * file_name, std::string & vsource, std::string & fsource);

uint32_t
create_shader_program(const char * vsource, const char * fsource);

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


	uint32_t vao;
	{
		float vertices[] = {
			0.5f, -0.5f, 0.0f, // Bottom right
			-0.5f, -0.5f, 0.0f, // Bottom left
			-0.5f, 0.5f, 0.0f, // Top left

			-0.5f, 0.5f, 0.0f, // Top left
			0.5f, -0.5f, 0.0f, // Bottom right
			0.5f, 0.5f, 0.0f, // Top right 
		};

		uint32_t vbo;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Shader stuff
		std::string vsource, fsource;
		read_from_file("../test.glsl", vsource, fsource);
		auto pid = create_shader_program(vsource.c_str(), fsource.c_str());
		glUseProgram(pid);

	}

	while(!glfwWindowShouldClose(win))
	{
		int state = glfwGetKey(win, GLFW_KEY_R);
		if (state == GLFW_PRESS)
		{
			// Reload shareder
			// Shader stuff
			std::string vsource, fsource;
			read_from_file("../test.glsl", vsource, fsource);
			auto pid = create_shader_program(vsource.c_str(), fsource.c_str());
			glUseProgram(pid);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(win);
	}

	return 0;
}

bool read_from_file(const char * file_name, std::string & vsource, std::string & fsource)
{
	std::string buffer;

	std::ifstream file(file_name);
	file.seekg(0, std::ios::end);

	buffer.resize(file.tellg());
	file.seekg(0);
	file.read(&buffer[0], buffer.size());

	size_t vpos = buffer.find("--VERTEX--\n");

	if(vpos == std::string::npos)
	{
		// Print error
		std::cout << "Vertex not found!\n";
		return true;
	}

	size_t fpos = buffer.find("--FRAGMENT--\n");

	if(fpos == std::string::npos)
	{
		// Print error
		std::cout << "Fragment not found!\n";
		return true;
	}

	if(fpos < vpos)
	{
		vsource = buffer.substr(0, vpos);
		fsource = buffer.substr(vpos + 11, buffer.size());
	}
	else
	{
		vsource = buffer.substr(vpos + 11, fpos - 13);
		fsource = buffer.substr(fpos + 13, buffer.size());
	}

	file.close();

	return false;
}

uint32_t
create_shader_program(const char * vsource, const char * fsource)
{
	auto _create_shader = [](uint32_t shader_type, const char * source){

		uint32_t _shader = glCreateShader(shader_type);

		glShaderSource(_shader, 1, &source, NULL);
		glCompileShader(_shader);

		int success;
		char log[512];

		glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(_shader, 512, NULL, log);
			std::cout << "Error compile shader : " << log << '\n';
		}

		return _shader;
	};

	uint32_t vshader = _create_shader(GL_VERTEX_SHADER, vsource);
	uint32_t fshader = _create_shader(GL_FRAGMENT_SHADER, fsource);
	uint32_t program = glCreateProgram();

	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	glLinkProgram(program);

	{
		int success;
		char log[512];

		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if(!success)
		{
			glGetProgramInfoLog(program, 512, NULL, log);
			std::cout << "Error compile program: " << log << '\n';
		}
	}

	glDeleteShader(vshader);
	glDeleteShader(fshader);

	return program;
}
