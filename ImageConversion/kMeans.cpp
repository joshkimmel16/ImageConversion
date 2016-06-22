#pragma once

#include "kMeans.h"
#include "bmpimage.h"
#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>
#include <algorithm>
#include <math.h>

using namespace std;

//TODO:
//1) Improve performance of iterative calculations

void kMeansBmp::kMeansExecute (BMPImage* OldImage, BMPImage* NewImage, int k)
{
	if (k!=2)
	{
		//throw exception
		throw "Only 2 means are supported!";
	}
	
	std::cout<<"Random partition\n";
		
	//generate random partition of data
	vector<vector<int>> Partition = RandomPartitionBmp(OldImage, k);

	std::cout<<"First means\n";
		
	//compute k means
	vector<vector<double>> Means = ComputeMeansBmp(OldImage, Partition, k);

	std::cout<<"First error\n";
		
	//compute total error
	double Error = TotalErrorBmp(OldImage, Partition, Means, k);

	bool Check = true;
	//loop:
	while (Check)
	{
		std::cout<<"Computing new partition\n";
			
		vector<vector<int>> NewPartition;
		
		//for each pixel:
		int part;
		int NewSet;
		double NewError;
		for (unsigned int i=0; i<OldImage->imageHeight; i++)
		{
			vector<int> temp;
			NewPartition.push_back(temp);
			
			for (unsigned int j=0; j<OldImage->imageWidth; j++)
			{
				part = Partition[i][j];
				
				//determine closest mean and assign to that group
				NewSet = ClosestMeanBmpNew(OldImage->rgb[i][j].rgbRed, OldImage->rgb[i][j].rgbGreen, OldImage->rgb[i][j].rgbBlue, Means, k);
				NewPartition[i].push_back(NewSet);
			}
		}

		std::cout<<"Computing new means\n";
		
		//compute new k means
		vector<vector<double>> NewMeans = ComputeMeansBmp(OldImage, NewPartition, k);

		std::cout<<"Computing new error\n";

		//compute total error and compare to previous total error 
		NewError = TotalErrorBmp(OldImage, NewPartition, NewMeans, k);

		//break loop if equal or larger, as the error has reached a local min
		if (NewError >= Error)
		{
			Check = false;
		}
		//otherwise set new partition, means, and error
		else
		{
			Partition = NewPartition;
			Means = NewMeans;
			Error = NewError;
		}
	}

	//determine which mean is closest to zero
	vector<double> zeros;
	zeros.push_back(0);
	zeros.push_back(0);
	zeros.push_back(0);

	bool firstlower = false;

	if (ComputeDistanceBmp(zeros, Means[0]) < ComputeDistanceBmp(zeros, Means[1]))
	{
		firstlower = true;
	}
		
	std::cout<<"Starting image conversion\n";
		
	//assign all points with higher mean a value of 255, others 0
	double d1;
	double d2;
	for (unsigned int i=0; i<OldImage->imageHeight; i++)
	{	
		for (unsigned int j=0; j<OldImage->imageWidth; j++)
			{
				d1 = pow(((double)OldImage->rgb[i][j].rgbRed - Means[0][0]), 2) + pow(((double)OldImage->rgb[i][j].rgbGreen - Means[0][1]), 2) + pow(((double)OldImage->rgb[i][j].rgbBlue - Means[0][2]), 2);
				d2 = pow(((double)OldImage->rgb[i][j].rgbRed - Means[1][0]), 2) + pow(((double)OldImage->rgb[i][j].rgbGreen - Means[1][1]), 2) + pow(((double)OldImage->rgb[i][j].rgbBlue - Means[1][2]), 2);

				if (Partition[i][j] == 0)
				{
					if (firstlower)
					{	
						NewImage->rgb[i][j].rgbBlue = 0;
						NewImage->rgb[i][j].rgbGreen = 0;
						NewImage->rgb[i][j].rgbRed = 0;
						NewImage->rgb[i][j].rgbReserved = 0;
					}
					else
					{	
						NewImage->rgb[i][j].rgbBlue = 255;
						NewImage->rgb[i][j].rgbGreen = 255;
						NewImage->rgb[i][j].rgbRed = 255;
						NewImage->rgb[i][j].rgbReserved = 255;
					}
						
				}
				else
				{
					if (firstlower)
					{	
						NewImage->rgb[i][j].rgbBlue = 255;
						NewImage->rgb[i][j].rgbGreen = 255;
						NewImage->rgb[i][j].rgbRed = 255;
						NewImage->rgb[i][j].rgbReserved = 255;
					}
					else
					{	
						NewImage->rgb[i][j].rgbBlue = 0;
						NewImage->rgb[i][j].rgbGreen = 0;
						NewImage->rgb[i][j].rgbRed = 0;
						NewImage->rgb[i][j].rgbReserved = 0;
					}
				}
			}
	}

	//computations for new header
	int padnum1 = (8-(OldImage->imageWidth)%8)%8; //number of additional bits to reach an integer byte count per row
	int NumBytes = (OldImage->imageWidth+padnum1)/8; //number of bytes per row before byte padding but after bit padding
	int bytepad = (4-(NumBytes)%4)%4; //number of additional bytes required for padding
	unsigned int ImgSize1 = (NumBytes + bytepad)*OldImage->imageHeight; //new image data size in bytes
	long hsize1 = ImgSize1 + 54 + 8; //new total file size in bytes
	long newoffset = OldImage->header.bfOffBits + 8; //new offset for the beginning of the image data
	unsigned short bits = 1; //new bits per pixel

	//copy all header and info
	NewImage->header.bfType = OldImage->header.bfType;
	NewImage->header.bfSize = hsize1; //new total file size
    NewImage->header.bfReserved1 = OldImage->header.bfReserved1;
    NewImage->header.bfReserved2 = OldImage->header.bfReserved2;
	NewImage->header.bfOffBits = newoffset; //new image offset

    NewImage->bmiHeader.biSize = OldImage->bmiHeader.biSize;
    NewImage->bmiHeader.biWidth = OldImage->bmiHeader.biWidth;
    NewImage->bmiHeader.biHeight = OldImage->bmiHeader.biHeight;
    NewImage->bmiHeader.biPlanes = OldImage->bmiHeader.biPlanes ;
    NewImage->bmiHeader.biBitCount = bits; //new bits per pixel
    NewImage->bmiHeader.biCompression = OldImage->bmiHeader.biCompression;
	NewImage->bmiHeader.biSizeImage = ImgSize1; //new image data size
    NewImage->bmiHeader.biXPelsPerMeter = OldImage->bmiHeader.biXPelsPerMeter;
    NewImage->bmiHeader.biYPelsPerMeter = OldImage->bmiHeader.biYPelsPerMeter;
    NewImage->bmiHeader.biClrUsed = OldImage->bmiHeader.biClrUsed;
    NewImage->bmiHeader.biClrImportant = OldImage->bmiHeader.biClrImportant;
}

