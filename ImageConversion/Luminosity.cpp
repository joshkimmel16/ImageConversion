#pragma once

#include "Luminosity.h"
#include "bmpimage.h"

using namespace std;

unsigned int LuminosityBmp::ComputeAverage (unsigned int r, unsigned int g, unsigned int b)
{
	unsigned int sum = 0;

	sum += (unsigned int)(0.21*((double)r) + 0.72*((double)g) + 0.07*((double)b));

	return sum;
}

void LuminosityBmp::LuminosityExecute (BMPImage* OldImage, BMPImage* NewImage)
{
	//convert image data
	unsigned int temp;
	for (int i = 0; i < OldImage->imageHeight; i++)
	{
		for (int j = 0; j < OldImage->imageWidth; j++)
		{
			temp = ComputeAverage(OldImage->rgb[i][j].rgbRed, OldImage->rgb[i][j].rgbGreen, OldImage->rgb[i][j].rgbBlue);

			NewImage->rgb[i][j].rgbRed = temp;
			NewImage->rgb[i][j].rgbGreen = temp;
			NewImage->rgb[i][j].rgbBlue = temp;
		}
	}

	//computations for new header and info
	unsigned short bits = 8; //new bits per pixel
	unsigned int padnum = (4-(OldImage->imageWidth)%4)%4; //byte row padding
	unsigned int ImageSize = (OldImage->imageWidth + padnum)*OldImage->imageHeight; //new size of image data in bytes
	long FileSize = ImageSize + 54 + 1024; //new total file size in bytes (including color palette)
	long NewOffset = OldImage->header.bfOffBits + 1024; //new offset for start of image data

	//copy all header and info
	NewImage->header.bfType = OldImage->header.bfType;
	NewImage->header.bfSize = FileSize;
    NewImage->header.bfReserved1 = OldImage->header.bfReserved1;
    NewImage->header.bfReserved2 = OldImage->header.bfReserved2;
	NewImage->header.bfOffBits = NewOffset;

    NewImage->bmiHeader.biSize = OldImage->bmiHeader.biSize;
    NewImage->bmiHeader.biWidth = OldImage->bmiHeader.biWidth;
    NewImage->bmiHeader.biHeight = OldImage->bmiHeader.biHeight;
    NewImage->bmiHeader.biPlanes = OldImage->bmiHeader.biPlanes ;
	NewImage->bmiHeader.biBitCount = bits;
    NewImage->bmiHeader.biCompression = OldImage->bmiHeader.biCompression;
	NewImage->bmiHeader.biSizeImage = ImageSize;
    NewImage->bmiHeader.biXPelsPerMeter = OldImage->bmiHeader.biXPelsPerMeter;
    NewImage->bmiHeader.biYPelsPerMeter = OldImage->bmiHeader.biYPelsPerMeter;
    NewImage->bmiHeader.biClrUsed = OldImage->bmiHeader.biClrUsed;
    NewImage->bmiHeader.biClrImportant = OldImage->bmiHeader.biClrImportant;
}