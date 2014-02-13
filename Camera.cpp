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
		glm::vec3(0,0,2), // ������� ������
		glm::vec3(0,0,1), // ����������� �������
		glm::vec3(0,1,0)  // ������ Up
	);
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
	view = glm::rotate( view, angle, vec3(-1.0f, 0.0f, 0.0f));
}

void Camera::RotateY( float angle )
{
	view = glm::rotate( view, angle, vec3(0.0f, -1.0f, 0.0f));
}

void Camera::MoveX( float dist )
{
	view = glm::translate(view, vec3(-dist, 0.0f, 0.0f));
}

void Camera::MoveY( float dist )
{
	view = glm::translate(view, vec3(0.0f, 0.0f, dist));
}

glm::mat4 Camera::CalculateMatrix()
{
	return projection * view;
}
