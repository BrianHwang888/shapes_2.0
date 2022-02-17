#include"../header/shape_attributes.h"

/*----- Shape Class -----*/
//Constructors of shape_2D
shape_2D::shape_2D() {
	number_sides = 0;
	internal_angles = 0;
}
shape_2D::shape_2D(int sides, double total_angles) {
	number_sides = sides;
	internal_angles = total_angles;
}

//Definition of area 
double shape_2D::area() { return calculate_area(); }
//Definition of perimeter
double shape_2D::perimeter() { return calculate_perimeter(); }

/*----- Traingle Attribute Class -----*/
//Consructors of triangle_attributes
triangle_attributes::triangle_attributes() : shape_2D(3, 180.0f) {
	base = 0;
	height = 0;
}
triangle_attributes::triangle_attributes(float base_measurement, float height_measurement) : shape_2D(3, 180.f) {
	base = base_measurement;
	height = height_measurement;
}
//Area definition
double triangle_attributes::calculate_area() { return base * height / 2; }

//Getters
float triangle_attributes::get_base() { return base; }
float triangle_attributes::get_height() { return height; }

/*----- Equalarterial Triangle Atrributes Class -----*/
//Constructors of equalaterial_triangle_attributes
equilateral_triangle_attributes::equilateral_triangle_attributes() : triangle_attributes(0, 0) {}
equilateral_triangle_attributes::equilateral_triangle_attributes(float side_length) : triangle_attributes(side_length, glm::tan(glm::pi<float>() / 3) * (side_length / 2)) {}

//Perimeter Definition
double equilateral_triangle_attributes::calculate_perimeter() { return double(base) * 3; }

/*----- Isosceles Triangle Atrributes Class -----*/
isosceles_triangle_attributes::isosceles_triangle_attributes() : triangle_attributes() {
	equal_sides_len = 0;
}
isosceles_triangle_attributes::isosceles_triangle_attributes(float side_length, float equal_lengths) : triangle_attributes(side_length, glm::tan(glm::pi<float>() / 3)* (side_length / 2)) {
	equal_sides_len = equal_lengths;
}

//Perimeter Definition
double isosceles_triangle_attributes::calculate_perimeter() { return (double)base + equal_sides_len * 2; }