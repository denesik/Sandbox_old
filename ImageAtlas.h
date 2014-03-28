#ifndef ImageAtlas_h__
#define ImageAtlas_h__

#include "Bitmap.h"

class ImageAtlas
{
private:

	static const int indent = 1;
	struct ElasticBox
	{
		int surfaceWidth, surfaceHeight;		// ������ � ������ "�����"
		int x, y, width, height;				// �������� "����������"
		ElasticBox *childrenBoxSmall;
		ElasticBox *childrenBoxBig;
	};

	Bitmap *image;
	ElasticBox *box;

	Bitmap *imageTmp;
	ElasticBox *boxTmp;

	unsigned int xTmp;
	unsigned int yTmp;

public:
	ImageAtlas(void);
	~ImageAtlas(void);

	bool InsertImage(Bitmap *image, iRect &rect);

	bool Create(unsigned int format, unsigned int width, unsigned int height);

	void Remove();

	Bitmap *GetAtlas()
	{
		return image;
	}

private:
	bool InsertImage();
	void RemoveElasticBox();
	void InitElasticBox();
};




class Atlas
{
private:
	struct ElasticBox
	{
		int x, y, w, h;				
		ElasticBox *childSmall;
		ElasticBox *childBig;
	};

	Bitmap *image;
	ElasticBox *box;

public:
	Atlas(unsigned int format, unsigned int maxSize);
	~Atlas();

	bool Load(std::string fileName);
	bool Save(std::string fileName);

	bool Insert(Atlas *atlas);
	bool Insert(Bitmap *image, std::string name);

	// ������� ogl ��������
	void Create();

	void Clear();

	// ������� ������� �������� � ������ vec4(x, y, h, w);
	i32vec4 GetImagePos(std::string name);

	//Texture *GetTexture();

};


#endif // ImageAtlas_h__