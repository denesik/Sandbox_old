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

	mat4 projection;
	mat4 view;

public:
	Camera(void);
	~Camera(void);

	void SetWindowSize(unsigned int width, unsigned int height);

	mat4 CalculateMatrix();

	void RotateX(float angle);
	void RotateY(float angle);

	void MoveX(float dist);
	void MoveY(float dist);
};


#endif // Camera_h__