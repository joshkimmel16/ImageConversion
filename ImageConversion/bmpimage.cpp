#pragma once

#include "bmpimage.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

//TODO:

BMPImage::BMPImage (string filename, int mode)
{
	switch (mode)
	{
	case 0:
		ReadBWBmp (filename);
		break;
	case 1:
		ReadGreyBmp (filename);
		break;
	case 2:
		ReadColorBmp (filename);
		break;
	default:
		throw "Invalid mode!";
	}
}

BMPImage::BMPImage(string filename)
{
	pFile.open(filename, ios::in | ios::binary);

	 if (pFile.is_open())
	 { 
		 //get header
		 header.bfType      = read_u16();
		 header.bfSize      = read_u32();
		 header.bfReserved1 = read_u16();
		 header.bfReserved2 = read_u16();
		 header.bfOffBits   = read_u32();

		 //get info
		 bmiHeader.biSize          = read_u32();
		 bmiHeader.biWidth         = read_s32();
		 bmiHeader.biHeight        = read_s32();
		 bmiHeader.biPlanes        = read_u16();
		 bmiHeader.biBitCount      = read_u16();
		 bmiHeader.biCompression   = read_u32();
		 bmiHeader.biSizeImage     = read_u32();
		 bmiHeader.biXPelsPerMeter = read_s32();
		 bmiHeader.biYPelsPerMeter = read_s32();
		 bmiHeader.biClrUsed       = read_u32();
		 bmiHeader.biClrImportant  = read_u32();

		 int padnum = (4-(bmiHeader.biWidth*3)%4)%4; //compute number of padding bytes per row

		 //store image data
		 rgb = new RGBColor*[bmiHeader.biHeight];
		 for (int i = 0; i < bmiHeader.biHeight; i++)
				rgb[i] = new RGBColor[bmiHeader.biWidth];

		 for (int i = 0; i < bmiHeader.biHeight; i++) {
			 for (int j = 0; j < bmiHeader.biWidth; j++) {
				rgb[i][j].rgbBlue = pFile.get();
				rgb[i][j].rgbGreen = pFile.get();
				rgb[i][j].rgbRed = pFile.get();
			 }

			 //read padding bytes
			 for (int a = 0; a < padnum; a++)
			 {
				 pFile.get();
			 }
		  }

		 //set object properties
		 imageWidth = bmiHeader.biWidth;
		 imageHeight = bmiHeader.biHeight;
	 }
	 else
	 {
		 throw "Invalid file path!";
	 }
	 
     pFile.close();
}

BMPImage::BMPImage(int width, int height)
{
	 //set dummy header
	 header.bfType      = 0;
     header.bfSize      = 0;
     header.bfReserved1 = 0;
     header.bfReserved2 = 0;
     header.bfOffBits   = 0;

     //set dummy info
	 bmiHeader.biSize          = 0;
     bmiHeader.biWidth         = width;
     bmiHeader.biHeight        = height;
     bmiHeader.biPlanes        = 0;
     bmiHeader.biBitCount      = 0;
     bmiHeader.biCompression   = 0;
     bmiHeader.biSizeImage     = 0;
     bmiHeader.biXPelsPerMeter = 0;
     bmiHeader.biYPelsPerMeter = 0;
     bmiHeader.biClrUsed       = 0;
     bmiHeader.biClrImportant  = 0;

     //set dummy image data
	 rgb = new RGBColor*[bmiHeader.biHeight];
     for (int i = 0; i < bmiHeader.biHeight; i++)
            rgb[i] = new RGBColor[bmiHeader.biWidth];

     for (int i = 0; i < bmiHeader.biHeight; i++) {
         for (int j = 0; j < bmiHeader.biWidth; j++) {
            rgb[i][j].rgbBlue = 1;
            rgb[i][j].rgbGreen = 1;
            rgb[i][j].rgbRed = 1;
         }
      }

     //set object properties
	 imageWidth = bmiHeader.biWidth;
     imageHeight = bmiHeader.biHeight;
}

BMPImage::~BMPImage ()
{
	for (int i=0; i<imageHeight; i++)
	{
		delete [] rgb[i];
	}

	delete [] rgb;
}

void BMPImage::ReadColorBmp (string filename)
{
	pFile.open(filename, ios::in | ios::binary);

	 if (pFile.is_open())
	 { 
		 //get header
		 header.bfType      = read_u16();
		 header.bfSize      = read_u32();
		 header.bfReserved1 = read_u16();
		 header.bfReserved2 = read_u16();
		 header.bfOffBits   = read_u32();

		 //get info
		 bmiHeader.biSize          = read_u32();
		 bmiHeader.biWidth         = read_s32();
		 bmiHeader.biHeight        = read_s32();
		 bmiHeader.biPlanes        = read_u16();
		 bmiHeader.biBitCount      = read_u16();
		 bmiHeader.biCompression   = read_u32();
		 bmiHeader.biSizeImage     = read_u32();
		 bmiHeader.biXPelsPerMeter = read_s32();
		 bmiHeader.biYPelsPerMeter = read_s32();
		 bmiHeader.biClrUsed       = read_u32();
		 bmiHeader.biClrImportant  = read_u32();

		 int padnum = (4-(bmiHeader.biWidth*3)%4)%4; //compute number of padding bytes per row

		 //store image data
		 rgb = new RGBColor*[bmiHeader.biHeight];
		 for (int i = 0; i < bmiHeader.biHeight; i++)
				rgb[i] = new RGBColor[bmiHeader.biWidth];

		 for (int i = 0; i < bmiHeader.biHeight; i++) {
			 for (int j = 0; j < bmiHeader.biWidth; j++) {
				rgb[i][j].rgbBlue = pFile.get();
				rgb[i][j].rgbGreen = pFile.get();
				rgb[i][j].rgbRed = pFile.get();
			 }

			 //read padding bytes
			 for (int a = 0; a < padnum; a++)
			 {
				 pFile.get();
			 }
		  }

		 //set object properties
		 imageWidth = bmiHeader.biWidth;
		 imageHeight = bmiHeader.biHeight;
	 }
	 else
	 {
		 throw "Invalid file path!";
	 }
	 
     pFile.close();
}

