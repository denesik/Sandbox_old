#ifndef Camera_h__
#define Camera_h__

#include "GameMath.h"

class Camera
{
public:
	mat4 projection;
	mat4 view;

public:
	Camera(void);
	~Camera(void);

	void Set();
};


#endif // Camera_h__