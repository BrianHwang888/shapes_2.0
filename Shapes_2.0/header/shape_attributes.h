#ifndef __SHAPE_ATTRIBUTE_H__
#define __SHAPE_ATTRIBUTE_H__

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/common.hpp>
#include<glm/gtc/matrix_transform.hpp>

/*----- shape_2D and shape_3D Attribute Class Definitions -----*/
//Shape_2D Class
class shape_2D {
private:

protected:
	int number_sides;
	double internal_angles;

	shape_2D();
	shape_2D(int sides, double total_angles);
	virtual double calculate_area() = 0;
	virtual double calculate_perimeter() = 0;

public:
	double area();
	double perimeter();
};

//Shape_3D Class
class shape_3D {
protected:
	int faces;
	int edges;
	double length;
	double depth;
	double height;

	shape_3D();
	shape_3D(int faces, double* measurements);
	double volume();
	double surface_area();
};

//Triangle Attributes; General attributes all triangles share
class triangle_attributes : public shape_2D {
protected:
	float base;
	float height;
	double calculate_area() override;
	triangle_attributes();
	triangle_attributes(float base_measurement, float heigh_measurement);

public:
	float get_base();
	float get_height();
};

//Equalaterial Triangle Attribute Class
class equilateral_triangle_attributes : public triangle_attributes {
private:
	double calculate_perimeter() override;

public:
	equilateral_triangle_attributes();
	equilateral_triangle_attributes(float side_length);

};

class isosceles_triangle_attributes : public triangle_attributes {
private:
	float equal_sides_len;

	double calculate_perimeter() override;

public:
	isosceles_triangle_attributes();
	isosceles_triangle_attributes(float side_length, float equal_lengths);
};

#endif