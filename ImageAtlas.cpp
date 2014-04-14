#include "ImageAtlas.h"
#include "Logger.h"
#include <value.h>
#include <writer.h>
#include <reader.h>

ImageAtlas::ImageAtlas( std::string name, Bitmap::PixelFormat _format, int _indent, const gm::Size &_maxSize, const gm::Size &initSize )
{
	atlasName = name;
	format = _format;
	indent = _indent;
	maxSize = _maxSize;
	atlasSize = initSize;
	atlasImage = nullptr;

	gm::Size boxSize = initSize;
	boxSize.Clamp(gm::Size(), maxSize);

	ElasticBox *atlasBox = new ElasticBox();
	atlasBox->rect = gm::Rectangle(gm::Point(), boxSize);
	atlasBox->childSmall = nullptr;
	atlasBox->childBig = nullptr;
	atlasBox->parent = nullptr;

	rootBoxList.push_back(atlasBox);
	emptyBoxList.push_back(atlasBox);

	atlasImage = new Bitmap(format, atlasBox->rect.size);
}

ImageAtlas::ImageAtlas( std::string fileName )
{
	atlasName = "";
	format = Bitmap::FORMAT_NULL;
	indent = 0;
	atlasImage = nullptr;

	std::ifstream configFile(fileName);

	if (!configFile.is_open()) 
	{
		LOG_WARNING("Не корректный формат атласа %s.", fileName);
		ImageAtlas(fileName, Bitmap::FORMAT_RGBA, 0, gm::Size(1024, 1024));
		return;
	}

	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse( configFile, root );
	if ( !parsingSuccessful )
	{
		LOG_WARNING("Загрузка атласа. Ошибка в структуре конфигурационного файла %s. %s", fileName, reader.getFormatedErrorMessages());
		configFile.close();
		ImageAtlas(fileName, Bitmap::FORMAT_RGBA, 0, gm::Size(1024, 1024));
		return;
	}

	atlasName = root["Name"].asString();
	indent = root["Indent"].asInt();
	atlasSize.width = root["Size"]["Width"].asInt();
	atlasSize.height = root["Size"]["Height"].asInt();
	maxSize.width = root["MaxSize"]["Width"].asInt();
	maxSize.height = root["MaxSize"]["Height"].asInt();

	std::vector<ElasticBox *> vectorBox;
	const Json::Value vectorBoxVal = root["ElasticBox"];

	for(unsigned int i = 0; i < vectorBoxVal.size(); i++)
	{
		ElasticBox *box = new ElasticBox;
		const Json::Value rectVal = vectorBoxVal[i]["rect"];
		box->rect.x = rectVal[0U].asInt();
		box->rect.y = rectVal[1U].asInt();
		box->rect.w = rectVal[2U].asInt();
		box->rect.h = rectVal[3U].asInt();

		box->childBig = nullptr;
		box->childSmall = nullptr;
		box->parent = nullptr;

		vectorBox.push_back(box);
	}

	for(unsigned int i = 0; i < vectorBoxVal.size(); i++)
	{
		ElasticBox &box = *vectorBox[i];
		
		int childBig = vectorBoxVal[i]["childBig"].asInt();
		if(childBig > -1 && childBig < (int)vectorBoxVal.size())
		{
			box.childBig = vectorBox[childBig];
		}

		int childSmall = vectorBoxVal[i]["childSmall"].asInt();
		if(childSmall > -1 && childSmall < (int)vectorBoxVal.size())
		{
			box.childSmall = vectorBox[childSmall];
		}

		int parent = vectorBoxVal[i]["parent"].asInt();
		if(parent > -1 && parent < (int)vectorBoxVal.size())
		{
			box.parent = vectorBox[parent];
		}
	}


	const Json::Value rootBoxListVal = root["RootBoxList"];
	const Json::Value emptyBoxListVal = root["EmptyBoxList"];

	for(unsigned int i = 0; i < rootBoxListVal.size(); i++)
	{
		int rootBoxVal = rootBoxListVal[i].asInt();
		if(rootBoxVal > -1 && rootBoxVal < (int)vectorBoxVal.size())
		{
			rootBoxList.push_back(vectorBox[rootBoxVal]);
		}
	}

	for(unsigned int i = 0; i < emptyBoxListVal.size(); i++)
	{
		int emptyBoxVal = emptyBoxListVal[i].asInt();
		if(emptyBoxVal > -1 && emptyBoxVal < (int)vectorBoxVal.size())
		{
			emptyBoxList.push_back(vectorBox[emptyBoxVal]);
		}
	}

	const Json::Value AtlasMapVal = root["atlasMap"];
	for(unsigned int i = 0; i < AtlasMapVal.size(); i++)
	{
		std::string nameRect = AtlasMapVal[i]["name"].asString();

		Json::Value rectAtlasMapVal = AtlasMapVal[i]["rect"];
		gm::Rectangle rect;
		rect.x = rectAtlasMapVal[0U].asInt();
		rect.y = rectAtlasMapVal[1U].asInt();
		rect.w = rectAtlasMapVal[2U].asInt();
		rect.h = rectAtlasMapVal[3U].asInt();
		atlasMap[nameRect] = rect;
	}

	std::string imgFileName = root["ImageFileName"].asString();
	atlasImage = new Bitmap(imgFileName);
	format = atlasImage->GetFormat();
	configFile.close();
}

