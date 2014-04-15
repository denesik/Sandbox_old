#ifndef Render_h__
#define Render_h__

#include <glew.h>
#include <glfw3.h>
#include "GameMath.h"
#include <vector>
#include "BufferArray.h"
#include "Bitmap.h"
#include <memory>

using namespace glm;

struct Texture
{
	unsigned int id;
	gm::Size size;

	Texture()
		: id(0), size()
	{};
	Texture(unsigned int _id, const gm::Size &_size)
		: id(_id), size(_size)
	{};

};

std::shared_ptr<Texture> CreateTexture(const Bitmap &bitmap, bool smoothing);

class Render
{
private:
	mat4 orthoProjection;

	unsigned int width;
	unsigned int height;

public:
	Render(void);
	~Render(void);

	bool Init();

	void SetWindowSize(unsigned int width, unsigned int height);

	mat4 GetOrthoProjection();

};


#endif // Render_h__