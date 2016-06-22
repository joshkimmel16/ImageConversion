#pragma once

#include "Images.h"
#include <vector>
#include <regex>

using namespace std;

#pragma region Pixel
Pixel::Pixel (unsigned int S, unsigned int r, unsigned int g, unsigned int b)
{	
	State = S;
		
	//BW
	if (S == 0)
	{	
		if (r == 0 || r == 1)
		{
			Values.push_back(r);
		}
		else
		{
			throw "BW pixels can only have an intensity value of 0 or 1!";
		}
	}
	//greyscale
	else if (S == 1)
	{
		if (r >= 0 && r <= 255)
		{
			Values.push_back(r);
		}
		else
		{
			throw "Greyscale pixels can only have an intensity value between 0 and 255 inclusive";
		}
	}
	//RGB
	else if (S == 2)
	{
		if (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255)
		{
			Values.push_back(r);
			Values.push_back(g);
			Values.push_back(b);
		}
		else
		{
			throw "RGB pixels can only have an intensity value between 0 and 255 inclusive";
		}
	}
	else
	{
		throw "State must be either 0, 1, or 2!";
	}
}

unsigned int Pixel::GetState ()
{
	return State;
}

unsigned int Pixel::GetPixelValue (int index)
{
	if (State == 0 || State == 1)
	{
		return Values[0];
	}
	else
	{
		if (index == 0)
		{
			return Values[0];
		}
		else if (index == 1)
		{
			return Values[1];
		}
		else if (index == 2)
		{
			return Values[2];
		}
		else
		{
			throw "Invalid index for pixel vector!";
		}
	}
}

void Pixel::SetPixelValue (unsigned int r, unsigned int g, unsigned int b)
{
	//BW
	if (State == 0)
	{	
		if (r == 0 || r == 1)
		{
			Values[0] = r;
		}
		else
		{
			throw "BW pixels can only have an intensity value of 0 or 1!";
		}
	}
	//greyscale
	else if (State == 1)
	{
		if (r >= 0 && r <= 255)
		{
			Values[0] = r;
		}
		else
		{
			throw "Greyscale pixels can only have an intensity value between 0 and 255 inclusive";
		}
	}
	//RGB
	else
	{
		if (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255)
		{
			Values[0] = r;
			Values[1] = g;
			Values[2] = b;
		}
		else
		{
			throw "RGB pixels can only have an intensity value between 0 and 255 inclusive";
		}
	}
}

#pragma endregion

#pragma region ImageObj
string ImageObj::CompressImageGreyBW ()
{
	if (State == 2)
	{
		throw "Cannot use this method on a color image!";
	}
		
	string result = "";

	for (unsigned int i=0; i<width; i++)
	{
		for (unsigned int j=0; j<height; j++)
		{
			result += ConvertToHex(Pixels[i][j]->GetPixelValue(0));
		}

		result += "&"; //delimiter
	}

	return result;
}

string ImageObj::CompressImageColor ()
{
	if (State != 2)
	{
		throw "Cannot use this method on non-color images!";
	}

	string result = "";

	for (unsigned int i=0; i<width; i++)
	{
		for (unsigned int j=0; j<height; j++)
		{
			result += ConvertToHex(Pixels[i][j]->GetPixelValue(0));
			result += ConvertToHex(Pixels[i][j]->GetPixelValue(1));
			result += ConvertToHex(Pixels[i][j]->GetPixelValue(2));
		}

		result += "&"; //delimiter
	}

	return result;
}

string ImageObj::ConvertToHex (unsigned int value)
{
	string result = "";

	while (value > 15)
	{
		char rem = HexMap(value % 16);
		result += rem;
		value = value / 16;
	}

	result += HexMap(value);
	string trueresult = "";

	for (int i=result.length()-1; i>-1; i--)
	{
		trueresult += result[i];
	}

	return trueresult;
}

char ImageObj::HexMap (unsigned int val)
{	
	switch (val)
	{
	case (10):
		return 'a';
	case (11):
		return 'b';
	case (12):
		return 'c';
	case (13):
		return 'd';
	case (14):
		return 'e';
	case (15):
		return 'f';
	default:
		return val+48;
	}
}