void BMPImage::ReadGreyBmp (string filename)
{
	pFile.open(filename, ios::in | ios::binary);

	 if (pFile.is_open())
	 { 
		 //get header
		 header.bfType      = read_u16();
		 header.bfSize      = read_u32();
		 header.bfReserved1 = read_u16();
		 header.bfReserved2 = read_u16();
		 header.bfOffBits   = read_u32();

		 //get info
		 bmiHeader.biSize          = read_u32();
		 bmiHeader.biWidth         = read_s32();
		 bmiHeader.biHeight        = read_s32();
		 bmiHeader.biPlanes        = read_u16();
		 bmiHeader.biBitCount      = read_u16();
		 bmiHeader.biCompression   = read_u32();
		 bmiHeader.biSizeImage     = read_u32();
		 bmiHeader.biXPelsPerMeter = read_s32();
		 bmiHeader.biYPelsPerMeter = read_s32();
		 bmiHeader.biClrUsed       = read_u32();
		 bmiHeader.biClrImportant  = read_u32();

		//ignore palette
		for (int a = 0; a < 256; a++)
		{
			read_u32();
		}

		 //image data
		 int padnum = (4-(bmiHeader.biWidth)%4)%4; //compute number of padding bytes per row

		 //store image data
		 rgb = new RGBColor*[bmiHeader.biHeight];
		 for (int i = 0; i < bmiHeader.biHeight; i++)
				rgb[i] = new RGBColor[bmiHeader.biWidth];

		 for (int i = 0; i < bmiHeader.biHeight; i++) {
			 for (int j = 0; j < bmiHeader.biWidth; j++) {
				rgb[i][j].rgbBlue = pFile.get();
				rgb[i][j].rgbGreen = rgb[i][j].rgbBlue;
				rgb[i][j].rgbRed = rgb[i][j].rgbBlue;
			 }

			 //read padding bytes
			 for (int a = 0; a < padnum; a++)
			 {
				 pFile.get();
			 }
		  }

		 //set object properties
		 imageWidth = bmiHeader.biWidth;
		 imageHeight = bmiHeader.biHeight;
	 }
	 else
	 {
		 throw "Invalid file path!";
	 }
	 
     pFile.close();
}

void BMPImage::ReadBWBmp (string filename)
{
	pFile.open(filename, ios::in | ios::binary);

	 if (pFile.is_open())
	 { 
		 //get header
		 header.bfType      = read_u16();
		 header.bfSize      = read_u32();
		 header.bfReserved1 = read_u16();
		 header.bfReserved2 = read_u16();
		 header.bfOffBits   = read_u32();

		 //get info
		 bmiHeader.biSize          = read_u32();
		 bmiHeader.biWidth         = read_s32();
		 bmiHeader.biHeight        = read_s32();
		 bmiHeader.biPlanes        = read_u16();
		 bmiHeader.biBitCount      = read_u16();
		 bmiHeader.biCompression   = read_u32();
		 bmiHeader.biSizeImage     = read_u32();
		 bmiHeader.biXPelsPerMeter = read_s32();
		 bmiHeader.biYPelsPerMeter = read_s32();
		 bmiHeader.biClrUsed       = read_u32();
		 bmiHeader.biClrImportant  = read_u32();

		 //ignore palette
		 read_u32();
		 read_u32();

		 //image data
		 int bytelinesize = (bmiHeader.biWidth)/8 + ((bmiHeader.biWidth)/8)%4; //number of bytes per row after padding
		 int padnum1 = (8-(bmiHeader.biWidth)%8)%8; //number of additional bits to reach an integer byte count per row
		 int NumBytes = (bmiHeader.biWidth+padnum1)/8; //number of bytes per row before byte padding but after bit padding
		 int bytepad = (4-(NumBytes)%4)%4; //number of additional bytes required for padding

		 //store image data
		 rgb = new RGBColor*[bmiHeader.biHeight];
		 for (int i = 0; i < bmiHeader.biHeight; i++)
				rgb[i] = new RGBColor[bmiHeader.biWidth];

		 for (int i = 0; i < bmiHeader.biHeight; i++) {
			 for (int j = 0; j < NumBytes; j++) {
				
				 int Truej = j*8;
				 int pixels = pFile.get();

				 //possibly only part of byte is image data - LITTLE ENDIAN OR BIG ENDIAN???
				 if (j == (NumBytes-1))
				 {
					 for (int b=7; b>(padnum1-1); b--,Truej++)
					 {
						 int hold = (int)pow(2, b);
						 rgb[i][Truej].rgbBlue = ((pixels & hold) > 0) ? 255:0;
						 rgb[i][Truej].rgbGreen = rgb[i][Truej].rgbBlue;
						 rgb[i][Truej].rgbRed = rgb[i][Truej].rgbBlue;
					 }
				 }
				 //whole byte is definitely image data - LITTLE ENDIAN OR BIG ENDIAN???
				 else
				 {
					 //convert byte into 8-bits
					 for (int b=7; b>(-1); b--,Truej++)
					 {
						 int hold = (int)pow(2, b);
						 rgb[i][Truej].rgbBlue = ((pixels & hold) > 0) ? 255:0;
						 rgb[i][Truej].rgbGreen = rgb[i][Truej].rgbBlue;
						 rgb[i][Truej].rgbRed = rgb[i][Truej].rgbBlue;
					 }
				 }
			 }

			 //read padding bytes
			 for (int a = 0; a < bytepad; a++)
			 {
				 pFile.get();
			 }
		  }

		 //set object properties
		 imageWidth = bmiHeader.biWidth;
		 imageHeight = bmiHeader.biHeight;
	 }
	 else
	 {
		 throw "Invalid file path!";
	 }
	 
     pFile.close();
}

