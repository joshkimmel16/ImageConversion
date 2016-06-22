#include <iostream>
#include "bmpimage.h"
#include "Images.h"
#include "kMeans.h"
#include "Luminosity.h"
#include "ImageConv.h"

using namespace std;

//TODO:
//1) Conversion program takes about 2 minutes to run - is this too long??? Think of performance improvements

int main(int argc, char* argv[])
{
	
	string source = "C:\\Users\\josh.kimmel\\Desktop\\ImageTest\\scan001.bmp";
	string result = "C:\\Users\\josh.kimmel\\Desktop\\ImageTest\\Despeckled.bmp";

	//vector<unsigned int> TopLeft;
	//TopLeft.push_back(200);
	//TopLeft.push_back(200);

	//RGBColor* color = new RGBColor;
	//color->rgbBlue = 0;
	//color->rgbGreen = 0;
	//color->rgbRed = 0;

	///// BITMAPCONVERTER TESTS //////

	BitMapConverter* bmp = new BitMapConverter(result);
	BMPImage* img = new BMPImage(source, 0);
	//BMPImage* r = new BMPImage(img->imageWidth, img->imageHeight);
	//kMeansBmp* conv = new kMeansBmp();
	//LuminosityBmp* GConv = new LuminosityBmp();

	//APPLY BORDER
	//bmp->ApplyBorder(img, TopLeft, 500, 700, 2, color);
	
	//APPLY HIGHLIGHT
	//bmp->ApplyHighlight(img, TopLeft, 500, 700);

	//APPLY REDACTION
	//bmp->ApplyRedaction(img, TopLeft, 500, 700, true);

	//INVERT IMAGE
	//bmp->InvertImage(r);

	//REFLECT IMAGE
	//bmp->ReflectImage(img, r, false);

	//ROTATE IMAGE
	//bmp->RotateImage(img, r, 1);

	//CONVERT COLOR TO BW
	//conv->kMeansExecute(img, r, 2);

	//CONVERT COLOR TO GS
	//GConv->LuminosityExecute(img, r);

	//SMOOTH
	//bmp->SmoothImage(img, r, 21, 5.0);

	//DESPECKLE
	bmp->DespeckleImage(img, 11, 3);

	//WRITE COLOR TO FILE
	//bmp->WriteBitMapToFile(r);

	//WRITE GS TO FILE
	//bmp->WriteBitMapToFileGrey(img);

	//WRITE BW TO FILE
	bmp->WriteBitMapToFileBW(img);
	
	delete bmp;
	//delete r;
	delete img;
	//delete conv;
	//delete GConv;
	
	////// IMAGECONVERTER TESTS ///////

	//ImageConverter* Test = new ImageConverter(source, result, true);
	//ImageConverter* Test1 = new ImageConverter(source, result, false);
	
	//IMAGE CONVERSION
	//Test->ConvertImage(0); //BW
	//Test->ConvertImage(1); //GS

	//ANNOTATIONS
	//Test1->Highlight(TopLeft, 500, 700); //HIGHLIGHT
	//Test1->Redact(TopLeft, 500, 700, true); //REDACT
	//Test1->Border(TopLeft, 500, 700, color); //BORDER

	//INVERSION
	//Test->Invert(false);

	//SMOOTH
	//Test->Smooth(5, 1);

	//DESPECKLE
	//Test->Despeckle(5, 1);

	//WRITE
	//Test->WriteImage();
	//Test1->WriteImage();

	//delete Test;
	//delete Test1;

	//delete color;

	//cin.get();
	
	return 0;
}



