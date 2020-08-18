#include "text.h"
#include <stdio.h>

#include "../shader/shader.h"
#include "font_texture.cpp"

Text::Text(GLFWwindow *window) {
	glGenBuffers(1, &VBO);

	// load font texture
	glGenTextures(1, &font_texture);
    glBindTexture(GL_TEXTURE_2D, font_texture); 
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 160, 20, 0, GL_ALPHA, GL_UNSIGNED_BYTE, font_texture_data);

	this->font_shader = get_font_shader();
	this->window = window;

	glUseProgram(font_shader);
	glUniform1i(glGetUniformLocation(font_shader, "texture1"), 0);
}

Text::~Text() {
	glDeleteTextures(1, &font_texture);
	glDeleteProgram(font_shader);
	glDeleteBuffers(1, &VBO);
}

void Text::printNum(float x, float y, float color[3], float size, float num, bool align_center) {
	char num_c[16];
    sprintf(num_c, "%g", num);

	int width, height;
    glfwGetWindowSize(window, &width, &height);

	// 3ICQ opengl plane map
	width /= 2;
	height /= 2;

	float bearing_x = 0.0, bearing_y = 0.0, y_pos;

	// calculate string width
    for (char n : num_c) {
		if (n == '\0') break;
		else if (n >= 48) n = (n - '0');
		else if (n == '-') n = 10;
		else if (n == '.') n = 11;

		bearing_x += glyphs[n].width * size / width;
	}

	// align string
	x -= align_center ? (bearing_x / 2) : bearing_x;
	y -= (10.0 * size / height); // half height of glyphs

	for (char n : num_c) {
		if (n == '\0') break;
		else if (n >= 48) n = (n - '0');
		else if (n == '-') n = 10;
		else if (n == '.') n = 11;

		bearing_x = glyphs[n].width * size / width;
		bearing_y = glyphs[n].height * size / height;
		y_pos = y + (glyphs[n].y_offset * size / width);
		
		// top left 
		d.push_back({
			x, y_pos, 
			color[0], color[1], color[2],
			glyphs[n].us, glyphs[n].ve
		});
		// bottom left
		d.push_back({
			x, (y_pos + bearing_y),
			color[0], color[1], color[2],
			glyphs[n].us, glyphs[n].vs
		});
		// bottom right
		d.push_back({
			(x + bearing_x), (y_pos + bearing_y),
			color[0], color[1], color[2],
			glyphs[n].ue, glyphs[n].vs
		});
		// top right
		d.push_back({
			(x + bearing_x), y_pos,
			color[0], color[1], color[2],
			glyphs[n].ue, glyphs[n].ve
		});

		// printf("%d at (%f,%f) (%f,%f)\n      (%f,%f) (%f,%f)\n", n,
		// 	d[d.size()-1].x, d[d.size()-1].y, d[d.size()-4].x, d[d.size()-4].y,
		// 	d[d.size()-2].x, d[d.size()-2].y, d[d.size()-3].x, d[d.size()-3].y
		// );

        x += bearing_x + 0.002;
    }
}

void Text::draw() {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, d.size() * sizeof(TextVertex), &d[0],  GL_STATIC_DRAW);

	// position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
	
	// enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font_texture);

	glUseProgram(font_shader);
    glDrawArrays(GL_QUADS, 0, d.size());

    glDisable(GL_BLEND);
}

void Text::clear() {
	d.clear();
	d.shrink_to_fit();
}