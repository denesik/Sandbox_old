#include "Camera.h"

#define GLM_FORCE_RADIANS
#include <gtc/matrix_transform.hpp>

Camera::Camera(void)
{
	fov = 45.0f;
	windowWidth = 1;
	windowHeight = 1;
	aspect = float(windowWidth) / float(windowHeight);
	zNear = 0.1f;
	zFar = 100.f;

	projection = glm::perspective(fov, aspect, zNear, zFar);
	view = glm::lookAt
	(
		glm::vec3(0,0,2), // ѕозици€ камеры
		glm::vec3(0,0,1), // направление взгл€да
		glm::vec3(0,1,0)  // вектор Up
	);
	rotx = 0;
	roty = 0;
	rotatex = mat4(1.0f);
	
}


Camera::~Camera(void)
{
}

void Camera::SetWindowSize( unsigned int width, unsigned int height )
{
	windowWidth = width;
	windowHeight = height;
	aspect = float(windowWidth) / float(windowHeight);

	projection = glm::perspective(fov, aspect, zNear, zFar);
}

void Camera::RotateX( float angle )
{
	float ax = radians(angle);
	rotx += ax;
	rotatex = glm::rotate(mat4(1.0f), ax, vec3(0.0f, 1.0f, 0.0f));
	view = rotatex * view;
}

void Camera::RotateY( float angle )
{

	roty += radians(angle);

}

void Camera::MoveX( float dist )
{
	mat4 translate;
	translate = glm::translate(mat4(1.0f), vec3(-dist, 0.0f, 0.0f));
	view = translate * view;
}

void Camera::MoveZ( float dist )
{
	mat4 translate;
	translate = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, dist));
	view = translate * view;
}

glm::mat4 Camera::CalculateMatrix()
{
	mat4 rotatey = glm::rotate(mat4(1.0f), roty, vec3(1.0f, 0.0f, 0.0f));
	return projection * rotatey * view;
}
