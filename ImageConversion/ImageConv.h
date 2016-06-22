#pragma once

#define DllExport   __declspec( dllexport )

#include "bmpimage.h"
#include "kMeans.h"
#include "Luminosity.h"
#include <iostream>
#include <string>

using namespace std;

class ImageConverter
{
private:
	string SourcePath; //source bitmap
	string ResultPath; //path to result bitmap
	BMPImage* bmp; //internal bitmap used for manipulation
	BMPImage* result; //result image to be used after conversion
	kMeansBmp* Converter; //object to convert internal bitmap to black and white
	LuminosityBmp* GConverter; //object to convert internal bitmap to greyscale

public:
	ImageConverter (string So, string Re, int mode); //constructor taking source and result paths
	~ImageConverter (); //custom destructor
	string GetSourcePath (); //accessor for source path
	string GetResultPath (); //accessor for result path
	void ConvertImage (int State); //public method to convert and image based on parameters (0=BW, 1=GS)
	void Highlight (vector<unsigned int>& TopLeft, unsigned int width, unsigned int height); //apply highlight to the source image
	void Redact (vector<unsigned int>& TopLeft, unsigned int width, unsigned int height, bool Color); //apply redaction to the source image
	void Border (vector<unsigned int>& TopLeft, unsigned int width, unsigned int height, unsigned int thickness, RGBColor* color); //apply border around given region of source image
	void Invert (); //invert the target image
	void Rotate (int rotation); //rotate the source image and write to target
	void Reflect (bool orientation); //reflect the source image and write to target
	void Smooth (int kernelsize, double sigma); //smooth the target image using gaussian filter
	void Despeckle (int kernelsize, int threshold); //despeckle BW image
	void WriteImage (bool Original); //write bitmap back to file
};


