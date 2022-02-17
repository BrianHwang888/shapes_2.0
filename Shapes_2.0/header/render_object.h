#ifndef __RENDER_OBJECT_H__
#define __RENDER_OBJECT_H__

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include"shader.h"
#include"shape_attributes.h"
#include"stb_image.h"

#define STB_IMAGE_IMPLEMENTATION

//Color attribute base class
class color_attribute {
protected:
	glm::vec4* color;
	glm::vec4* color_buffer;

public:
	color_attribute(glm::vec4 color, int vertices);
	~color_attribute();

	void generate_single_color(int vertices);
	glm::vec4* get_color_buffer();
};

class texture_attribute {
private:
	unsigned int texture;
	unsigned char* data;
	int width;
	int height;
	int nr_channels;

public:
	texture_attribute(char *texture_path, int width, int height, int num_color_channels);
	
};
//Render object base class
class render_object {
private:
	virtual void create_position() = 0;
	void set_shader(shader_program shader);

protected:
	int vertices;
	glm::vec3 position;
	
	glm::vec3* position_buffer;
	glm::vec3* normal_buffer;
	color_attribute* color;
	shader_program shader;
	GLuint VAO;
	GLuint VBO;
	glm::mat4 model;

	render_object();
	render_object(int num_vertices, glm::vec3 position, glm::vec4 color);
	void generate_buffer();
	virtual void create_color();
	virtual void create_normal();

};

class equilateral_triangle : public render_object {
private:
	float side_len;
	equilateral_triangle_attributes properties;

	void create_position();
public:
	equilateral_triangle();
	equilateral_triangle(glm::vec3 spawn_position, float side_length);
};

#endif //! __RENDER_OBJECT_H__