unsigned int ImageObj::ConvertToNum (string hex)
{
	unsigned int sum = 0;
	unsigned int count = 0;
	for (int i=hex.length()-1; i>-1; i++)
	{
		unsigned int multiplier = (unsigned int)pow(16, count);
		sum += (multiplier * NumMap(hex[i]));
	}

	return sum;
}

unsigned int ImageObj::NumMap (char hex)
{
	switch (hex)
	{
	case ('a'):
		return 10;
	case ('b'):
		return 11;
	case ('c'):
		return 12;
	case ('d'):
		return 13;
	case ('e'):
		return 14;
	case ('f'):
		return 15;
	default:
		return hex-48;
	}
}

unsigned int ImageObj::StringToNum (string s)
{
	unsigned int sum = 0;

	for (unsigned int i=0; i<s.length(); i++)
	{
		unsigned int multiplier = (unsigned int)pow(10, i);
		unsigned int digit = s[s.length()-1-i]-48;

		sum += (digit*multiplier);
	}

	return sum;
}

string ImageObj::NumToString (unsigned int num)
{
	string temp = "";
	while (num > 0)
	{
		char digit = (num % 10)-48;
		temp += digit;

		num = num / 10;
	}

	string result = "";
	for (int i=temp.length()-1; i>-1; i--)
	{
		result += temp[i];
	}

	return result;
}

ImageObj::ImageObj (unsigned int S, unsigned int xSize, unsigned int ySize)
{
	State = S;
	if (xSize == 0 || ySize == 0)
	{
		throw "Invalid image size!";
	}
	width = xSize;
	height = ySize;

	//valid state
	if (S == 0 || S == 1 || S == 2)
	{
		for (unsigned int i=0; i<width; i++)
		{
			vector<Pixel*> temp;
			Pixels.push_back(temp);

			for (unsigned int j=0; j<height; j++)
			{
				if (S == 0)
				{
					Pixel* p = new Pixel(0, 0, 0, 0);
					Pixels[i].push_back(p);
				}
				else if (S == 1)
				{
					Pixel* p = new Pixel(1, 0, 0, 0);
					Pixels[i].push_back(p);
				}
				else
				{
					Pixel* p = new Pixel(2, 0, 0, 0);
					Pixels[i].push_back(p);
				}
					
			}
		}
	}
	//invalid state
	else
	{
		throw "Invalid state for the image!";
	}
}

ImageObj::ImageObj (vector<vector<Pixel*>> p)
{
	if (p.size() == 0 || p[0].size() == 0)
	{
		throw "Invalid pixel vector!";
	}

	int hold = p[0].size();
	for (unsigned int a = 0; a < p.size(); a++)
	{
		if (p[a].size() != hold)
		{
			throw "Invalid pixel vector!";
		}
	}
		
	State = p[0][0]->GetState();
	width = p.size();
	height = p[0].size();
		
	for (unsigned int i = 0; i < width; i++)
	{
		vector<Pixel*> temp;
		Pixels.push_back(temp);
		for (unsigned int j = 0; j < height; j++)
		{
			int S = p[i][j]->GetState();
			unsigned int pix = p[i][j]->GetPixelValue(0);
			if (S == 2)
			{
				Pixel* newp = new Pixel(S, pix, p[i][j]->GetPixelValue(1), p[i][j]->GetPixelValue(2));
				Pixels[i].push_back(newp);
			}
			else
			{
				Pixel* newp = new Pixel(S, pix, 0, 0);
				Pixels[i].push_back(newp);
			}
		}
	}
}

