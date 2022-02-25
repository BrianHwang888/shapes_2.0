#ifndef __RENDER_OBJECT_H__
#define __RENDER_OBJECT_H__

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include"shader.h"
#include"shape_attributes.h"

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
	void set_shader(shader_program& shader);

protected:
	int total_vertices;
	glm::vec3 position;
	
	glm::vec3* position_buffer;
	glm::vec3* normal_buffer;
	color_attribute* color;
	shader_program shader;
	GLuint VAO;
	GLuint VBO;

	glm::mat4 model;
	glm::mat4 rotation;
	glm::mat4 translation;
	glm::mat4 scaling;

	render_object();
	render_object(int num_vertices, glm::vec3& position, glm::vec4 color);
	render_object(int num_vertices, glm::vec3& position, glm::vec4 color, shader_program& program);
	void generate_buffer();
	virtual void create_color();
	virtual void create_normal();
	void draw();

public:
	glm::mat4 get_model_matrix();
	glm::mat4 get_rotation_matrix();
	glm::mat4 get_translation_matrix();
	glm::mat4 get_scaling_matrix();

	void set_model_matrix(glm::mat4& new_model_mat);
	void set_rotation_matrix(glm::mat4& new_rotation_mat);
	void set_translation_matrix(glm::mat4& new_translation_mat);
	void set_scaling_matrix(glm::mat4& new_scaling_mat);

	void rotate(float angle, glm::vec3 axis);
	void translate(glm::vec3 point);
	void scale(float size);

	void apply_transformations();
};

class equilateral_triangle : public render_object {
private:
	float side_len;
	equilateral_triangle_attributes properties;

	void create_position();
public:
	equilateral_triangle();
	equilateral_triangle(shader_program& program, glm::vec3 spawn_position, float side_length);
	void generate_triangle_buffer();
	void draw_triangle();
};

class square : public render_object {
private:
	float side_len;
	void create_position();

public:
	square();
	square(shader_program& shader_program, float side_length);
	square(shader_program& shader_program, glm::vec3 spawn_position, float side_length);
	void generate_square_buffer();
	void draw_square();
};
#endif //! __RENDER_OBJECT_H__