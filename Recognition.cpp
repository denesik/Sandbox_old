#include "Recognition.h"
#include "Bitmap.h"
#include "Logger.h"



Recognition::Recognition(void)
{
}


Recognition::~Recognition(void)
{
}

void Recognition::Main()
{
	Bitmap image("depp.png");

	gm::Size imageSize = image.GetSize();

	
	Bitmap imageDerg(Bitmap::FORMAT_RGB, imageSize);

	glm::uint8 mat55[5][5] = 
	{
		{2, 4, 5, 4, 2},
		{4, 9, 12, 9, 4},
		{5, 12, 15, 12, 5},
		{4, 9, 12, 9, 4},
		{2, 4, 5, 4, 2}
	};

	for (int i = 2; i < imageSize.height - 2; i++)
		for (int j = 2; j < imageSize.width - 2; j++)
		{
			gm::Color imc = image.GetPixel(gm::Point(j, i));

			int R = 0;
			int G = 0;
			int B = 0;

			for(int k = -2; k < 3; k++)
				for(int t = -2; t < 3; t++)
				{
					gm::Color imc = image.GetPixel(gm::Point(j + t, i + k));
					R += imc.R * mat55[k + 2][t + 2];
					G += imc.G * mat55[k + 2][t + 2];
					B += imc.B * mat55[k + 2][t + 2];
				}
			R = int(float(R) / 159);
			G = int(float(G) / 159);
			B = int(float(B) / 159);
			imageDerg.SetPixel(gm::Point(j, i), gm::Color(R, G, B, 255));
		}

	imageDerg.Save("depp_derp.png");


	Bitmap imageLum(Bitmap::FORMAT_LUMINANCE, imageSize);

	for (int i = 0; i < imageSize.height; i++)
		for (int j = 0; j < imageSize.width; j++)
		{
			gm::Color imc = imageDerg.GetPixel(gm::Point(j, i));

			glm::uint8 c = glm::uint8(imc.R * 0.299f + imc.G * 0.587f + imc.B * 0.114f);

			imageLum.SetPixel(gm::Point(j, i), gm::Color(c, c, c, 255));
		}

	imageLum.Save("depp_lum.png");


	Bitmap imageRob(Bitmap::FORMAT_LUMINANCE, imageSize);

	for (int i = 0; i < imageSize.height - 1; i++)
		for (int j = 0; j < imageSize.width - 1; j++)
		{
			float C1 = 0; float C2 = 0;

			C1 = float(abs(imageLum.GetPixel(gm::Point(j, i)).R - 
				imageLum.GetPixel(gm::Point(j + 1, i + 1)).R));

			C2 = float(abs(imageLum.GetPixel(gm::Point(j + 1, i)).R - 
				imageLum.GetPixel(gm::Point(j, i + 1)).R));

			int c = int(sqrt( C1 * C1 + C2 * C2));

			imageRob.SetPixel(gm::Point(j, i), gm::Color(c, c, c, 255));
		}
	
	imageRob.Save("depp_rob.png");

}
