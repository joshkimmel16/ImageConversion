#pragma once

#include <vector>

using namespace std;

class Pixel
{
private:
	unsigned int State; //color=2, greyscale=1, BW=0
	vector<unsigned int> Values;

public:
	Pixel (unsigned int S, unsigned int r, unsigned int g, unsigned int b); //constructor taking state and rgb values
	unsigned int GetState (); //accessor function for state
	unsigned int GetPixelValue (int index); //get the rgb value at the given index
	void SetPixelValue (unsigned int r, unsigned int g, unsigned int b); //set all pixel values

};

class ImageObj
{
private: 
	unsigned int State; //color=2, greyscale=1, BW=0
	unsigned int height;
	unsigned int width;
	vector<vector<Pixel*>> Pixels;

	string CompressImageGreyBW (); //create a string for DB storage for a greyscale/BW image
	string CompressImageColor (); //create a string for DB storage for a color image
	string ConvertToHex (unsigned int value); //take integer and return 2-char hex representation
	char HexMap (unsigned int val); //given an integer [0,15], return the corresponding hex character
	unsigned int ConvertToNum (string hex); //take 2-char hex and return decimal value as integer
	unsigned int NumMap (char hex); //given a hex character [0-9, a-f], return corresponding integer value
	unsigned int StringToNum (string s); //convert the given integer string to a number
	string NumToString (unsigned int num); //convert an integer to a string of that integer

public:
	string Name; //name for the image

	ImageObj (unsigned int S, unsigned int width, unsigned int height); //constructor taking state and width/height, initializes white image
	ImageObj (vector<vector<Pixel*>> p); //constructor that copies the given pixel vector (pseudo-copy constructor)
	ImageObj (string xml); //constructor that uses xml representation of image data to create object
	~ImageObj (); //destructor that will be overloaded to delete pixel vector
	unsigned int GetState (); //accessor function for state of image
	unsigned int GetHeight (); //accessor function for height of image
	unsigned int GetWidth (); //accessor function for width of image
	Pixel* GetPixel (unsigned int x, unsigned int y); //return a pointer to the pixel at the given index
	void SetPixelValue (unsigned int x, unsigned int y, unsigned int r, unsigned int g, unsigned int b); //set the rgb value of the pixel at the given index
	string CompressImage (); //return xml string of image data for DB storage

};
