#include "ImageAtlas.h"
#include "Logger.h"
#include <value.h>
#include <writer.h>



ImageAtlas::ImageAtlas(void)
{
	image = nullptr;
	box = new ElasticBox;
	InitElasticBox();
}


ImageAtlas::~ImageAtlas(void)
{
	Remove();
	if(box)
	{
		delete box;
		box = nullptr;
	}
}

bool ImageAtlas::InsertImage( Bitmap *_image, iRect &rect )
{
	boxTmp = box;
	imageTmp = _image;
	bool result = InsertImage();

	rect.x = xTmp;
	rect.y = yTmp;
	rect.w = _image->GetWidth();
	rect.h = _image->GetHeight();

	return result;
}

bool ImageAtlas::InsertImage()
{
	ElasticBox *boxTmpParent = boxTmp;

	if(boxTmp->surfaceHeight == 0 && boxTmp->surfaceWidth == 0)
	{
		i32vec2 dstPoint;

		xTmp = dstPoint.x = boxTmp->x;
		yTmp = dstPoint.y = boxTmp->y;

		// Копируем всю поверхность
		bool blitResult = image->Blit(&dstPoint, nullptr, imageTmp);
		if(!blitResult)
		{
			//LOG(LOG_ERROR, "TextureAtlas. Image не добавлена.");
			return false;
		}

		unsigned int w1 = boxTmp->surfaceWidth = imageTmp->GetWidth();
		unsigned int h1 = boxTmp->surfaceHeight = imageTmp->GetHeight();

		boxTmp->childrenBoxSmall = new ElasticBox;
		boxTmp->childrenBoxSmall->surfaceHeight = 0;
		boxTmp->childrenBoxSmall->surfaceWidth = 0;
		boxTmp->childrenBoxSmall->childrenBoxBig = nullptr;
		boxTmp->childrenBoxSmall->childrenBoxSmall = nullptr;
		boxTmp->childrenBoxBig = new ElasticBox;
		boxTmp->childrenBoxBig->surfaceHeight = 0;
		boxTmp->childrenBoxBig->surfaceWidth = 0;
		boxTmp->childrenBoxBig->childrenBoxBig = nullptr;
		boxTmp->childrenBoxBig->childrenBoxSmall = nullptr;
		// Правый нижний прямоугольник
		//double d1 = sqrt(double(h1 * h1) + double((boxTmp->width - w1) * (boxTmp->width - w1)));
		// Левый верхний прямоугольник
		//double d2 = sqrt(double((boxTmp->height - h1) * (boxTmp->height - h1)) + double(w1 * w1));

		// Правый нижний прямоугольник
		double d1 = sqrt(double(boxTmp->height * boxTmp->height) + double((boxTmp->width - w1 - indent) * (boxTmp->width - w1 - indent)));
		// Левый верхний прямоугольник
		double d2 = sqrt(double((boxTmp->height - h1 - indent) * (boxTmp->height - h1 - indent)) + double(boxTmp->width * boxTmp->width));

		if(d1 < d2)
		{
			// Правый нижний прямоугольник - маленький
			boxTmp->childrenBoxSmall->height = h1;
			boxTmp->childrenBoxSmall->width = boxTmp->width - w1 - indent;
			boxTmp->childrenBoxSmall->x = boxTmp->x + w1 + indent;
			boxTmp->childrenBoxSmall->y = boxTmp->y;

			boxTmp->childrenBoxBig->x = boxTmp->x;
			boxTmp->childrenBoxBig->y = boxTmp->y + h1 + indent;
			boxTmp->childrenBoxBig->width = boxTmp->width;
			boxTmp->childrenBoxBig->height = boxTmp->height - h1 - indent;
		}
		else
		{
			// Левый верхний прямоугольник - маленький
			boxTmp->childrenBoxSmall->height = boxTmp->height - h1 - indent;
			boxTmp->childrenBoxSmall->width = w1;
			boxTmp->childrenBoxSmall->x = boxTmp->x;
			boxTmp->childrenBoxSmall->y = boxTmp->y + h1 + indent;

			boxTmp->childrenBoxBig->x = boxTmp->x + w1 + indent;
			boxTmp->childrenBoxBig->y = boxTmp->y;
			boxTmp->childrenBoxBig->width = boxTmp->width - w1 - indent;
			boxTmp->childrenBoxBig->height = boxTmp->height;
		}
	}
	else
	{
		int w1 = imageTmp->GetWidth();
		int h1 = imageTmp->GetHeight();
		// Попытаемся уместиться в маленькую площадь.
		if(boxTmp->childrenBoxSmall->height >= h1 && boxTmp->childrenBoxSmall->width >= w1)
		{
			boxTmp = boxTmp->childrenBoxSmall;
			if(InsertImage())
			{
				return true;
			}
			else
				boxTmp = boxTmpParent;
		}

		if(boxTmp->childrenBoxBig->height >= h1 && boxTmp->childrenBoxBig->width >= w1)
		{
			boxTmp = boxTmp->childrenBoxBig;
			return InsertImage();
		}
		return false;
	}

	return true;

}

