// serfreeman1337 // 24.05.20 //

#include "graph.h"

#include <glad/glad.h>

#include <stdio.h>
#include <math.h>

using namespace std;

Graph::Graph(string expr, float (*x_range)[2], float (*y_range)[2], float *x_dist, float *y_dist, unsigned int shader) {
	this->parser = new Parser(expr);
	
	// set pointers
	this->x_range = x_range;
	this->y_range = y_range;
	this->x_dist = x_dist;
	this->y_dist = y_dist;
	this->shader = shader;

	glGenBuffers(1, &VBO);
}

void Graph::rebuild() {
	vector<array<float, 2>> d;
	build(&d);
	vbo_size = d.size();

	// hora hora kimi no yume mo 

	if (!vbo_size)
		return;

	// kimi no namae mo arekore naku natta

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vbo_size * (2 * sizeof(float)), &d.front(), GL_STATIC_DRAW);
}

void Graph::draw() {
	if (!vbo_size)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	int color = glGetUniformLocation(shader, "color");

	glUseProgram(shader);

	if (vbo_size) {
		glLineWidth(1.2f);
		glUniform3f(color, 1.0f, 1.0f, 0.0f);

		glDrawArrays(GL_LINE_STRIP, 0, vbo_size);
	}
}

void Graph::build(std::vector<std::array<float, 2>> *d) {
	float step = 0.015;

	for (float x = (*x_range)[0], y; x <= (*x_range)[1]; x += step) {
		// y = sin(x) * x;
		y = parser->evaluate(x);
		// printf("y(%lf)=%lf [%lf|%lf]\n", x, y, (*x_range)[1], (*y_range)[1]);

		float xp = ((x - (*x_range)[0]) * (1.0 / *x_dist) * 2.0) - 1.0;
		float yp = ((y - (*y_range)[0]) * (1.0 / *y_dist) * 2.0) - 1.0;
		
		d->push_back({xp, yp});

		// printf("-[G]-> x: %lf, y: %lf\n", xp, yp);
		
	}
}
