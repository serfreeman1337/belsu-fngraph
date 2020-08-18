// serfreeman1337 // 24.05.20 //

#include "axes.h"

#include <math.h>
#include <stdio.h>
#include <cstring>

Axes::Axes(GLFWwindow *window, Text *text, unsigned int shader,
	float (*x_range)[2], float (*y_range)[2], float *x_dist, float *y_dist
) {
	// set pointers
	this->x_range = x_range;
	this->y_range = y_range;
	this->x_dist = x_dist;
	this->y_dist = y_dist;
	this->shader = shader;

	this->window = window;
	this->text = text;

	glGenBuffers(1, &VBO);
}

void Axes::rebuild() {
	d.clear();
	d.shrink_to_fit();

	calculate_step(false);
	calculate_step(true);

	build(false);
	build(true);

	vbo_size = d.size();

	if (!vbo_size)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vbo_size * sizeof(AxesVertex), &d[0], GL_STATIC_DRAW);
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

void Axes::build(bool is_y) {
	float (*range)[2], *dist;
	float (*o_range)[2], *o_dist;
	float step;

	if (is_y) {
		range = y_range;
		dist = y_dist;
		step = y_step;

		o_range = x_range;
		o_dist = x_dist;
	} else {
		range = x_range;
		dist = x_dist;
		step = x_step;

		o_range = y_range;
		o_dist = y_dist;
	}

	// printf("[axis%d] %lf->%lf, step: %g\n",
	// 	   is_y, (*range)[0], (*range)[1], step);

	// calculate offset from point 0
	float off = (-(*o_range)[0] * (1.0 / *o_dist) * 2.0) - 1.0;

	if (fabs(off) >= 1.0)
		return;

	// axis lines
	if (is_y) {
		d.push_back({off, 1.0f});
		d.push_back({off, -1.0f});
	}
	else {
		d.push_back({-1.0f, off});
		d.push_back({1.0f, off});
	}

	// calculate marker offset
	float m_offset;

	modf((*range)[0], &m_offset); // start placing markers from round integer

	// calculate nearest marker step
	// TODO: find better method ?

	if (step <= 1.0) {
		if ((*range)[0] >= 0.0) {
			while (m_offset < (*range)[0]) {
				m_offset += step;
			}
		} else {
			while (m_offset > (*range)[0] + step) {
				m_offset -= step;
			}
		}
	} else {
		int i_offset = static_cast<int>(m_offset);
		int i_step = static_cast<int>(step);

		if ((*range)[0] >= 0.0) {
			while (m_offset < (*range)[0] || i_offset % i_step != 0) {
				i_offset += 1;
				m_offset += 1.0;
			}
		} else {
			while (i_offset % i_step != 0) {
				i_offset += 1;
				m_offset += 1.0;
			}
		}
	}

	m_offset -= (*range)[0];

	float text_color[3] = {1.0f, 0.5f, 0.2f};

	// markers !!!
	for (float m_val = ((*range)[0] + m_offset), x_pos, y_pos, text_x, text_y; 
		m_val <= (*range)[1]; 
		m_val += step
	) {
		// marker position on screen !!!
		x_pos = ((m_val - (*range)[0]) * (1.0 / *dist) * 2.0) - 1.0;
		y_pos = 0.02;

		// printf("-> v: %lf, x: %lf\n", m_val, xp);

		if (x_pos <= -1.0) { // hack: fix displaying on screen edges;
			x_pos = -0.9999;
		}
		else if (x_pos >= 1.0) {
			x_pos = 0.9999;
		}

		if (is_y) { // just turn around 90* degrees :D
			// marker from left to right
			d.push_back({-y_pos + off, x_pos});
			d.push_back({y_pos + off, x_pos});

			// set text position
			text_x = -y_pos + off - 0.02;
			text_y = x_pos;
		}
		else {
			// from up to down
			d.push_back({x_pos, y_pos + off});
			d.push_back({x_pos, -y_pos + off});

			// set text position
			text_x = x_pos;
			text_y = -y_pos + off - 0.04;
		}

		// TODO: fix ?
		m_val = round( m_val * 10000.0 ) / 10000.0;

		if (m_val == 0.0) {
			if (!is_y) {
				continue;
			}

			text_y -= 0.06;
		}

		text->printNum(text_x, text_y, text_color, 0.5, m_val, !is_y);
	}
}

void Axes::calculate_step(bool is_y) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	float *dist, *step;
	int size;

	if (is_y) {
		dist = y_dist;
		step = &y_step;

		size = height;
	}  else {
		dist = x_dist;
		step = &x_step;

		size = width;
	}

	*step = 1.0f;

	// calculate step size
	float pixel_distance;

	unsigned char cur_step = 1;
	float cur_multi = 1.0f, multi = 10.0f;
	bool is_decreasing = false;

	// check if need to decrease step instead increasing
	pixel_distance = (*step * (1.0 / *dist) * size);

	if (pixel_distance > 64.0f) {
		cur_multi = multi = 0.1;
		cur_step = 3; // start from 0.5 and go downwards

		is_decreasing = true;
	}

	char step_str[16];
	unsigned char step_distance;

	while (true) {
		switch (cur_step) {
			case 1: *step = 1.0 * cur_multi;
			break;
			case 2: *step = 2.0 * cur_multi;
			break;
			case 3: *step = 5.0 * cur_multi;
			break;
			case 4:
				cur_multi *= multi;
			break;
		}

		sprintf(step_str, "-%g", *step);
		step_distance = strlen(step_str) * 15 + 12; // 15 pixels per char + 12 distance between

		pixel_distance = (*step * (1.0 / *dist) * size);
		
		if (is_decreasing) {
			if (pixel_distance < step_distance) break; // step size found

			cur_step --; // go backwards in decrease mode
			if (cur_step == 0) cur_step = 4;
		} else {
			if (pixel_distance > step_distance) break; // step size found

			cur_step ++;
			if (cur_step == 5) cur_step = 0;
		}
	}
}