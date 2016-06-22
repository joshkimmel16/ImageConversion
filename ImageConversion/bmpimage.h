#pragma once

#include <fstream>
#include <vector>

using namespace std;

//data structure for bitmap header
typedef struct
{
    unsigned int    bfType; //header type
    unsigned long   bfSize; //header file size
    unsigned int    bfReserved1; //header reserved1
    unsigned int    bfReserved2; //header reserved2
    unsigned long   bfOffBits; //header offset
} BitMapFileHeader;

//data structure for bitmap information
typedef struct
{
    unsigned int    biSize; //information header size
    int             biWidth; //information image width
    int             biHeight; //information image height
    unsigned short  biPlanes; //information color planes
    unsigned short  biBitCount; //information image bits per pixel
    unsigned int    biCompression; //information image compression type
    unsigned int    biSizeImage; //information image size
    int             biXPelsPerMeter; //information image pixels per meter x-axis
    int             biYPelsPerMeter; //information image pixels per meter y-axis
    unsigned int    biClrUsed; //information number of colors used
    unsigned int    biClrImportant; //information important colors
} BitMapInfoHeader;

//data structure for pixel data
typedef struct
{
    int   rgbBlue; //blue byte
    int   rgbGreen; //green byte
    int   rgbRed; //red byte
    int   rgbReserved; //reserved byte
} RGBColor;


//data structure for bitmap
class BMPImage
{
private:
    unsigned short read_u16(); //read 2 bytes (short int)
    unsigned int   read_u32(); //read 4 bytes (unsigned int)
    int            read_s32(); //read 4 bytes (signed int)
	void ReadColorBmp (string filename); //read in color bitmap
	void ReadGreyBmp (string filename); //read in GS bitmap
	void ReadBWBmp (string filename); //read in BW bitmap
public:
	BitMapFileHeader header;
	BitMapInfoHeader bmiHeader;
    std::ifstream pFile; //bmp source file
    int imageWidth; //image width
    int imageHeight; //image height
    RGBColor **rgb; //MD array of pixel data
	BMPImage(string filename, int mode); //constructor determining how to read based on type of bitmap
    BMPImage(string fileName); //constructor taking file name
	BMPImage(int width, int height); //constructor creating a blank image
	~BMPImage (); //custom destructor
};

class BitMapConverter
{
private:
	string ResultPath; //path to result .bmp file
	int NormalCoordinate (int original, int whole); //map pixel value to normal grid
	int OriginalCoordinate (int normal, int whole); //map normal grid coordinate to pixel value
	RGBColor* GaussianFilter (BMPImage* PixelMatrix, int x, int y, vector<vector<double>>& Gaussian); //apply gaussian filter to surrounding pixels to produce smoothed value
	vector<vector<double>> GaussianKernel (int kernelsize, double sigma); //compute the gaussian matrix given kernelsize and sigma

public:
	BitMapConverter (string rp); //constructor to take in result .bmp file path
	void WriteBitMapToFile (BMPImage* img); //take a bmp object and write it to file
	void WriteBitMapToFileBW (BMPImage* img); //take a bmp object that is known to be BW and write it to file
	void WriteBitMapToFileGrey (BMPImage* img); //take a bmp object that is known to be greyscale and write it to file
	void ApplyHighlight (BMPImage* img, vector<unsigned int>& TopLeft, unsigned int width, unsigned int height); //apply a highlight to the given region
	void ApplyRedaction (BMPImage* img, vector<unsigned int>& TopLeft, unsigned int width, unsigned int height, bool mode); //apply either a white or black redaction to the given region depending on mode
	void ApplyBorder (BMPImage* img, vector<unsigned int>& TopLeft, unsigned int width, unsigned int height, unsigned int thickness, RGBColor* color); //apply a border around the given region of the given color
	void InvertImage (BMPImage* img); //invert a BW image
	void RotateImage (BMPImage* img, BMPImage* result, int rotation); //rotate an image 90/180/270 degress clockwise or counterclockwise (negative = counter, 1=90, 2=180, 3=270)
	void ReflectImage (BMPImage* img, BMPImage* result, bool orientation); //reflect an image about its center, either vertically or horizontally
	void SmoothImage (BMPImage* img, BMPImage* result, int kernelsize, double sigma); //smooth image using gaussian filter
	void DespeckleImage (BMPImage* img, int kernelsize, int threshold); //remove black speckles from image
};

