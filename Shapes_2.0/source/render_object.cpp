#include"../header/render_object.h"
#define STB_IMAGE_IMPLEMENTATION
#include"../header/stb_image.h"

/*----- Color Related Classes -----*/
color_attribute::color_attribute(glm::vec4 color, int total_vertices) {
	this->color = new glm::vec4(color);
	color_buffer = new glm::vec4[total_vertices];

}
color_attribute::~color_attribute() {
	delete[] color;
	delete[] color_buffer;
}
void color_attribute::generate_single_color(int total_vertices) {
	for (int i = 0; i < total_vertices; i++)
		color_buffer[i] = *color;
}
glm::vec4* color_attribute::get_color_buffer() { return color_buffer; }

/*----- Texture Releated Classes -----*/
texture_attribute::texture_attribute(char* texture_path, int width, int height, int num_color_channels) {
	this->width = width;
	this->height = height;
	nr_channels = num_color_channels;
	data = stbi_load(texture_path, &this->width, &this->height, &this->nr_channels, 0);
	texture = 0;
}


/*----- Render Object Related Classess -----*/

//Constructors of base render_object
render_object::render_object() {
	VAO = 0;
	VBO = 0;
	total_vertices = 0;
	color = NULL;
	normal_buffer = NULL;
	position_buffer = NULL;
}
render_object::render_object(int num_vertices, glm::vec3& position, glm::vec4 color) {
	VAO = 0;
	VBO = 0;
	total_vertices = num_vertices;
	position_buffer = new glm::vec3[total_vertices];
	this->color = new color_attribute(color, total_vertices);
	normal_buffer = new glm::vec3[total_vertices];
}
render_object::render_object(int num_vertices, glm::vec3& position, glm::vec4 color, shader_program& program) {
	VAO = 0;
	VBO = 0;
	shader = program;
	total_vertices = num_vertices;
	position_buffer = new glm::vec3[total_vertices];
	this->color = new color_attribute(color, total_vertices);
	normal_buffer = new glm::vec3[total_vertices];
}
//Create color attributes
void render_object::create_color() {
	color->generate_single_color(total_vertices);
}

//Create normal data
void render_object::create_normal() {
	glm::vec3 u, v, normal;
	for (int i = 0; i < total_vertices; i += 3) {
		u = position_buffer[i + 1] - position_buffer[i];
		v = position_buffer[i + 2] - position_buffer[i];
		normal = glm::normalize(glm::cross(u, v));

		normal_buffer[i] = normal;
		normal_buffer[i + 1] = normal;
		normal_buffer[i + 2] = normal;
	}
}
//Generate data buffers (positions, color, and normal)
void render_object::generate_buffer() {
	unsigned long long vertex_data_size = sizeof(glm::vec3) * 2 + sizeof(glm::vec4);
	GLuint vertex_position, vertex_color, vertex_normal;

	create_position();
	create_color();
	create_normal();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	vertex_position = glGetAttribLocation(shader.ID, "vPosition");
	vertex_normal = glGetAttribLocation(shader.ID, "vNormal");
	vertex_color = glGetAttribLocation(shader.ID, "vColor");

	glBufferData(GL_ARRAY_BUFFER, total_vertices * vertex_data_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, total_vertices * sizeof(glm::vec3), (void*)position_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, total_vertices * sizeof(glm::vec3), total_vertices * sizeof(glm::vec4), (void*)color->get_color_buffer());
	glBufferSubData(GL_ARRAY_BUFFER, total_vertices * (sizeof(glm::vec3) + sizeof(glm::vec4)), total_vertices * sizeof(glm::vec3), (void*)normal_buffer);

	glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glVertexAttribPointer(vertex_color, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)(total_vertices * sizeof(glm::vec3)));
	glVertexAttribPointer(vertex_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(total_vertices * (sizeof(glm::vec3) + sizeof(glm::vec4))));

	glEnableVertexAttribArray(vertex_position);
	glEnableVertexAttribArray(vertex_color);
	glEnableVertexAttribArray(vertex_normal);
}
void render_object::draw() {
	glUseProgram(shader.ID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, total_vertices);
}
//Setters
void render_object::set_shader(shader_program& shader) {
	this->shader = shader;
}
void render_object::set_model_matrix(glm::mat4& new_model_mat) {
	model = new_model_mat;
}
void render_object::set_rotation_matrix(glm::mat4& new_rotation_mat) {
	rotation = new_rotation_mat;
}
void render_object::set_translation_matrix(glm::mat4& new_translation_mat) {
	translation = new_translation_mat;
}
void render_object::set_scaling_matrix(glm::mat4& new_scaling_mat) {
	scaling = new_scaling_mat;
}

//Getters
glm::mat4 render_object::get_model_matrix() {
	return model;
}
glm::mat4 render_object::get_rotation_matrix() {
	return rotation;
}
glm::mat4 render_object::get_translation_matrix() {
	return translation;
}
glm::mat4 render_object::get_scaling_matrix() {
	return scaling;
}

/*----- Equilateral Triangle Definitions -----*/
equilateral_triangle::equilateral_triangle() {
	side_len = 0;
}
equilateral_triangle::equilateral_triangle(shader_program& program, glm::vec3 spawn_position, float side_length) : render_object(3, position, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), program){
	position = spawn_position;
	side_len = side_length;
	properties = equilateral_triangle_attributes(side_length);
}
void equilateral_triangle::create_position() {
	float half_base = properties.get_base() / 2;
	float half_height = properties.get_height() / 2;
	
	position_buffer[0] = glm::vec3(position.x + half_base, position.y - half_height, position.z);
	position_buffer[1] = glm::vec3(position.x, position.y + half_height, position.z);
	position_buffer[2] = glm::vec3(position.x - half_base, position.y - half_height, position.z);
}
void equilateral_triangle::generate_triangle_buffer() {
	generate_buffer();
}
void equilateral_triangle::draw_triangle() {
	draw();
}

/*----- Square Definitions -----*/
square::square() {
	side_len = 0;
}
square::square(shader_program& program, glm::vec3 spawn_position, float side_length) : render_object(6, position, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), program) {
	position = spawn_position;
	side_len = side_length;

}
square::square(shader_program& program, float side_length) : render_object(6, position, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), program) {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	side_len = side_length;
}
void square::create_position() {
	float half_len = side_len / 2;

	position_buffer[0] = glm::vec3(position.x - half_len, position.y + half_len, position.z);
	position_buffer[1] = glm::vec3(position.x - half_len, position.y - half_len, position.z);
	position_buffer[2] = glm::vec3(position.x + half_len, position.y + half_len, position.z);
	position_buffer[3] = glm::vec3(position.x + half_len, position.y - half_len, position.z);
	position_buffer[4] = glm::vec3(position.x + half_len, position.y + half_len, position.z);
	position_buffer[5] = glm::vec3(position.x - half_len, position.y - half_len, position.z);
}
void square::generate_square_buffer() {
	generate_buffer();
}
void square::draw_square() {
	draw();
}