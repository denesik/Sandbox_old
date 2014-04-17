#ifndef ImageAtlas_h__
#define ImageAtlas_h__

#include "Bitmap.h"
#include <map>
#include <list>

class ImageAtlas
{
private:
	struct ElasticBox
	{
		gm::Rectangle rect;	
		ElasticBox *childSmall;
		ElasticBox *childBig;
		ElasticBox *parent;
	};

	std::string atlasName;

	Bitmap *atlasImage;

	Bitmap::PixelFormat format;
	gm::Size maxSize;
	gm::Size atlasSize;

	int indent;

	std::list<ElasticBox *> rootBoxList;
	std::list<ElasticBox *> emptyBoxList;

	std::map<std::string, gm::Rectangle> atlasMap;

public:
	ImageAtlas(std::string name, Bitmap::PixelFormat format, int indent, const gm::Size &maxSize, const gm::Size &initSize = gm::Size(16, 16));
	ImageAtlas(std::string fileName);
	~ImageAtlas();

	bool Save(std::string dir = "");

	bool Insert(ImageAtlas &atlas);
	bool Insert(const Bitmap &image, std::string name);

	void Clear();

	// Вернуть позицию картинки в атласе
	gm::Rectangle GetImagePos(std::string name) const;

	Bitmap *GetBitmap();
	const gm::Size &GetSize() const;

	std::string GetName() const;

private:
	void CreateEmptyBox(ElasticBox &box, gm::Rectangle &insertRect);
	void PushEmptyBox(ElasticBox *box);
	bool MajorRect(gm::Rectangle &r1, gm::Rectangle &r2);

	void ResizeAtlas(const gm::Size &newSize);

	ImageAtlas(const ImageAtlas& atlas){};

};


#endif // ImageAtlas_h__