// #ifndef GraphicText_h__
// #define GraphicText_h__
// 
// #include <string>
// #include <vector>
// #include "BufferArray.h"
// #include "Rectangle.h"
// #include "Font.h"
// 
// class GraphicText
// {
// private:
// 	std::vector<uint32_t> utf32text;
// 
// //	BufferArrayRoot buffer;
// 
// 	float constraintWidth;
// 	float constraintHeight;
// 
// 	float x, y, z;
// 
// 	Rectangle geometryRectangle;
// 	FontTexture fontTexture;
// 
// public:
// 	GraphicText(void);
// 	~GraphicText(void);
// 
// 	void SetPos(const vec3 &pos);
// 
// 	void SetText(std::string text);
// 
// 	void Draw();
// 
// private:
// 	void CreateBuffer();
// 
// };
// 
// 
// #endif // GraphicText_h__