bool ImageAtlas::Create( unsigned int format, unsigned int width, unsigned int height )
{
	if(!box)
	{
		box = new ElasticBox();
		InitElasticBox();
	}
	image = new Bitmap;
	image->Generate(format , width, height, 0x00000000);
	box->x = 0;
	box->y = 0;
	box->width = width;
	box->height = height;
	box->surfaceHeight = 0;
	box->surfaceWidth = 0;
	box->childrenBoxBig = nullptr;
	box->childrenBoxSmall = nullptr;
	return true;
}

void ImageAtlas::Remove()
{
	if(image != nullptr)
	{
		delete image;
		image = nullptr;
	}
	if(box)
	{
		ElasticBox *boxParent = box;
		if(boxParent->childrenBoxBig)
		{
			box = boxParent->childrenBoxBig;
			RemoveElasticBox();
		}
		if(boxParent->childrenBoxSmall)
		{
			box = boxParent->childrenBoxSmall;
			RemoveElasticBox();
		}

		box = boxParent;
		InitElasticBox();
	}
}

void ImageAtlas::RemoveElasticBox()
{
	ElasticBox *boxParent = box;
	if(boxParent->childrenBoxBig)
	{
		box = boxParent->childrenBoxBig;
		RemoveElasticBox();
	}
	if(boxParent->childrenBoxSmall)
	{
		box = boxParent->childrenBoxSmall;
		RemoveElasticBox();
	}

	delete boxParent;
	boxParent = nullptr;
}

void ImageAtlas::InitElasticBox()
{
	box->x = 0;
	box->y = 0;
	box->width = 0;
	box->height = 0;
	box->surfaceHeight = 0;
	box->surfaceWidth = 0;
	box->childrenBoxBig = nullptr;
	box->childrenBoxSmall = nullptr;
}




Atlas::Atlas( std::string name, Bitmap1::PixelFormat _format, const gm::Size &_maxSize, const gm::Size &initSize )
{
	atlasName = name;
	format = _format;
	maxSize = _maxSize;
	atlasSize = initSize;

	gm::Size boxSize = initSize;
	boxSize.Clamp(gm::Size(), maxSize);

	ElasticBox *atlasBox = new ElasticBox();
	atlasBox->rect = gm::Rectangle(gm::Point(),boxSize);
	atlasBox->childSmall = nullptr;
	atlasBox->childBig = nullptr;
	atlasBox->parent = nullptr;

	rootBoxList.push_back(atlasBox);
	emptyBoxList.push_back(atlasBox);

	atlasImage = new Bitmap1(format, atlasBox->rect.size);
}

Atlas::~Atlas()
{
	delete atlasImage;
	atlasImage = nullptr;
	//рекурсивно удалить box
}

bool Atlas::Load( std::string fileName )
{
	return true;
}