vector<vector<int>> kMeansBmp::RandomPartitionBmp (BMPImage* Img, int k)
{
	//generate an empty MD array
	vector<vector<int>> partition;
	
	//loop through MD array
	int random;
	for (unsigned int i=0; i<Img->imageHeight; i++)
	{
		vector<int> temp;
		partition.push_back(temp);
		
		for (unsigned int j=0; j<Img->imageWidth; j++)
		{
			//compute a random number [0,k-1]
			random  = rand() % k;

			//assign corrresponding pixel the random value
			partition[i].push_back(random);
		}
	}

	return partition;
}

vector<vector<double>> kMeansBmp::ComputeMeansBmp (BMPImage* Img, vector<vector<int>>& Partition, int k)
{
	//initialize result vector
	vector<vector<double>> result;
	for (int a=0; a<k; a++)
	{
		vector<double> temp;
		result.push_back(temp);
			
		for (int c=0; c<3; c++)
		{
			result[a].push_back(0);
		}
	}
	
	//keep a vector with the count of each set in the partition
	vector<int> cardinalities = GetCardinalitiesBmp(Partition, k);

	//loop through image, adding values of each pixel to partition total
	int temp1;
	int temp2;
	int temp3;
	int part;
	for (unsigned int i=0; i<Img->imageHeight; i++)
	{
		for (unsigned int j=0; j<Img->imageWidth; j++)
		{
			temp1 = Img->rgb[i][j].rgbRed;
			temp2 = Img->rgb[i][j].rgbGreen;
			temp3 = Img->rgb[i][j].rgbBlue;
			part = Partition[i][j];

			result[part][0] = result[part][0]+(double)temp1;
			result[part][1] = result[part][1]+(double)temp2;
			result[part][2] = result[part][2]+(double)temp3;
		}
	}

	//compute means
	for (unsigned int b=0; b<result.size(); b++)
	{	
		result[b][0] = result[b][0] / (double)cardinalities[b];
		result[b][1] = result[b][1] / (double)cardinalities[b];
		result[b][2] = result[b][2] / (double)cardinalities[b];
	}

	return result;
}

