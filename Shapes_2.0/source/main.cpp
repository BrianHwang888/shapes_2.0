#include<stdio.h>
#include<stdlib.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"../header/camera.h"
#include"../header/render_object.h"

//Inital window height and width
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//Window related functions and callback functions
void init_window(int option);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyboard_input_callback(GLFWwindow* window, int key, int scancode, int action, int mod);
void mouse_input_callback(GLFWwindow* window, double x_pos, double y_pos);
void process_input(GLFWwindow* window);

//Variables for main camera
camera main_camera(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, -1.0f));
double last_x = WINDOW_WIDTH / 2.0f;
double last_y = WINDOW_HEIGHT / 2.0f;
bool first_mouse = true;

//Variables for updating frames
float delta_time = 0.0f;
float last_frame = 0.0f;

//Main shader program
shader_program* basic_program;

int main() {
	glfwInit();
	printf("OpenGL version supported %s\n", glfwGetVersionString());

	//Window setup
	init_window(3);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Shapes", NULL, NULL);
	if (window == NULL) {
		perror("Failed to create window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		perror("Failed to load GLAD\n");
		exit(EXIT_FAILURE);
	}

	//Setup callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //register framebuffer callback
	glfwSetKeyCallback(window, keyboard_input_callback); //register keyboard callback
	glfwSetCursorPosCallback(window, mouse_input_callback); //register mosue callback
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //removes cursor in window

	//Load in shader files
	const char* shader_paths[2] = { "source/shader/basic_vertex.glsl", "source/shader/basic_fragment.glsl" }; //basic vertex and fragment shader paths
	basic_program = new shader_program(shader_paths[0], shader_paths[1]);

	
	float current_frame;
	//Sets background to light blue color
	glClearColor(0.529f, 0.807f, 0.92f, 1.0f);

	equilateral_triangle triangle(glm::vec3(0.0f, 0.0f, 0.0f), 10.0f);
	triangle.generate_triangle_buffer();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		//update frame variable
		current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;
		process_input(window); 

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

//Initalization of window
void init_window(int option) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, option);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, option);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

//Framebuffer callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//Keyboard callback function
void keyboard_input_callback(GLFWwindow* window, int key, int scancode, int action, int mod) {
	GLfloat current_time = (GLfloat)glfwGetTime();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}

//Process keyboard input
void process_input(GLFWwindow* window) {

	//WASD movment
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		main_camera.process_keyboard(FORWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		main_camera.process_keyboard(LEFT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		main_camera.process_keyboard(BACKWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		main_camera.process_keyboard(RIGHT, delta_time);
}

//Mouse input
void mouse_input_callback(GLFWwindow* window, double x_pos, double y_pos) {
	//checks to see if inital mouse movement
	if (first_mouse) {
		last_x = x_pos;
		last_y = y_pos;
		first_mouse = false;
	}

	double x_diff = x_pos - last_x;
	double y_diff = last_y - y_pos;

	last_x = x_pos;
	last_y = y_pos;

	main_camera.process_mouse(x_diff, y_diff);
}