bool Atlas::Save()
{
	std::ofstream configFile(atlasName + ".json");

	Json::Value root;

	root["Name"] = atlasName;
	root["Size"]["Width"] = atlasSize.width;
	root["Size"]["Height"] = atlasSize.height;
	root["MaxSize"]["Width"] = maxSize.width;
	root["MaxSize"]["Height"] = maxSize.height;

	//root["RootBoxList"] = 

	Json::Value rootBoxListVal(Json::arrayValue);
	Json::Value emptyBoxListVal(Json::arrayValue);
	//rootBoxListVal.append(Json::Value(1));

	std::list<ElasticBox *> stackBox;
	for(auto i = rootBoxList.begin(); i != rootBoxList.end(); i++)
	{
		stackBox.push_back(*i);
	}

	std::vector<ElasticBox *> vectorBox;

	while(stackBox.begin() != stackBox.end())
	{
		ElasticBox *box = stackBox.back();
		stackBox.pop_back();

		vectorBox.push_back(box);

		if(box->childBig)
			stackBox.push_back(box->childBig);
		if(box->childSmall)
			stackBox.push_back(box->childSmall);
	}

	for(auto i = rootBoxList.begin(); i != rootBoxList.end(); i++)
	{
		for(unsigned int j = 0; j < vectorBox.size(); j++)
		{
			if(vectorBox[j] == *i)
			{
				rootBoxListVal.append(Json::Value(j));
				break;
			}
		}
	}

	for(auto i = emptyBoxList.begin(); i != emptyBoxList.end(); i++)
	{
		for(unsigned int j = 0; j < vectorBox.size(); j++)
		{
			if(vectorBox[j] == *i)
			{
				emptyBoxListVal.append(Json::Value(j));
				break;
			}
		}
	}

	stackBox.clear();
	for(auto i = rootBoxList.begin(); i != rootBoxList.end(); i++)
	{
		stackBox.push_back(*i);
	}

	while(stackBox.begin() != stackBox.end())
	{
		ElasticBox *box = stackBox.back();
		stackBox.pop_back();

		for(unsigned int j = 0; j < vectorBox.size(); j++)
		{
			if(vectorBox[j] == box)
			{
				Json::Value rectVal(Json::arrayValue);
				rectVal.append(Json::Value(box->rect.x));
				rectVal.append(Json::Value(box->rect.y));
				rectVal.append(Json::Value(box->rect.w));
				rectVal.append(Json::Value(box->rect.h));
				root["ElasticBox"][j]["rect"] = rectVal;

				int childSmall = -1;
				for(unsigned int k = 0; k < vectorBox.size(); k++)
				{
					if(vectorBox[k] == box->childSmall)
					{
						childSmall = k;
						break;
					}
				}
				root["ElasticBox"][j]["childSmall"] = childSmall;

				int childBig = -1;
				for(unsigned int k = 0; k < vectorBox.size(); k++)
				{
					if(vectorBox[k] == box->childBig)
					{
						childBig = k;
						break;
					}
				}
				root["ElasticBox"][j]["childBig"] = childBig;

				int parent = -1;
				for(unsigned int k = 0; k < vectorBox.size(); k++)
				{
					if(vectorBox[k] == box->parent)
					{
						parent = k;
						break;
					}
				}
				root["ElasticBox"][j]["parent"] = parent;

				break;
			}
		}

		if(box->childBig)
			stackBox.push_back(box->childBig);
		if(box->childSmall)
			stackBox.push_back(box->childSmall);
	}

	root["RootBoxList"] = rootBoxListVal;
	root["EmptyBoxList"] = emptyBoxListVal;

	atlasImage->Save(atlasName + ".png");

	configFile << root;
	configFile.close();

	return true;
}

bool Atlas::Insert( Atlas &atlas )
{
	return true;
}

bool Atlas::Insert( const Bitmap1 &image, std::string name )
{
	// Проверяем, есть ли битмап с таким именем
	if(atlasMap.find(name) != atlasMap.end())
	{
		LOG_WARNING("Битмап %s уже имеется в атласе.", name);
		return false;
	}

	const gm::Size &imageSize = image.GetSize();


	for(auto j = emptyBoxList.begin(); j != emptyBoxList.end(); j++)
	{
		gm::Rectangle bitmapRect((*j)->rect.pos, imageSize);

		if((*j)->rect.Contains(bitmapRect))
		{
			ElasticBox &box = **j;
			gm::Rectangle insertRect = atlasImage->Insert(image, box.rect.pos);
			atlasMap[name] = insertRect;

			// Удаляем бокс из списка пустых боксов
			emptyBoxList.erase(j);

			// Выбираем бокс
			// Вставляем пустые боксы в список в упорядоченном виде по возрастанию
			CreateEmptyBox(box, insertRect);

			return true;
		}
	}

	// Не смогли вставить битмап.
	// Размер атласа равен максимальному. Выход
	if(atlasSize == maxSize)
	{
		LOG_WARNING("Невозможно вставить битмап в атлас. Достигнут максимальный размер атласа.");
		return false;
	}
	// Увеличиваем размер атласа.

	ResizeAtlas(gm::Size(atlasSize.width * 2, atlasSize.height * 2));

	return Insert(image, name);
}

void Atlas::Clear()
{

}

gm::Rectangle Atlas::GetImagePos( std::string name )
{
	return gm::Rectangle();
}