double kMeansBmp::TotalErrorBmp (BMPImage* Img, vector<vector<int>>& Partition, vector<vector<double>>& Means, int k)
{
	double sum = 0;
	
	//loop through image
	//compute positive difference between mean and actual value and add to total
	int part;
	double diff1;
	for (unsigned int i=0; i<Img->imageHeight; i++)
	{
		for (unsigned int j=0; j<Img->imageWidth; j++)
		{
			part = Partition[i][j];

			diff1 = pow(((double)Img->rgb[i][j].rgbRed - Means[part][0]), 2) + pow(((double)Img->rgb[i][j].rgbGreen - Means[part][1]), 2) + pow(((double)Img->rgb[i][j].rgbBlue - Means[part][2]), 2);
			sum += diff1;
		}
	}

	return sum;
}


double kMeansBmp::ComputeDistanceBmp (vector<double>& x, vector<double>& y)
{
	if (x.size() != y.size() || x.size() == 0 || y.size() == 0)
	{
		//throw exception
		throw "Vectors must be non-empty and must have the same number of elements!";
	}
		
	double sum = 0;
	//loop through each element, take difference between vectors
	for (unsigned int i = 0; i < x.size(); i++)
	{
		sum += abs(x[i] - y[i]);
	}

	return sum;
}

vector<int> kMeansBmp::GetCardinalitiesBmp (vector<vector<int>>& Partition, int k)
{
	//initialize result vector
	vector<int> result;
	for (int a=0; a<k; a++)
	{
		result.push_back(0);
	}
	
	//loop through partition and increment the cardinality of the given set when a member appears
	int temp;
	for (unsigned int i=0; i<Partition.size(); i++)
	{
		for (unsigned int j=0; j<Partition[i].size(); j++)
		{
			temp = Partition[i][j];
			result[temp] = result[temp]+1;
		}
	}

	return result;
}



/*int kMeansBmp::ClosestMeanBmp (vector<double>& Image, vector<vector<double>>& Means, int k)
{
	double closest = 1000;
	int result;

	//loop through means, computing the distance of the given pixel to each
	for (unsigned int i=0; i<Means.size(); i++)
	{
		double diff = ComputeDistanceBmp(Image, Means[i]);
		//if distance is smaller, save that mean
		if (diff < closest)
		{
			closest = diff;
			result = i;
		}
	}

	return result;
}*/

int kMeansBmp::ClosestMeanBmpNew (int r, int g, int b, vector<vector<double>>& Means, int k)
{
	int result = 0;

	double closest = pow((Means[0][0] - (double)r), 2) + pow((Means[0][1] - (double)g), 2) + pow((Means[0][2] - (double)b), 2);

	//loop through means, computing the distance of the given pixel to each
	double diff;
	for (unsigned int i=1; i<Means.size(); i++)
	{
		diff = pow((Means[i][0] - (double)r), 2) + pow((Means[i][1] - (double)g), 2) + pow((Means[i][2] - (double)b), 2);
		//if distance is smaller, save that mean
		if (diff < closest)
		{
			closest = diff;
			result = i;
		}
	}

	return result;
}