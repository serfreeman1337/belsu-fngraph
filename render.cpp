// serfreeman1337 // 24.05.20 //

#include "axes/axes.h"
#include "graph/graph.h"
#include "shader/shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include <vector>

float x_range[2], y_range[2], x_dist, y_dist;

Axes *axes;
vector<Graph*> graphs;

void rebuild() {
	x_dist = x_range[1] - x_range[0];
	y_dist = y_range[1] - y_range[0];

	axes->rebuild();
	for (auto graph : graphs) {
		graph->rebuild();
	}
}

void display(GLFWwindow *window) {
	glClear(GL_COLOR_BUFFER_BIT);

	axes->draw();
	for (auto graph : graphs) {
		graph->draw();
	}

	glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	// printf("Resizing [%d,%d]\n", width, height);
	glViewport(0, 0, width, height);
}

void window_refresh_callback(GLFWwindow *window) {
	// printf("-> Window refresh\n");
	display(window);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	static unsigned char press;

	switch (key) {
		case GLFW_KEY_UP: press = (action != GLFW_RELEASE) ? (press | 1) : (press & ~1);
		break;
		case GLFW_KEY_DOWN: press = (action != GLFW_RELEASE) ? (press | 1 << 1) : (press & ~(1 << 1));
		break;
		case GLFW_KEY_RIGHT:  press = (action != GLFW_RELEASE) ? (press | 1 << 2) : (press & ~(1 << 2));
		break;
		case GLFW_KEY_LEFT: press = (action != GLFW_RELEASE) ? (press | 1 << 3) : (press & ~(1 << 3));
		break;
	}

	if (press) {
#define move_step 0.5f
		float move_x = 0.0f, move_y = 0.0f;
	
		if (press & 1)
			move_y += move_step;
		if (press & (1 << 1))
			move_y -= move_step;
		if (press & (1 << 2))
			move_x += move_step;
		if (press & (1 << 3))
			move_x -= move_step;

		if (move_x)  {
			x_range[0] += move_x;
			x_range[1] += move_x;
		}

		if (move_y) {
			y_range[0] += move_y;
			y_range[1] += move_y;
		}

		rebuild();
		display(window);
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_KP_ADD) {
			x_range[0] -= 1.0;
			x_range[1] += 1.0;

			y_range[0] -= 1.0;
			y_range[1] += 1.0;

			rebuild();
			display(window);
		}
		else if (key == GLFW_KEY_KP_SUBTRACT) {
			x_range[0] += 1.0;
			x_range[1] -= 1.0;

			y_range[0] += 1.0;
			y_range[1] -= 1.0;

			rebuild();
			display(window);
		}
	}
}

double last_pos[2];

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos){
	double diff_x = xpos - last_pos[0];
	double diff_y = ypos - last_pos[1];

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	double normalizedX = (-2.0 * diff_x / width) * ((x_range[1] - x_range[0]) / 2.0);
	double normalizedY = (2.0 * diff_y / height) * ((y_range[1] - y_range[0]) / 2.0);

	x_range[0] += normalizedX;
	x_range[1] += normalizedX;

	y_range[0] += normalizedY;
	y_range[1] += normalizedY;

	rebuild();

	// printf("->POS-> %lf,%lf [%lf,%lf] [%lf,%lf]\n", xpos, ypos, diff_x, diff_y, normalizedX, normalizedY);

	last_pos[0] = xpos;
	last_pos[1] = ypos;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			glfwGetCursorPos(window, &last_pos[0], &last_pos[1]);
			glfwSetCursorPosCallback(window, cursor_position_callback);
		}
		else if (action == GLFW_RELEASE)
			glfwSetCursorPosCallback(window, NULL);

	}
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
	// printf("-scroll-> %lf %lf\n", xoffset, yoffset);

	if (yoffset == 1.0) {
		x_range[0] *= 0.5;
		x_range[1] *= 0.5;
		y_range[0] *= 0.5;
		y_range[1] *= 0.5;
	} else if (yoffset == -1.0) {
		x_range[0] *= 1.5;
		x_range[1] *= 1.5;
		y_range[0] *= 1.5;
		y_range[1] *= 1.5;
	}

	rebuild();
	display(window);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	}
}

void init_render(string str) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow *window = glfwCreateWindow(400, 400, str.c_str(), NULL, NULL);
	if (window == NULL) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowRefreshCallback(window, window_refresh_callback);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to load OpenGL and its extensions\n");
		return;
	}
	glViewport(0, 0, 400, 400);

	// 
	x_range[0] = y_range[0] = -1.0f;
	x_range[1] = y_range[1] = 1.0f;

	unsigned int shader = get_line_shader();
	axes = new Axes(&x_range, &y_range, &x_dist, &y_dist, shader);
	
	string delim = ",";
	auto start = 0U;
	auto end = str.find(delim);
	while (end != string::npos) {
		graphs.push_back(new Graph(str.substr(start, end - start), &x_range, &y_range, &x_dist, &y_dist, shader));
		
		start = end + delim.length();
		end = str.find(delim, start);
	}
	
	graphs.push_back(new Graph(str.substr(start, end), &x_range, &y_range, &x_dist, &y_dist, shader));

	rebuild();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		display(window);

		glfwWaitEvents();
	}

	glfwTerminate();
	
	graphs.clear();
}