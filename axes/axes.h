// serfreeman1337 // 24.05.20 //

#pragma once

#include <vector>

#include "../text/text.h"

using namespace std;

typedef struct {
	float x, y;
}
AxesVertex;

class Axes {
public:
	Axes(GLFWwindow *window, Text *text, unsigned int shader,
		float (*x_range)[2], float (*y_range)[2], float *x_dist, float *y_dist
	);

	void rebuild();
	void draw();
private:
	GLFWwindow *window;
	Text *text;
	unsigned int shader;

	unsigned int VBO;
	unsigned int vbo_size;

	float (*x_range)[2], (*y_range)[2];
	float *x_dist, *y_dist;
	float x_step, y_step;

	std::vector<AxesVertex> d;

	void build(bool is_y);
	void calculate_step(bool is_y);
};