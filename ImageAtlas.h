#ifndef ImageAtlas_h__
#define ImageAtlas_h__

#include "Bitmap.h"
#include <map>
#include <list>

class Texture1
{
public:
	unsigned int id;
	gm::Size size;
};

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
		gm::Rectangle rect;	
		ElasticBox *childSmall;
		ElasticBox *childBig;
		ElasticBox *parent;
	};

	Bitmap1 *atlasImage;
	ElasticBox *atlasBox;

	Bitmap1::PixelFormat format;
	gm::Size maxSize;

	std::list<ElasticBox *> emptyBoxList;

	std::map<std::string, gm::Rectangle> atlasMap;

public:
	Atlas(Bitmap1::PixelFormat format, const gm::Size &maxSize, const gm::Size &initSize = gm::Size(16, 16));
	~Atlas();

	bool Load(std::string fileName);
	bool Save(std::string fileName);

	bool Insert(Atlas &atlas);
	bool Insert(const Bitmap1 &image, std::string name);

	void Clear();

	// ������� ������� �������� � ������
	gm::Rectangle GetImagePos(std::string name);

	Bitmap1 *GetBitmap();

	//Texture1 GetTexture();
private:
	void CreateEmptyBox(ElasticBox &box, gm::Rectangle &insertRect);
	void PushEmptyBox(ElasticBox *box);
	bool MajorRect(gm::Rectangle &r1, gm::Rectangle &r2);

};


#endif // ImageAtlas_h__