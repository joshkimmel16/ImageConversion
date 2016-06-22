#pragma once

#include "bmpimage.h"
#include <vector>

using namespace std;

//data structure for bitmap
class kMeansBmp
{
private:
    vector<vector<int>> RandomPartitionBmp (BMPImage* Img, int k); //generate random partition of pixels
	vector<vector<double>> ComputeMeansBmp (BMPImage* Img, vector<vector<int>>& Partition, int k); //compute the mean of each set in the given partition
	double TotalErrorBmp (BMPImage* Img, vector<vector<int>>& Partition, vector<vector<double>>& Means, int k); //compute the total error given a set of means and a partition
	double ComputeDistanceBmp (vector<double>& x, vector<double>& y); //given two vectors, compute the distance between them
	vector<int> GetCardinalitiesBmp (vector<vector<int>>& Partition, int k); //given a partition, compute the number of pixels in each set
	//int ClosestMeanBmp (vector<double>& Image, vector<vector<double>>& Means, int k); //given a pixel, return the index of the closest mean to that pixel
	int ClosestMeanBmpNew (int r, int g, int b, vector<vector<double>>& Means, int k); //given a pixel, return the index of the closest mean to that pixel
public:
	void kMeansExecute (BMPImage* OldImage, BMPImage* NewImage, int k); //run the k-means algorithm on the old image and copy into the result image
};
