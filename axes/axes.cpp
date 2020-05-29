// serfreeman1337 // 24.05.20 //

#include "axes.h"

#include <glad/glad.h>

#include <math.h>

Axes::Axes(float (*x_range)[2], float (*y_range)[2], float *x_dist, float *y_dist, unsigned int shader) {
	// set pointers
	this->x_range = x_range;
	this->y_range = y_range;
	this->x_dist = x_dist;
	this->y_dist = y_dist;
	this->shader = shader;

	glGenBuffers(1, &VBO);
}

void Axes::rebuild() {
	vector<array<float, 2>> d;
	
	build(false, 0.5, &d);
	build(true, 0.5, &d);

	vbo_size = d.size();

	if (!vbo_size)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vbo_size * (2 * sizeof(float)), &d.front(), GL_STATIC_DRAW);
}

void Axes::draw() {
	if (!vbo_size)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	int color = glGetUniformLocation(shader, "color");

	glUseProgram(shader);

	if (vbo_size) {
		glLineWidth(1.0f);
		glUniform3f(color, 1.0f, 0.5f, 0.2f);

		glDrawArrays(GL_LINES, 0, vbo_size);
	}
}

void Axes::build(bool isY, float step, vector<array<float, 2>> *d) {
	float (*range)[2], *dist;
	float (*o_range)[2], *o_dist;

	if (isY) {
		range = y_range;
		dist = y_dist;

		o_range = x_range;
		o_dist = x_dist;
	} else {
		range = x_range;
		dist = x_dist;

		o_range = y_range;
		o_dist = y_dist;
	}

	// printf("[axis%d] %lf->%lf\n",
	// 	   isY, (*range)[0], (*range)[1]);

	// calculate point O offset
	float off = (-(*o_range)[0] * (1.0 / *o_dist) * 2.0) - 1.0;

	if (fabs(off) >= 1.0)
		return;

	// axis lines
	if (isY) {
		d->push_back({off, 1.0f});
		d->push_back({off, -1.0f});
	}
	else {
		d->push_back({-1.0f, off});
		d->push_back({1.0f, off});
	}

	// calculate marker offset
	float m_offset;
	modf((*range)[0], &m_offset); // start placing markers from round integer
	// printf("-1->m_offset: %lf %d\n", m_offset, (m_offset >= 0.0));

	// calculate nearest marker step
	// TODO: find better method ?
	if ((*range)[0] >= 0.0)
		while (m_offset < (*range)[0])
			m_offset += step;
	else
		while (m_offset > (*range)[0] + step)
			m_offset -= step;

	m_offset -= (*range)[0];
	// printf("-2->m_offset: %lf\n", m_offset);

	// markers !!!
	for (float m_val = ((*range)[0] + m_offset); m_val <= (*range)[1]; m_val += step) {
		// marker position on screen !!!
		float xp = ((m_val - (*range)[0]) * (1.0 / *dist) * 2.0) - 1.0;
		float yp = 0.02;

		// printf("-> v: %lf, x: %lf\n", m_val, xp);

		if (xp <= -1.0) { // hack: fix displaying on screen edges;
			xp = -0.9999;
		}
		else if (xp >= 1.0) {
			xp = 0.9999;
		}

		if (isY) { // just turn around 90* degrees :D
			d->push_back({-yp + off, xp});
			d->push_back({yp + off, xp});
		}
		else {
			d->push_back({xp, yp + off});
			d->push_back({xp, -yp + off});
		}
	}
}