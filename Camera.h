#ifndef Camera_h__
#define Camera_h__

#include "GameMath.h"

class Camera
{
private:
	float fov;
	unsigned int windowWidth;
	unsigned int windowHeight;
	float aspect;
	float zNear;
	float zFar;


	float rotx;
	float roty;
	mat4 perspectiveProjection;
	mat4 view;

	mat4 rotatex;

public:
	Camera(void);
	~Camera(void);

	void SetWindowSize(unsigned int width, unsigned int height);

	mat4 CalculateMatrix();

	void RotateX(float angle);
	void RotateY(float angle);

	void MoveX(float dist);
	void MoveZ(float dist);
};


#endif // Camera_h__