unsigned short BMPImage::read_u16(){
    unsigned char b0, b1;
    b0 = pFile.get();
    b1 = pFile.get();

    //little-endian, so shift second byte up 8
	return ((b1 << 8) | b0);
}

unsigned int BMPImage::read_u32(){
    unsigned char b0, b1, b2, b3;
    b0 = pFile.get();
    b1 = pFile.get();
    b2 = pFile.get();
    b3 = pFile.get();

    //little-endian, so shift 4th byte up 24, 3rd byte up 16, 2nd byte up 8
	return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

int BMPImage::read_s32(){
    unsigned char b0, b1, b2, b3;
    b0 = pFile.get();
    b1 = pFile.get();
    b2 = pFile.get();
    b3 = pFile.get();
    
	//little-endian, so shift 4th byte up 24, 3rd byte up 16, 2nd byte up 8, cast as int to use sign
	return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}


BitMapConverter::BitMapConverter (string rp)
{
	ResultPath = rp;
}

void BitMapConverter::WriteBitMapToFile (BMPImage* img)
{
	ofstream output;
	output.open(ResultPath, ios::out | ios::app | ios::binary);

	if (output.is_open())
	{	
		unsigned int padnum = (4-(img->imageWidth*3)%4)%4; //required byte padding per row
		
		//header
		unsigned int bftype = img->header.bfType; output.write(reinterpret_cast <const char*> (&bftype), 2);
		long bfsize = img->header.bfSize; output.write(reinterpret_cast <const char*> (&bfsize), 4);
		int bfr1 = img->header.bfReserved1; output.write(reinterpret_cast <const char*> (&bfr1), 2);
		int bfr2 = img->header.bfReserved2; output.write(reinterpret_cast <const char*> (&bfr2), 2);
		long bfoffbits = img->header.bfOffBits; output.write(reinterpret_cast <const char*> (&bfoffbits), 4);

		//info
		unsigned int bisize = img->bmiHeader.biSize; output.write(reinterpret_cast <const char*> (&bisize), 4);
		int biwidth = img->bmiHeader.biWidth; output.write(reinterpret_cast <const char*> (&biwidth), 4);
		int biheight = img->bmiHeader.biHeight; output.write(reinterpret_cast <const char*> (&biheight), 4);
		unsigned short biplanes = img->bmiHeader.biPlanes; output.write(reinterpret_cast <const char*> (&biplanes), 2);
		unsigned short bibitcount = img->bmiHeader.biBitCount; output.write(reinterpret_cast <const char*> (&bibitcount), 2);
		unsigned int bicompression = img->bmiHeader.biCompression; output.write(reinterpret_cast <const char*> (&bicompression), 4);
		unsigned int bisizeimage = img->bmiHeader.biSizeImage; output.write(reinterpret_cast <const char*> (&bisizeimage), 4);
		int bixpix = img->bmiHeader.biXPelsPerMeter; output.write(reinterpret_cast <const char*> (&bixpix), 4);
		int biypix = img->bmiHeader.biYPelsPerMeter; output.write(reinterpret_cast <const char*> (&biypix), 4);
		unsigned int biclrused = img->bmiHeader.biClrUsed; output.write(reinterpret_cast <const char*> (&biclrused), 4);
		unsigned int biclrimp = img->bmiHeader.biClrImportant; output.write(reinterpret_cast <const char*> (&biclrimp), 4);

		int bmppad = 0;

		//write image data
		for (unsigned int i=0; i<(unsigned int)img->imageHeight; i++)
		{
			for (unsigned int j=0; j<(unsigned int)img->imageWidth; j++)
			{
				unsigned char val1 = (unsigned char)img->rgb[i][j].rgbBlue;
				unsigned char val2 = (unsigned char)img->rgb[i][j].rgbGreen;
				unsigned char val3 = (unsigned char)img->rgb[i][j].rgbRed;
					
				output.write(reinterpret_cast <const char*> (&(val1)), 1);
				output.write(reinterpret_cast <const char*> (&(val2)), 1);
				output.write(reinterpret_cast <const char*> (&(val3)), 1);
			}

			//pad row
			for (unsigned int test1=0; test1 < padnum; test1++)
			{
				unsigned char holder = (unsigned char)bmppad;
				output.write(reinterpret_cast <const char*> (&holder), 1);
			}
		}
	}
	else
	{
		throw "Invalid result file path!";
	}

	output.close();
}


void BitMapConverter::WriteBitMapToFileBW (BMPImage* img)
{
	//compute various new properties for header and padding
	int bytelinesize = (img->imageWidth)/8 + ((img->imageWidth)/8)%4; //number of bytes per row after padding

	int padnum1 = (8-(img->imageWidth)%8)%8; //number of additional bits to reach an integer byte count per row
	int NumBytes = (img->imageWidth+padnum1)/8; //number of bytes per row before byte padding but after bit padding
	int bytepad = (4-(NumBytes)%4)%4; //number of additional bytes required for padding

	unsigned int ImgSize1 = (NumBytes + bytepad)*img->imageHeight; //new image data size in bytes
	long hsize1 = ImgSize1 + 54 + 8; //new total file size in bytes
	long newoffset = img->header.bfOffBits + 8; //new offset for the beginning of the image data

	unsigned short bits = 1; //new bits per pixel

	ofstream output;
	output.open(ResultPath, ios::out | ios::app | ios::binary);

	if (output.is_open())
	{	
		//header
		unsigned int bftype = img->header.bfType; output.write(reinterpret_cast <const char*> (&bftype), 2);
		long bfsize = img->header.bfSize; output.write(reinterpret_cast <const char*> (&bfsize), 4);
		//output.write(reinterpret_cast <const char*> (&hsize1), 4);
		int bfr1 = img->header.bfReserved1; output.write(reinterpret_cast <const char*> (&bfr1), 2);
		int bfr2 = img->header.bfReserved2; output.write(reinterpret_cast <const char*> (&bfr2), 2);
		long bfoffset = img->header.bfOffBits; output.write(reinterpret_cast <const char*> (&bfoffset), 4);
		//output.write(reinterpret_cast <const char*> (&newoffset), 4);

		//info
		unsigned int bisize = img->bmiHeader.biSize; output.write(reinterpret_cast <const char*> (&bisize), 4);
		int biwidth = img->bmiHeader.biWidth; output.write(reinterpret_cast <const char*> (&biwidth), 4);
		int biheight = img->bmiHeader.biHeight; output.write(reinterpret_cast <const char*> (&biheight), 4);
		unsigned short biplanes = img->bmiHeader.biPlanes; output.write(reinterpret_cast <const char*> (&biplanes), 2);
		unsigned short bibits = img->bmiHeader.biBitCount; output.write(reinterpret_cast <const char*> (&bibits), 2);
		//output.write(reinterpret_cast <const char*> (&bits), 2);
		unsigned int bicompression = img->bmiHeader.biCompression; output.write(reinterpret_cast <const char*> (&bicompression), 4);
		unsigned int biImageSize = img->bmiHeader.biSizeImage; output.write(reinterpret_cast <const char*> (&biImageSize), 4);
		//output.write(reinterpret_cast <const char*> (&ImgSize1), 4);
		int bixpix = img->bmiHeader.biXPelsPerMeter; output.write(reinterpret_cast <const char*> (&bixpix), 4);
		int biypix = img->bmiHeader.biYPelsPerMeter; output.write(reinterpret_cast <const char*> (&biypix), 4);
		unsigned int biclrused = img->bmiHeader.biClrUsed; output.write(reinterpret_cast <const char*> (&biclrused), 4);
		unsigned int biclrimp = img->bmiHeader.biClrImportant; output.write(reinterpret_cast <const char*> (&biclrimp), 4);
		
		//write the color palette
		unsigned int Palette1 [2] = {0, 4294967295}; //second number represents white in binary
		output.write(reinterpret_cast <const char*> (&(Palette1)), 8);

		//image data
		int bmppad = 0; //use for padding bytes
		int count = 0; //use to keep track of groups of 8 bits
		int checking = img->imageWidth + padnum1; //use for number of bits per row before padding
		unsigned int tempnum; //use to write bytes from bits of image data
		unsigned int holdnum; //use to store current bit
		unsigned char tempchar; //use for actual writing of byte to file
		for (unsigned int i=0; i<(unsigned int)img->imageHeight; i++)
		{	
			for (unsigned int j=0; j<(unsigned int)checking; j++)
			{
				//add dummy pixels once end of image data row is reached
				if (j >= (unsigned int)img->imageWidth)
				{
					holdnum = 0;
				}
				else
				{
					//white pixel
					if (img->rgb[i][j].rgbBlue == 255)
					{
						holdnum = 1;
					}
					//black pixel
					else
					{
						holdnum = 0;
					}
				}
				//write to file every 8 bits
				if (count == 8)
				{
					count = 0;

					tempchar = (unsigned char)tempnum;
					output.write(reinterpret_cast <const char*> (&(tempchar)), 1);
					tempnum = 0;
				}
				tempnum += (unsigned int)(pow(2, 7-count)*holdnum); //little-endian, so shift the current bit appropriately
				count++;
			}

			//write the remaining byte if we are in the last column
			if (i == (img->imageHeight-1))
			{
				tempchar = (unsigned char)tempnum;
				output.write(reinterpret_cast <const char*> (&(tempchar)), 1);
			}

			//pad the additional bytes
			for (int test1=0; test1 < bytepad; test1++)
			{
				unsigned char holder2 = (unsigned char)bmppad;
				output.write(reinterpret_cast <const char*> (&holder2), 1);
			}
		}
	}
	else
	{
		throw "Invalid result file path!";
	}

	output.close();
}


void BitMapConverter::WriteBitMapToFileGrey (BMPImage* img)
{
	//compute various new properties for header and padding
	unsigned short bits = 8; //new bits per pixel
	unsigned int padnum = (4-(img->imageWidth)%4)%4; //byte row padding
	unsigned int ImageSize = (img->imageWidth + padnum)*img->imageHeight; //new size of image data in bytes
	long FileSize = ImageSize + 54 + 1024; //new total file size in bytes (including color palette)
	long NewOffset = img->header.bfOffBits + 1024; //new offset for start of image data

	ofstream output;
	output.open(ResultPath, ios::out | ios::app | ios::binary);
	
	if (output.is_open())
	{	
		//header
		unsigned int bftype = img->header.bfType; output.write(reinterpret_cast <const char*> (&bftype), 2);
		long bfsize = img->header.bfSize; output.write(reinterpret_cast <const char*> (&bfsize), 4);
		//output.write(reinterpret_cast <const char*> (&FileSize), 4);
		int bfr1 = img->header.bfReserved1; output.write(reinterpret_cast <const char*> (&bfr1), 2);
		int bfr2 = img->header.bfReserved2; output.write(reinterpret_cast <const char*> (&bfr2), 2);
		long bfoffset = img->header.bfOffBits; output.write(reinterpret_cast <const char*> (&bfoffset), 4);
		//output.write(reinterpret_cast <const char*> (&NewOffset), 4);

		//info
		unsigned int bisize = img->bmiHeader.biSize; output.write(reinterpret_cast <const char*> (&bisize), 4);
		int biwidth = img->bmiHeader.biWidth; output.write(reinterpret_cast <const char*> (&biwidth), 4);
		int biheight = img->bmiHeader.biHeight; output.write(reinterpret_cast <const char*> (&biheight), 4);
		unsigned short biplanes = img->bmiHeader.biPlanes; output.write(reinterpret_cast <const char*> (&biplanes), 2);
		unsigned short bibits = img->bmiHeader.biBitCount; output.write(reinterpret_cast <const char*> (&bibits), 2);
		//output.write(reinterpret_cast <const char*> (&bits), 2);
		unsigned int bicompression = img->bmiHeader.biCompression; output.write(reinterpret_cast <const char*> (&bicompression), 4);
		unsigned int bisizeimage = img->bmiHeader.biSizeImage; output.write(reinterpret_cast <const char*> (&bisizeimage), 4);
		//output.write(reinterpret_cast <const char*> (&ImageSize), 4);
		int bixpix = img->bmiHeader.biXPelsPerMeter; output.write(reinterpret_cast <const char*> (&bixpix), 4);
		int biypix = img->bmiHeader.biYPelsPerMeter; output.write(reinterpret_cast <const char*> (&biypix), 4);
		unsigned int biclrused = img->bmiHeader.biClrUsed; output.write(reinterpret_cast <const char*> (&biclrused), 4);
		unsigned int biclrimp = img->bmiHeader.biClrImportant; output.write(reinterpret_cast <const char*> (&biclrimp), 4);

		int bmppad = 0; //use for padding bytes
		
		//palette data
		unsigned char c1;
		unsigned char c2 = (unsigned char)bmppad;
		for (int a = 0; a < 256; a++)
		{
			c1 = (unsigned char)a;
			output.write(reinterpret_cast <const char*> (&(c1)), 1);
			output.write(reinterpret_cast <const char*> (&(c1)), 1);
			output.write(reinterpret_cast <const char*> (&(c1)), 1);
			output.write(reinterpret_cast <const char*> (&(c2)), 1);
		}

		//image data
		for (unsigned int i=0; i<(unsigned int)img->imageHeight; i++)
		{
			for (unsigned int j=0; j<(unsigned int)img->imageWidth; j++)
			{
				c1 = (unsigned char)img->rgb[i][j].rgbBlue;
					
				output.write(reinterpret_cast <const char*> (&(c1)), 1);
			}

			for (unsigned int test1=0; test1 < padnum; test1++)
			{
				output.write(reinterpret_cast <const char*> (&c2), 1);
			}
		}
	}
	else
	{
		throw "Invalid result file path!";
	}

	output.close();
}

void BitMapConverter::ApplyHighlight (BMPImage* img, vector<unsigned int>& TopLeft, unsigned int width, unsigned int height)
{
	//checks to ensure valid inputs
	if (width == 0 || height == 0 || (TopLeft[0] + width) >= (unsigned int)img->imageWidth || (TopLeft[1] + height) >= (unsigned int)img->imageHeight)
	{
		throw "Invalid coordinates for highlight!";
	}

	//reflect coordinates based on how bitmap is read in
	unsigned int TrueLeft = TopLeft[0];
	unsigned int TrueTop = (img->imageHeight - TopLeft[1]);

	//update image data
	for (unsigned int i=TrueTop; i>(TrueTop-height); i--)
	{
		for (unsigned int j=TrueLeft; j<(TrueLeft+width); j++)
		{
			//only change the pixel value if it's white
			if ((img->rgb[i][j].rgbBlue == 255) && (img->rgb[i][j].rgbGreen == 255) && (img->rgb[i][j].rgbRed == 255))
			{
				//yellow pixel
				img->rgb[i][j].rgbBlue = 0;
				img->rgb[i][j].rgbGreen = 255;
				img->rgb[i][j].rgbRed = 255;
				img->rgb[i][j].rgbReserved = 0;
			}
		}
	}
}

void BitMapConverter::ApplyRedaction (BMPImage* img, vector<unsigned int>& TopLeft, unsigned int width, unsigned int height, bool mode)
{
	//checks to ensure valid inputs
	if (width == 0 || height == 0 || (TopLeft[0] + width) >= (unsigned int)img->imageWidth || (TopLeft[1] + height) >= (unsigned int)img->imageHeight)
	{
		throw "Invalid coordinates for redaction!";
	}

	//reflect coordinates based on how bitmap is read in
	unsigned int TrueLeft = TopLeft[0];
	unsigned int TrueTop = (img->imageHeight - TopLeft[1]);

	//update image data
	for (unsigned int i=TrueTop; i>(TrueTop-height); i--)
	{
		for (unsigned int j=TrueLeft; j<(TrueLeft+width); j++)
		{
			//black redaction
			if (mode)
			{
				//black pixel
				img->rgb[i][j].rgbBlue = 0;
				img->rgb[i][j].rgbGreen = 0;
				img->rgb[i][j].rgbRed = 0;
				img->rgb[i][j].rgbReserved = 0;
			}
			//white redaction
			else
			{
				//white pixel
				img->rgb[i][j].rgbBlue = 255;
				img->rgb[i][j].rgbGreen = 255;
				img->rgb[i][j].rgbRed = 255;
				img->rgb[i][j].rgbReserved = 0;
			}
		}
	}
}

void BitMapConverter::ApplyBorder (BMPImage* img, vector<unsigned int>& TopLeft, unsigned int width, unsigned int height, unsigned int thickness, RGBColor* color)
{
	//checks to ensure valid inputs
	if (width == 0 || height == 0 || (TopLeft[0] + width) >= (unsigned int)img->imageWidth || (TopLeft[1] + height) >= (unsigned int)img->imageHeight)
	{
		throw "Invalid coordinates for redaction!";
	}

	//reflect coordinates based on how bitmap is read in
	unsigned int TrueLeft = TopLeft[0];
	unsigned int TrueTop = (img->imageHeight - TopLeft[1]);

	//left border
	for (unsigned int j=TrueLeft; j<(TrueLeft+thickness); j++)
	{
		for (unsigned int i=TrueTop; i>(TrueTop-height); i--)
		{
			img->rgb[i][j].rgbBlue = color->rgbBlue;
			img->rgb[i][j].rgbGreen = color->rgbGreen;
			img->rgb[i][j].rgbRed = color->rgbRed;
		}
	}

	//right border
	for (unsigned int j=(TrueLeft+width-thickness); j<(TrueLeft+width); j++)
	{
		for (unsigned int i=TrueTop; i>(TrueTop-height); i--)
		{
			img->rgb[i][j].rgbBlue = color->rgbBlue;
			img->rgb[i][j].rgbGreen = color->rgbGreen;
			img->rgb[i][j].rgbRed = color->rgbRed;
		}
	}

	//top border
	for (unsigned int i=TrueTop; i>(TrueTop-thickness); i--)
	{
		for (unsigned int j=TrueLeft; j<(TrueLeft+width); j++)
		{
			img->rgb[i][j].rgbBlue = color->rgbBlue;
			img->rgb[i][j].rgbGreen = color->rgbGreen;
			img->rgb[i][j].rgbRed = color->rgbRed;
		}
	}

	//bottom border
	for (unsigned int i=(TrueTop-height); i<(TrueTop-height+thickness); i++)
	{
		for (unsigned int j=TrueLeft; j<(TrueLeft+width); j++)
		{
			img->rgb[i][j].rgbBlue = color->rgbBlue;
			img->rgb[i][j].rgbGreen = color->rgbGreen;
			img->rgb[i][j].rgbRed = color->rgbRed;
		}
	}
	
}

void BitMapConverter::InvertImage (BMPImage* img)
{
	//update image data
	for (int i=0; i<img->imageHeight; i++)
	{
		for (int j=0; j<img->imageWidth; j++)
		{
			//convert white pixel to black
			if ((img->rgb[i][j].rgbBlue == 255) && (img->rgb[i][j].rgbGreen == 255) && (img->rgb[i][j].rgbRed == 255))
			{
				img->rgb[i][j].rgbBlue = 0;
				img->rgb[i][j].rgbGreen = 0;
				img->rgb[i][j].rgbRed = 0;
			}
			//convert black pixel to white
			else
			{
				img->rgb[i][j].rgbBlue = 255;
				img->rgb[i][j].rgbGreen = 255;
				img->rgb[i][j].rgbRed = 255;
			}
		}
	}
}

int BitMapConverter::NormalCoordinate (int original, int whole)
{
	int half = whole / 2;
	
	if (half % 2 == 0)
	{
		if (original >= half)
		{
			return (original-half+1);
		}
		else
		{
			return (original-half);
		}
	}
	else
	{
		return (original - half);
	}
}

int BitMapConverter::OriginalCoordinate (int normal, int whole)
{
	int half = whole / 2;
	
	if (half % 2 == 0)
	{
		if ((normal+half-1) >= half)
		{
			return (normal+half-1);
		}
		else
		{
			return (normal+half);
		}
	}
	else
	{
		return (normal + half);
	}
}

void BitMapConverter::RotateImage (BMPImage* img, BMPImage* result, int rotation)
{
	//normalize pixel grid
	unsigned int halfheight = img->imageHeight / 2;
	unsigned int halfwidth = img->imageWidth / 2;

	//90-degrees clockwise or 270-degrees counterclockwise
	if (rotation == 1 || rotation == -3)
	{
		for (int i=0; i<img->imageHeight; i++)
		{
			for (int j=0; j<img->imageWidth; j++)
			{
				int newj = OriginalCoordinate(NormalCoordinate(i, img->imageHeight), img->imageHeight);
				int newi = OriginalCoordinate((NormalCoordinate(j, img->imageWidth)*(-1)), img->imageWidth);

				result->rgb[newi][newj].rgbBlue = img->rgb[i][j].rgbBlue;
				result->rgb[newi][newj].rgbGreen = img->rgb[i][j].rgbGreen;
				result->rgb[newi][newj].rgbRed = img->rgb[i][j].rgbRed;
			}
		}
	}
	//180 degrees clockwise or counterclockwise
	else if (rotation == 2 || rotation == -2)
	{
		for (int i=0; i<img->imageHeight; i++)
		{
			for (int j=0; j<img->imageWidth; j++)
			{
				int newj = OriginalCoordinate(NormalCoordinate(j, img->imageWidth)*(-1), img->imageWidth);
				int newi = OriginalCoordinate((NormalCoordinate(i, img->imageHeight)*(-1)), img->imageHeight);

				result->rgb[newi][newj].rgbBlue = img->rgb[i][j].rgbBlue;
				result->rgb[newi][newj].rgbGreen = img->rgb[i][j].rgbGreen;
				result->rgb[newi][newj].rgbRed = img->rgb[i][j].rgbRed;
			}
		}
	}
	//270-degrees clockwise or 90-degrees counterclockwise
	else if (rotation == 3 || rotation == -1)
	{
		for (int i=0; i<img->imageHeight; i++)
		{
			for (int j=0; j<img->imageWidth; j++)
			{
				int newj = OriginalCoordinate(NormalCoordinate(i, img->imageHeight)*(-1), img->imageHeight);
				int newi = OriginalCoordinate(NormalCoordinate(j, img->imageWidth), img->imageWidth);

				result->rgb[newi][newj].rgbBlue = img->rgb[i][j].rgbBlue;
				result->rgb[newi][newj].rgbGreen = img->rgb[i][j].rgbGreen;
				result->rgb[newi][newj].rgbRed = img->rgb[i][j].rgbRed;
			}
		}
	}
	//bad rotation value
	else
	{
		throw "Invalid rotation value!";
	}

	//copy all header and info (except for width and height)
	result->header.bfType = img->header.bfType;
    result->header.bfSize = img->header.bfSize;
    result->header.bfReserved1 = img->header.bfReserved1;
    result->header.bfReserved2 = img->header.bfReserved2;
    result->header.bfOffBits = img->header.bfOffBits;

    result->bmiHeader.biSize = img->bmiHeader.biSize;
    result->bmiHeader.biPlanes = img->bmiHeader.biPlanes;
    result->bmiHeader.biBitCount = img->bmiHeader.biBitCount;
    result->bmiHeader.biCompression = img->bmiHeader.biCompression;
    result->bmiHeader.biSizeImage = img->bmiHeader.biSizeImage;
    result->bmiHeader.biXPelsPerMeter = img->bmiHeader.biXPelsPerMeter;
    result->bmiHeader.biYPelsPerMeter = img->bmiHeader.biYPelsPerMeter;
    result->bmiHeader.biClrUsed = img->bmiHeader.biClrUsed;
    result->bmiHeader.biClrImportant = img->bmiHeader.biClrImportant;
	
}

void BitMapConverter::ReflectImage (BMPImage* img, BMPImage* result, bool orientation)
{
	//normalize pixel grid
	unsigned int halfheight = img->imageHeight / 2;
	unsigned int halfwidth = img->imageWidth / 2;

	//reflect about y-axis
	if (orientation)
	{
		for (int i=0; i<img->imageHeight; i++)
		{
			for (int j=0; j<img->imageWidth; j++)
			{
				int newj = OriginalCoordinate(NormalCoordinate(j, img->imageWidth)*(-1), img->imageWidth);

				result->rgb[i][newj].rgbBlue = img->rgb[i][j].rgbBlue;
				result->rgb[i][newj].rgbGreen = img->rgb[i][j].rgbGreen;
				result->rgb[i][newj].rgbRed = img->rgb[i][j].rgbRed;
			}
		}
	}
	//reflect about x-axis
	else
	{
		for (int i=0; i<img->imageHeight; i++)
		{
			for (int j=0; j<img->imageWidth; j++)
			{
				int newi = OriginalCoordinate(NormalCoordinate(i, img->imageHeight)*(-1), img->imageHeight);

				result->rgb[newi][j].rgbBlue = img->rgb[i][j].rgbBlue;
				result->rgb[newi][j].rgbGreen = img->rgb[i][j].rgbGreen;
				result->rgb[newi][j].rgbRed = img->rgb[i][j].rgbRed;
			}
		}
	}

	//copy all header and info (except for width and height)
	result->header.bfType = img->header.bfType;
    result->header.bfSize = img->header.bfSize;
    result->header.bfReserved1 = img->header.bfReserved1;
    result->header.bfReserved2 = img->header.bfReserved2;
    result->header.bfOffBits = img->header.bfOffBits;

    result->bmiHeader.biSize = img->bmiHeader.biSize;
    result->bmiHeader.biPlanes = img->bmiHeader.biPlanes ;
    result->bmiHeader.biBitCount = img->bmiHeader.biBitCount;
    result->bmiHeader.biCompression = img->bmiHeader.biCompression;
    result->bmiHeader.biSizeImage = img->bmiHeader.biSizeImage;
    result->bmiHeader.biXPelsPerMeter = img->bmiHeader.biXPelsPerMeter;
    result->bmiHeader.biYPelsPerMeter = img->bmiHeader.biYPelsPerMeter;
    result->bmiHeader.biClrUsed = img->bmiHeader.biClrUsed;
    result->bmiHeader.biClrImportant = img->bmiHeader.biClrImportant;
}

vector<vector<double>> BitMapConverter::GaussianKernel (int kernelsize, double sigma)
{
	//check for odd kernelsize
	if (kernelsize % 2 == 0)
	{
		throw "Kernel size must be odd!";
	}

	vector<vector<double>> result;

	const double e = exp(1.0);
	const double pi = (22.0)/(7.0);
	const double constant = pow(2*pi*pow(sigma, 2.0), -1);

	//loop through points
	double sum = 0;
	for (int i=0; i<kernelsize; i++)
	{
		vector<double> temp;
		result.push_back(temp);
		
		for (int j=0; j<kernelsize; j++)
		{
			int x = (j > (kernelsize/2) ? (j-(kernelsize/2)) : (j-(kernelsize/2)));
			int y = (i <= (kernelsize/2) ? (i+(kernelsize/2)) : ((i*-1)+(kernelsize/2)));

			double exponent = ((pow(x, 2)+pow(y, 2)*-1 / (2*pow(sigma, 2))));

			double val = (constant*pow(e,exponent));
			result[i].push_back(val);
			sum += val;
		}
	}

	//normalize the matrix
	for (int i=0; i<kernelsize; i++)
	{
		for (int j=0; j<kernelsize; j++)
		{
			result[i][j] = (result[i][j] / sum);
		}
	}

	return result;
}

RGBColor* BitMapConverter::GaussianFilter (BMPImage* PixelMatrix, int x, int y, vector<vector<double>>& Gaussian)
{
	//get neighborhood size
	int kernelsize = Gaussian.size();
	
	//compute bounds for neighborhood
	int leftbound = (x-(kernelsize/2) >= 0)? (x-(kernelsize/2)):0;
	int rightbound = (x+(kernelsize/2) < PixelMatrix->imageWidth)? (x+(kernelsize/2)):PixelMatrix->imageWidth-1;
	int bottombound = (y-(kernelsize/2) >= 0)? (y-(kernelsize/2)):0;
	int topbound = (y+(kernelsize/2) < PixelMatrix->imageHeight)? (y+(kernelsize/2)):PixelMatrix->imageHeight-1;

	double rsum = 0;
	double gsum = 0;
	double bsum = 0;
	for (int i=bottombound; i<=topbound; i++)
	{
		for (int j=leftbound; j<=rightbound; j++)
		{
			int xIndex = (j-leftbound);
			int yIndex = kernelsize-(i-bottombound)-1;

			rsum += (Gaussian[yIndex][xIndex] * (double)PixelMatrix->rgb[i][j].rgbRed);
			gsum += (Gaussian[yIndex][xIndex] * (double)PixelMatrix->rgb[i][j].rgbGreen);
			bsum += (Gaussian[yIndex][xIndex] * (double)PixelMatrix->rgb[i][j].rgbBlue);
		}
	}

	//round sums to ints
	int red = (int)(rsum + 0.5);
	int green = (int)(gsum + 0.5);
	int blue = (int)(bsum + 0.5);

	//create result
	RGBColor* result = new RGBColor();
	result->rgbBlue = blue;
	result->rgbGreen = green;
	result->rgbRed = red;

	return result;
}

void BitMapConverter::SmoothImage (BMPImage* img, BMPImage* result, int kernelsize, double sigma)
{
	//compute gaussian matrix
	vector<vector<double>> Gaussian = GaussianKernel(kernelsize, sigma);
	
	//apply gaussian filter to each pixel to determine new color values
	for (int i=(kernelsize/2); i<img->imageHeight-(kernelsize/2); i++)
	{
		for (int j=(kernelsize/2); j<img->imageWidth-(kernelsize/2); j++)
		{
			RGBColor* temp = GaussianFilter(img, j, i, Gaussian);

			result->rgb[i][j].rgbBlue = temp->rgbBlue;
			result->rgb[i][j].rgbGreen = temp->rgbGreen;
			result->rgb[i][j].rgbRed = temp->rgbRed;

			delete temp;
		}
	}

	//copy bottom border pixels without filter
	for (int i=0; i<(kernelsize/2); i++)
	{
		for (int j=0; j<img->imageWidth; j++)
		{
			result->rgb[i][j].rgbBlue = img->rgb[i][j].rgbBlue;
			result->rgb[i][j].rgbGreen = img->rgb[i][j].rgbGreen;
			result->rgb[i][j].rgbRed = img->rgb[i][j].rgbRed;
		}
	}

	//copy top border pixels without filter
	for (int i=(img->imageHeight-(kernelsize/2)); i<img->imageHeight; i++)
	{
		for (int j=0; j<img->imageWidth; j++)
		{
			result->rgb[i][j].rgbBlue = img->rgb[i][j].rgbBlue;
			result->rgb[i][j].rgbGreen = img->rgb[i][j].rgbGreen;
			result->rgb[i][j].rgbRed = img->rgb[i][j].rgbRed;
		}
	}

	//copy left border pixels without filter
	for (int j=0; j<(kernelsize/2); j++)
	{
		for (int i=0; i<img->imageHeight; i++)
		{
			result->rgb[i][j].rgbBlue = img->rgb[i][j].rgbBlue;
			result->rgb[i][j].rgbGreen = img->rgb[i][j].rgbGreen;
			result->rgb[i][j].rgbRed = img->rgb[i][j].rgbRed;
		}
	}

	//copy right border pixels without filter
	for (int j=(img->imageWidth-(kernelsize/2)); j<img->imageWidth; j++)
	{
		for (int i=0; i<img->imageHeight; i++)
		{
			result->rgb[i][j].rgbBlue = img->rgb[i][j].rgbBlue;
			result->rgb[i][j].rgbGreen = img->rgb[i][j].rgbGreen;
			result->rgb[i][j].rgbRed = img->rgb[i][j].rgbRed;
		}
	}

	//copy header and info
	result->header.bfType = img->header.bfType;
    result->header.bfSize = img->header.bfSize;
    result->header.bfReserved1 = img->header.bfReserved1;
    result->header.bfReserved2 = img->header.bfReserved2;
    result->header.bfOffBits = img->header.bfOffBits;

    result->bmiHeader.biSize = img->bmiHeader.biSize;
    result->bmiHeader.biPlanes = img->bmiHeader.biPlanes ;
    result->bmiHeader.biBitCount = img->bmiHeader.biBitCount;
    result->bmiHeader.biCompression = img->bmiHeader.biCompression;
    result->bmiHeader.biSizeImage = img->bmiHeader.biSizeImage;
    result->bmiHeader.biXPelsPerMeter = img->bmiHeader.biXPelsPerMeter;
    result->bmiHeader.biYPelsPerMeter = img->bmiHeader.biYPelsPerMeter;
    result->bmiHeader.biClrUsed = img->bmiHeader.biClrUsed;
    result->bmiHeader.biClrImportant = img->bmiHeader.biClrImportant;

}

void BitMapConverter::DespeckleImage (BMPImage* img, int kernelsize, int threshold)
{
	int leftbound;
	int rightbound;
	int topbound;
	int bottombound;
	int count;
	
	for (int y=0; y<img->imageHeight; y++)
	{
		for (int x=0; x<img->imageWidth; x++)
		{
			//only deal with black pixels (potential speckles)
			if (img->rgb[y][x].rgbBlue == 0)
			{
				leftbound = (x-(kernelsize/2) >= 0)? (x-(kernelsize/2)):0;
				rightbound = (x+(kernelsize/2) < img->imageWidth)? (x+(kernelsize/2)):img->imageWidth-1;
				bottombound = (y-(kernelsize/2) >= 0)? (y-(kernelsize/2)):0;
				topbound = (y+(kernelsize/2) < img->imageHeight)? (y+(kernelsize/2)):img->imageHeight-1;

				count = 0;

				//check surrounding pixels to see if any are also black
				for (int i=bottombound; i<=topbound; i++)
				{
					for (int j=leftbound; j<=rightbound; j++)
					{
						if (j!=x && i!=y && img->rgb[i][j].rgbBlue == 0)
						{
							count++;
						}
					}
				}

				//if no surrounding pixels were black, set the current pixel to white as it is a speckle
				if (count <= threshold)
				{
					img->rgb[y][x].rgbBlue = 255;
				}
			}
		}
	}
	
	
}