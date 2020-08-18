// serfreeman1337 // 24.05.20 //

#include <glad/glad.h>
#include <stdio.h>

unsigned int build_shader(const char *vertexSrc, const char *fragmentSrc) {
	char infoLog[512];
	int success;

	// Vertex shader
	unsigned int vertex;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSrc, NULL);

	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("Failed to compile vertexShader: [%s]\n", infoLog);

		glDeleteShader(vertex);
		return 0;
	}
	
	// Fragment shader
	unsigned int fragment;
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSrc, NULL);

	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("Failed to compile fragmentShader: [%s]\n", infoLog);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return 0;
	}

	unsigned int shader = glCreateProgram();

	glAttachShader(shader, vertex);
	glAttachShader(shader, fragment);

	glLinkProgram(shader);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		printf("Failed to link shaders: [%s]\n", infoLog);

		glDeleteProgram(shader);

		return 0;
	}

	return shader;
}

unsigned int get_line_shader() {
	const char *vertexCode = "#version 110\n"
		"attribute vec2 pos;\n"
		"void main() {\n"
		"	gl_Position = vec4(pos, 0.0, 1.0);\n"
		"}\0";
	const char *fragmentCode = "#version 110\n"
		"uniform vec3 color;\n"
		"void main() {\n"
		"	gl_FragColor = vec4(color, 1.0);\n"
		"}\0";
	
	return build_shader(vertexCode, fragmentCode);
}

unsigned int get_font_shader() {
	const char *vertexCode = "#version 110\n"
		"attribute vec2 aPos;\n"
		"attribute vec3 aColor;\n"
		"attribute vec2 aTexCoord;\n"
		"varying vec3 ourColor;\n"
		"varying vec2 TexCoord;\n"
		"void main() {\n"
		"	gl_Position = vec4(aPos, 0.0, 1.0);\n"
		"	ourColor = aColor;\n"
		"	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
		"}\0";
	const char *fragmentCode = "#version 110\n"
		"varying vec3 ourColor;\n"
		"varying vec2 TexCoord;\n"
		"uniform sampler2D texture1;\n"
		"uniform sampler2D texture2;\n"
		"void main() {\n"
		"	gl_FragColor = vec4(ourColor, texture2D(texture1, TexCoord).a);\n"
		"}\0";
	
	return build_shader(vertexCode, fragmentCode);
}