void Atlas::CreateEmptyBox( ElasticBox &box, gm::Rectangle &insertRect )
{

	if(box.rect == insertRect)
		return;

	// Создаем 4 ректангла
	gm::Rectangle RectLeftSmall(gm::FromLTRB(box.rect.Left(), insertRect.Bottom(), insertRect.Right(), box.rect.Bottom()));
	gm::Rectangle RectRightBig(gm::FromLTRB(insertRect.Right(), insertRect.Top(), box.rect.Right(), box.rect.Bottom()));

	if(RectRightBig.IsAreaNull())
	{
		box.childSmall = new ElasticBox;
		box.childSmall->childBig = nullptr;
		box.childSmall->childSmall = nullptr;
		box.childSmall->parent = &box;
		box.childSmall->rect = RectLeftSmall;

		PushEmptyBox(box.childSmall);
		return;
	}

	gm::Rectangle RectTopSmall(gm::FromLTRB(insertRect.Right(), box.rect.Top(), box.rect.Right(), insertRect.Bottom()));
	gm::Rectangle RectBottomBig(gm::FromLTRB(insertRect.Left(), insertRect.Bottom(), box.rect.Right(), box.rect.Bottom()));

	if(RectBottomBig.IsAreaNull())
	{
		box.childSmall = new ElasticBox;
		box.childSmall->childBig = nullptr;
		box.childSmall->childSmall = nullptr;
		box.childSmall->parent = &box;
		box.childSmall->rect = RectTopSmall;

		PushEmptyBox(box.childSmall);
		return;
	}

	// Находим маленький бокс в первом случае
	gm::Rectangle &rs1 = RectLeftSmall;
	gm::Rectangle &rb1 = RectRightBig;
	if(MajorRect(RectLeftSmall, RectRightBig))
	{
		rs1 = RectRightBig;
		rb1 = RectLeftSmall;
	}
	// Находим маленький бокс во втором случае
	gm::Rectangle &rs2 = RectTopSmall;
	gm::Rectangle &rb2 = RectBottomBig;
	if(MajorRect(RectTopSmall, RectBottomBig))
	{
		rs2 = RectBottomBig;
		rb2 = RectTopSmall;
	}


	gm::Rectangle &rectBig = rb1;
	gm::Rectangle &rectSmall = rs1;
	// Если самый маленький бокс во втором случае
	if(MajorRect(rs1, rs2))
	{
		// первый случай
		rectBig = rb2;
		rectSmall = rs2;
	}

	box.childBig = new ElasticBox;
	box.childBig->childBig = nullptr;
	box.childBig->childSmall = nullptr;
	box.childBig->parent = &box;
	box.childBig->rect = rectBig;

	box.childSmall = new ElasticBox;
	box.childSmall->childBig = nullptr;
	box.childSmall->childSmall = nullptr;
	box.childSmall->parent = &box;
	box.childSmall->rect = rectSmall;

	PushEmptyBox(box.childBig);
	PushEmptyBox(box.childSmall);

}

void Atlas::PushEmptyBox( ElasticBox *insertBox )
{

	if(emptyBoxList.begin() == emptyBoxList.end())
	{
		emptyBoxList.push_front(insertBox);
		return;
	}

	for(auto i = emptyBoxList.begin(); i != emptyBoxList.end(); i++)
	{
		ElasticBox &box = **i;
		// Если текущий бокс больше вставляемого
		if(MajorRect(box.rect, insertBox->rect))
		{
			// Вставляем перед ним
			emptyBoxList.insert(i, insertBox);
			return;
		}
	}

	emptyBoxList.push_back(insertBox);
}

bool Atlas::MajorRect( gm::Rectangle &r1, gm::Rectangle &r2 )
{
	return r1.w * r1.w + r1.h * r1.h > r2.w * r2.w + r2.h * r2.h;
}

Bitmap1 * Atlas::GetBitmap()
{
	return atlasImage;
}

void Atlas::ResizeAtlas( const gm::Size &newSize )
{
	gm::Size boxSize = newSize;
	boxSize.Clamp(atlasSize, maxSize);

	gm::Rectangle atlasRect(gm::Point(), atlasSize);
	gm::Rectangle newAtlasRect(gm::Point(), boxSize);

	gm::Rectangle RectLeftSmall(gm::FromLTRB(newAtlasRect.Left(), atlasRect.Bottom(), atlasRect.Right(), newAtlasRect.Bottom()));
	gm::Rectangle RectRightBig(gm::FromLTRB(atlasRect.Right(), atlasRect.Top(), newAtlasRect.Right(), newAtlasRect.Bottom()));

	// Если площадь не нулевая
	if(!RectLeftSmall.IsAreaNull())
	{
		ElasticBox *atlasBox = new ElasticBox();
		atlasBox->rect = RectLeftSmall;
		atlasBox->childSmall = nullptr;
		atlasBox->childBig = nullptr;
		atlasBox->parent = nullptr;

		rootBoxList.push_back(atlasBox);
		emptyBoxList.push_back(atlasBox);
	}

	if(!RectRightBig.IsAreaNull())
	{
		ElasticBox *atlasBox = new ElasticBox();
		atlasBox->rect = RectRightBig;
		atlasBox->childSmall = nullptr;
		atlasBox->childBig = nullptr;
		atlasBox->parent = nullptr;

		rootBoxList.push_back(atlasBox);
		emptyBoxList.push_back(atlasBox);
	}

	atlasSize = boxSize;
	atlasImage->Resize(atlasSize);
}
