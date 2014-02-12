#include "Camera.h"

#include <gtc/matrix_transform.hpp>

Camera::Camera(void)
{
}


Camera::~Camera(void)
{
}

void Camera::Set()
{
	projection = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	view = lookAt
		(
		glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
}
