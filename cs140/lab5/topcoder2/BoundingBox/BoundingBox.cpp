#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
using namespace std;

class BoundingBox {
  public:
    int smallestArea(const vector <int> &X, const vector <int> &Y);
};


int BoundingBox::smallestArea(const vector <int> &X, const vector <int> &Y)
{
	int minX = X[0], maxX = X[0];
	int minY = Y[0], maxY = Y[0];
	for(int i = 0; i < X.size(); i++)
	{
		if(X[i] < minX)
			minX = X[i];
		else if(X[i] > maxX)
			maxX = X[i];
		if(Y[i] < minY)
			minY = Y[i];
		else if(Y[i] > maxY)
			maxY = Y[i];
	}

	return (maxX - minX) * (maxY - minY);
}
