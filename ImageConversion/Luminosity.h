#pragma once

#include "bmpimage.h"
#include <vector>

using namespace std;

//data structure for bitmap
class LuminosityBmp
{
private:
    unsigned int ComputeAverage (unsigned int r, unsigned int g, unsigned int b); //compute the new pixel average for the given pixel
public:
	void LuminosityExecute (BMPImage* OldImage, BMPImage* NewImage); //run the luminosity algorithm to convert a color image to greyscale
};