ImageObj::ImageObj (string xml)
{
	string name;
	string state;
	string imgdata;
	string w;
	string h;
		
	regex e1("<name>(.+)</name><state>(.+)</state><image>(.+)</image><width>(.+)</width><height>(.+)</height>");

	smatch result;
	regex_search(xml, result, e1);
	for(size_t i=0; i<result.size(); ++i)
	{
		switch (i)
		{
			case (1):
				name = result[i];
				break;
			case (2):
				state = result[i];
				break;
			case (3):
				imgdata = result[i];
				break;
			case (4):
				w = result[i];
				break;
			case (5):
				h = result[i];
				break;
			default:
				break;
		}
	}

	Name = name;
	
	if (state != "0" && state != "1" && state != "2")
	{
		throw "Invalid state in xml!";
	}
	State = NumMap(state[0]);

	width = StringToNum(w);
	height = StringToNum(h);

	//process image data
	unsigned int tempw = 1;
	unsigned int temph = 1;
	if (State == 1 || State == 0)
	{
		vector<Pixel*> temp;
		Pixels.push_back(temp);
		
		for (unsigned int i=0; i<imgdata.length(); i+=2)
		{
			if (imgdata[i] == '&')
			{
				i--;
				
				tempw++;
				if (temph != height)
				{
					throw "Invalid pixel data! (height mismatch)";
				}
				temph = 1;

				if (tempw != width)
				{
					vector<Pixel*> newtemp;
					Pixels.push_back(newtemp);
				}
				continue;
			}

			string current = ""; 
			current+= imgdata[i];
			current+= imgdata[i+1];

			unsigned int newnum = StringToNum(current);

			Pixel* Hold = new Pixel(State, newnum, 0, 0);
			Pixels[tempw].push_back(Hold);

			temph++;
		}

		if (tempw != width)
		{
			throw "Invalid pixel data! (width mismatch)";
		}
	}
	else
	{
		vector<Pixel*> temp;
		Pixels.push_back(temp);
		
		for (unsigned int i=0; i<imgdata.length(); i+=6)
		{
			if (imgdata[i] == '&')
			{
				i-=5;
				
				tempw++;
				if (temph != height)
				{
					throw "Invalid pixel data! (height mismatch)";
				}
				temph = 1;

				if (tempw != width)
				{
					vector<Pixel*> newtemp;
					Pixels.push_back(newtemp);
				}
				continue;
			}

			string current1 = ""; 
			current1+= imgdata[i];
			current1+= imgdata[i+1];

			string current2 = ""; 
			current2+= imgdata[i+2];
			current2+= imgdata[i+3];

			string current3 = ""; 
			current3+= imgdata[i+4];
			current3+= imgdata[i+5];

			unsigned int newnum1 = StringToNum(current1);
			unsigned int newnum2 = StringToNum(current2);
			unsigned int newnum3 = StringToNum(current3);

			Pixel* Hold = new Pixel(State, newnum1, newnum1, newnum1);
			Pixels[tempw].push_back(Hold);

			temph++;
		}

		if (tempw != width)
		{
			throw "Invalid pixel data! (width mismatch)";
		}
	}
}

ImageObj::~ImageObj ()
{
	for (unsigned int y = 0; y < width; y++)
	{
		for (unsigned int x = 0; x < height; x++)
		{
			delete Pixels[y][x];
		}
	}
}

unsigned int ImageObj::GetState ()
{
	return State;
}

unsigned int ImageObj::GetHeight ()
{
	return height;
}

unsigned int ImageObj::GetWidth ()
{
	return width;
}

Pixel* ImageObj::GetPixel (unsigned int x, unsigned int y)
{
	if (y >= height || x >= width)
	{
		throw "Invalid pixel index!";
	}

	return Pixels[x][y];
}

void ImageObj::SetPixelValue (unsigned int x, unsigned int y, unsigned int r, unsigned int g, unsigned int b)
{
	if (y >= height || x >= width)
	{
		throw "Invalid pixel index!";
	}

	Pixels[x][y]->SetPixelValue(r, g, b);
}

string ImageObj::CompressImage ()
{
	string xml = "<name>";
	xml += Name;
	xml += "</name><state>";
	xml += State+48;
	xml += "</state><image>";
	if (State == 2)
	{
		xml += CompressImageColor();
	}
	else
	{
		xml += CompressImageGreyBW();
	}

	xml += "</image><width>";

	xml += NumToString(width);

	xml += "</width><height>";

	xml += NumToString(height);

	xml += "</height>";


	return xml;
}
#pragma endregion


