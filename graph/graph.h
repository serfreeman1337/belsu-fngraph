// serfreeman1337 // 24.05.20 //

#pragma once

#include <vector>
#include "../parser/parser.h"

typedef struct {
	float x, y;
}
GraphVertex;

class Graph {
public:
	Graph(string expr, 
		float (*x_range)[2], float (*y_range)[2], 
		float *x_dist, float *y_dist, 
		unsigned int shader);
	~Graph();

	void rebuild();
	void draw();

	void set_expression(string str);
private:
	Parser *parser;
	unsigned int shader;

	unsigned int VBO;
	unsigned int vbo_size;

	float (*x_range)[2], (*y_range)[2];
	float *x_dist, *y_dist;

	std::vector<GraphVertex> d;

	// vector<unsigned int> vbo_breaks;

	void build();
};