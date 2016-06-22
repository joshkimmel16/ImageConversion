#pragma once

#include "ImageConv.h"
#include "kMeans.h"
#include "bmpimage.h"

using namespace std;

ImageConverter::ImageConverter (string So, string Re, int mode)
{
	SourcePath = So;
	ResultPath = Re;

	bmp = new BMPImage(SourcePath, mode);
	result = new BMPImage(bmp->imageWidth, bmp->imageHeight);

	Converter = new kMeansBmp();
	GConverter = new LuminosityBmp();
}

ImageConverter::~ImageConverter ()
{
	delete bmp;
	delete result;
	delete Converter;
	delete GConverter;
}

string ImageConverter::GetSourcePath ()
{
	return SourcePath;
}

string ImageConverter::GetResultPath ()
{
	return ResultPath;
}

void ImageConverter::ConvertImage (int State)
{
	//checks
	//if source is GS, state must be 0
	if (bmp->bmiHeader.biBitCount == 8 && State != 0)
	{
		throw "Converting a greyscale image to greyscale is meaningless!";
	}

	//if source is BW, don't do conversion
	if (bmp->bmiHeader.biBitCount == 1)
	{
		throw "No conversion should be done on a black and white image!";
	}
	
	//depending on internal state, convert source image to BW or GS and then store in result image
	//GS
	if (State == 1)
	{
		GConverter->LuminosityExecute(bmp, result);
	}
	//BW
	else if (State == 0)
	{
		Converter->kMeansExecute(bmp, result, 2);
	}
	else
	{
		throw "State input must be 1 (greyscale) or 0 (black and white)!";
	}

	//write result image
	WriteImage(false);
}

void ImageConverter::Highlight (vector<unsigned int>& TopLeft, unsigned int width, unsigned int height)
{
	//check for color image
	if (bmp->bmiHeader.biBitCount == 8 || bmp->bmiHeader.biBitCount == 1)
	{
		throw "Cannot highlight an image that is not color!";
	}
	
	BitMapConverter* Conv = new BitMapConverter (ResultPath);

	Conv->ApplyHighlight (bmp, TopLeft, width, height);

	//write source image
	WriteImage (true);

	delete Conv;
}

void ImageConverter::Redact (vector<unsigned int>& TopLeft, unsigned int width, unsigned int height, bool Color)
{
	BitMapConverter* Conv = new BitMapConverter (ResultPath);

	Conv->ApplyRedaction (bmp, TopLeft, width, height, Color);

	//write source image
	WriteImage (true);

	delete Conv;
}

void ImageConverter::Border (vector<unsigned int>& TopLeft, unsigned int width, unsigned int height, unsigned int thickness, RGBColor* color)
{
	//if source is BW or GS, ensure the border is either black or white
	if (bmp->bmiHeader.biBitCount == 8 || bmp->bmiHeader.biBitCount == 1)
	{
		if ((color->rgbBlue == 255 && color->rgbGreen == 255 && color->rgbRed == 255) || (color->rgbBlue == 0 && color->rgbGreen == 0 && color->rgbRed == 0))
		{}
		else
		{
			throw "Must apply a black and white border to a greyscale/black and white image!";
		}
	}
	
	BitMapConverter* Conv = new BitMapConverter (ResultPath);

	Conv->ApplyBorder (bmp, TopLeft, width, height, thickness, color);

	//write source image
	WriteImage (true);

	delete Conv;
}

void ImageConverter::Invert ()
{
	//ensure source is BW
	if (bmp->bmiHeader.biBitCount != 1)
	{
		throw "Cannot invert an image that is not black and white!";
	}
	
	BitMapConverter* Conv = new BitMapConverter (ResultPath);
	Conv->InvertImage(bmp);

	//write result image
	WriteImage (true);
	
	delete Conv;
}

void ImageConverter::WriteImage (bool Original)
{
	//determine whether to write source or result image
	BMPImage* Hold;
	if (Original)
	{
		Hold = bmp;
	}
	else
	{
		Hold = result;
	}
	
	//depending on internal state, write as smaller bmp or color bmp
	BitMapConverter* Conv = new BitMapConverter (ResultPath);
	//BW
	if (Hold->bmiHeader.biBitCount == 1)
	{
		Conv->WriteBitMapToFileBW(Hold);
	}
	//GS
	else if (Hold->bmiHeader.biBitCount == 8)
	{
		Conv->WriteBitMapToFileGrey(Hold);
	}
	//color
	else
	{
		Conv->WriteBitMapToFile(Hold);
	}

	delete Conv;
	delete Hold;
}

void ImageConverter::Rotate (int rotation)
{
	BitMapConverter* Conv = new BitMapConverter (ResultPath);

	//update result image if needed
	if (rotation == 1 || rotation == -1 || rotation == 3 || rotation == -3)
	{
		delete result;
		result = new BMPImage (bmp->imageHeight, bmp->imageWidth);
	}

	Conv->RotateImage(bmp, result, rotation);

	//write result image
	WriteImage (false);

	delete Conv;
}

void ImageConverter::Reflect (bool orientation)
{
	BitMapConverter* Conv = new BitMapConverter (ResultPath);

	Conv->ReflectImage(bmp, result, orientation);

	//write result image
	WriteImage (false);

	delete Conv;
}

void ImageConverter::Smooth (int kernelsize, double sigma)
{
	//ensure that source image is not BW
	if (bmp->bmiHeader.biBitCount == 1)
	{
		throw "Cannot smooth a black and white image!";
	}
	
	BitMapConverter* Conv = new BitMapConverter (ResultPath);

	Conv->SmoothImage(bmp, result, kernelsize, sigma);

	//write result image
	WriteImage (false);

	delete Conv;
}

void ImageConverter::Despeckle (int kernelsize, int threshold)
{
	//ensure that source image is BW
	if (bmp->bmiHeader.biBitCount != 1)
	{
		throw "Cannot despeckle a color or greyscale image!";
	}

	BitMapConverter* Conv = new BitMapConverter (ResultPath);

	Conv->DespeckleImage(bmp, kernelsize, threshold);

	//write result image
	WriteImage (true);

	delete Conv;
}