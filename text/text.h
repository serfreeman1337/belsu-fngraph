#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#pragma once

typedef struct {
    float width;
    float height;
    float us, ue; // ustart uend
	float vs, ve;
	float y_offset;
}
Glyph;

typedef struct {
	float x, y;
	float r, g, b;
	float u, v;
}
TextVertex;

class Text {
public:
	Text(GLFWwindow *window);
	~Text();

	void printNum(float x, float y, float color[3], float size, float num, bool align_center);
	void draw();
	void clear();
private:
	const Glyph glyphs[12] = {
		{14.0f, 20.0f, 0.2f, 0.2875f, 0.0f, 1.0f}, // 0
        {9.0f, 20.0f, 0.8375f, 0.89375f, 0.0f, 1.0f}, // 1
        {14.0f, 20.0f, 0.29375f, 0.38125f, 0.0f, 1.0f}, // 2
        {13.0f, 20.0f, 0.575f, 0.65625f, 0.0f, 1.0f}, // 3
        {14.0f, 20.0f, 0.3875f, 0.475f, 0.0f, 1.0f}, // 4
        {13.0f, 20.0f, 0.6625f, 0.74375f, 0.0f, 1.0f}, // 5
        {14.0f, 20.0f, 0.48125f, 0.56875f, 0.0f, 1.0f}, // 6
        {13.0f, 20.0f, 0.75f, 0.83125f, 0.0f, 1.0f}, // 7
        {15.0f, 20.0f, 0.0f, 0.09375f, 0.0f, 1.0f}, // 8
        {15.0f, 20.0f, 0.1f, 0.19375f, 0.0f, 1.0f}, // 9
		{9.0f, 2.0f, 0.9f, 0.95625f, 0.0f, 0.1f, 9.0f}, // -
		{4.0f, 2.0f, 0.9625f, 0.9875f, 0.0f, 0.1f} // .
    };

	unsigned int VBO;
	unsigned int vbo_size;

	GLFWwindow *window;
	unsigned int font_texture;
	unsigned int font_shader;

	std::vector<TextVertex> d;
};