ImageAtlas::~ImageAtlas()
{
	if(atlasImage)
	{
		delete atlasImage;
		atlasImage = nullptr;
	}
	//рекурсивно удалить box
}

bool ImageAtlas::Save(std::string dir)
{
	std::string fileDir = dir;
	if(dir.size() > 0)
		fileDir += "/";

	std::ofstream configFile(fileDir + atlasName + ".json");

	Json::Value root;

	root["Name"] = atlasName;
	root["Indent"] = indent;
	root["Size"]["Width"] = atlasSize.width;
	root["Size"]["Height"] = atlasSize.height;
	root["MaxSize"]["Width"] = maxSize.width;
	root["MaxSize"]["Height"] = maxSize.height;

	Json::Value rootBoxListVal(Json::arrayValue);
	Json::Value emptyBoxListVal(Json::arrayValue);

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

	unsigned int indexAtlasMap = 0;
	for(auto i = atlasMap.begin(); i != atlasMap.end(); i++)
	{
		root["atlasMap"][indexAtlasMap]["name"] = (*i).first;

		Json::Value rectAtlasMapVal(Json::arrayValue);
		rectAtlasMapVal.append(Json::Value((*i).second.x));
		rectAtlasMapVal.append(Json::Value((*i).second.y));
		rectAtlasMapVal.append(Json::Value((*i).second.w));
		rectAtlasMapVal.append(Json::Value((*i).second.h));

		root["atlasMap"][indexAtlasMap]["rect"] = rectAtlasMapVal;
		indexAtlasMap++;
	}

	root["ImageFileName"] = fileDir + atlasName + ".png";

	atlasImage->Save(fileDir + atlasName + ".png");

	configFile << root;
	configFile.close();

	return true;
}

bool ImageAtlas::Insert( ImageAtlas &atlas )
{
	return true;
}

bool ImageAtlas::Insert( const Bitmap &image, std::string name )
{
	// Проверяем, есть ли битмап с таким именем
	if(atlasMap.find(name) != atlasMap.end())
	{
	//	LOG_WARNING("Битмап %s уже имеется в атласе.", name.c_str());
		return false;
	}

	gm::Size indentSize(indent, indent);

	const gm::Size &imageSize = image.GetSize() + indentSize;


	for(auto j = emptyBoxList.begin(); j != emptyBoxList.end(); j++)
	{
		gm::Rectangle bitmapRect((*j)->rect.pos, imageSize);

		if((*j)->rect.Contains(bitmapRect))
		{
			ElasticBox &box = **j;
			gm::Rectangle insertRect = atlasImage->Insert(image, box.rect.pos);
			atlasMap[name] = insertRect;
			insertRect.Inflate(indentSize);

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

void ImageAtlas::Clear()
{

}

gm::Rectangle ImageAtlas::GetImagePos( std::string name )
{
	auto it = atlasMap.find(name);
	if(it != atlasMap.end())
	{
		return (*it).second;
	}
	return gm::Rectangle();
}

void ImageAtlas::CreateEmptyBox( ElasticBox &box, gm::Rectangle &insertRect )
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

void ImageAtlas::PushEmptyBox( ElasticBox *insertBox )
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

bool ImageAtlas::MajorRect( gm::Rectangle &r1, gm::Rectangle &r2 )
{
	return r1.w * r1.w + r1.h * r1.h > r2.w * r2.w + r2.h * r2.h;
}

Bitmap * ImageAtlas::GetBitmap()
{
	return atlasImage;
}

void ImageAtlas::ResizeAtlas( const gm::Size &newSize )
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

const gm::Size & ImageAtlas::GetSize() const
{
	return atlasSize;
}

std::string ImageAtlas::GetName() const
{
	return atlasName;
}
