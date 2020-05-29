// serfreeman1337 // 24.05.20 //

#pragma once

#include <array>
#include <vector>

#include "../parser/parser.h"

using namespace std;

class Graph {
public:
	Graph(string expr, float (*x_range)[2], float (*y_range)[2], float *x_dist, float *y_dist, unsigned int shader);

	void rebuild();
	void draw();

	void set_expression(string str);
private:
	float (*x_range)[2], (*y_range)[2];
	float *x_dist, *y_dist;

	unsigned int shader;

	unsigned int VBO;
	unsigned int vbo_size;
	// vector<unsigned int> vbo_breaks;

	Parser *parser;

	void build(vector<array<float, 2>> *d);
};