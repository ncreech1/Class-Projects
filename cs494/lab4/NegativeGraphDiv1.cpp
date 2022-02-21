#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <cmath>

using namespace std;

/* Nicholas Creech
 * 11/15/21
 * CS494 Lab4
 */

int main(int argc, char **argv)
{
	vector<vector<long long>> adj;
	vector<vector<long long>> S;
	vector<vector<long long>> L;
	vector<vector<long long>> A;
	vector<vector<long long>> ACurrent;
	vector<vector<vector<long long>>> ACharges;
	vector<int> from;
	vector<int> to;
	vector<int> weights;
	int charges, nodes;
	string line, word;
	int param;
	int currCharges;

	if(argc > 2)
	{
		cout << "usage: ./NegativeGraphDiv1 <P0 | P1>" << endl;
		return -1;
	}

	//Read in graph
	param = 0;
	while(getline(cin, line))
	{
		stringstream ss(line);

		while(ss >> word)
		{
			if(param == 0)
				nodes = stoi(word);
			if(param == 1)
				from.push_back(stoi(word));
			if(param == 2)
				to.push_back(stoi(word));
			if(param == 3)
				weights.push_back(stoi(word));
			if(param == 4)
				charges = stoi(word);
		}

		param++;
	}

	//Initialize adjacency matrices
	for(int i = 0; i < nodes; i++)
	{
		vector<long long> inner;
		adj.push_back(inner);
		S.push_back(inner);
		L.push_back(inner);
		A.push_back(inner);
		adj[i].resize(nodes, 6000000);
		S[i].resize(nodes, 0);
		L[i].resize(nodes, 0);
		A[i].resize(nodes, 6000000);
	}

	//Find smallest and largest weight edges from each node to each other node
	for(int i = 0; i < nodes; i++)
	{
		for(int j = 0; j < from.size(); j++)
		{
			//Found an edge from current node
			if(from[j] == i + 1)
			{
				//Set smallest weight for node from[j] to node to[j]
				if(weights[j] < adj[i][to[j] - 1])
					adj[i][to[j] - 1] = weights[j];
				
				//Set largest weight for node from[j] to node to[j]
				if(weights[j] > L[i][to[j] - 1])
					L[i][to[j] - 1] = weights[j];
			}
		}
	}

	//Print adjacency matrix
	if(argc == 2 && strcmp(argv[1], "P0") == 0)
	{
		for(int i = 0; i < adj.size(); i++)
		{
			for(int j = 0; j < adj[i].size(); j++)
				printf("%7d ", adj[i][j]);
			
			cout << endl;
		}
	}

	//Copy initial adjacency matrix to S
	for(int i = 0; i < nodes; i++)
	{
		for(int j = 0; j < nodes; j++)
			S[i][j] = adj[i][j];

		S[i][i] = 0;
	}

	//Perform Floyd-Warshall on S
	for(int i = 0; i < nodes; i++)
	{
		for(int x = 0; x < nodes; x++)
		{
			for(int y = 0; y < nodes; y++)
			{
				if(S[x][i] + S[i][y] < S[x][y])
					S[x][y] = S[x][i] + S[i][y];
			}
		}	
	}

	//Print Floyd-Warshall matrix S
	if(argc == 2 && strcmp(argv[1], "P1") == 0)
	{
		for(int i = 0; i < S.size(); i++)
		{
			for(int j = 0; j < S[i].size(); j++)
				printf("%7d ", S[i][j]);
			
			cout << endl;
		}
	}

	//Print solution path from S if no charges to be used
	if(charges == 0)
	{
		cout << S[0][nodes - 1] << endl;
		return 0;
	}

	//Create shortest paths matrix with one charge used (A)
	for(int f = 0; f < nodes; f++)
	{
		for(int t = 0; t < nodes; t++)
		{
			for(int i = 0; i < nodes; i++)
			{
				for(int j = 0; j < nodes; j++)
				{
					if(L[i][j] != 0 || i == j)
					{
						if(S[f][i] + S[j][t] + L[i][j] * -1 < A[f][t])
							A[f][t] = S[f][i] + S[j][t] + L[i][j] * -1;
					}
				}
			}
		}
	}

	//Print shortest paths matrix with one charge used (A)
	if(argc == 2 && strcmp(argv[1], "P2") == 0)
	{
		for(int i = 0; i < A.size(); i++)
		{
			for(int j = 0; j < A[i].size(); j++)
				printf("%7d ", A[i][j]);
			
			cout << endl;
		}
	}

	//Create list of A matrices
	ACharges.push_back(A);
	for(int w = 0; w < charges; w++)
	{
		//Find A matrices up to closest power of two
		if(pow(2, w + 1) > charges)
			break;

		//Intialize new A for a power of 2
		ACurrent.clear();
		for(int i = 0; i < nodes; i++)
		{
			vector<long long> inner;
			ACurrent.push_back(inner);
			ACurrent[i].resize(nodes, 6000000);
		}

		//Perform Floyd-Warshall-like algorithm to generate new A
		for(int i = 0; i < nodes; i++)
		{
			for(int x = 0; x < nodes; x++)
			{
				for(int y = 0; y < nodes; y++)
				{
					if(ACharges[w][x][i] + ACharges[w][i][y] < ACurrent[x][y])
						ACurrent[x][y] = ACharges[w][x][i] + ACharges[w][i][y];
				}
			}		
		}

		//Add new A to list
		ACharges.push_back(ACurrent);
	}

	//Find final A matrix
	currCharges = pow(2, ACharges.size() - 1);
	A = ACharges.back();
	for(int w = ACharges.size() - 2; w >= 0; w--)
	{
		//Add A matrices until Acharges is reached
		if(currCharges + pow(2, w) <= charges)
		{
			//Intialize new A
			ACurrent.clear();
			for(int i = 0; i < nodes; i++)
			{
				vector<long long> inner;
				ACurrent.push_back(inner);
				ACurrent[i].resize(nodes, 6000000);
			}

			//Perform Floyd-Warshall-like algorithm to generate new A
			for(int f = 0; f < nodes; f++)
			{
				for(int t = 0; t < nodes; t++)
				{
					for(int z = 0; z < nodes; z++)
					{
						if(A[z][t] + ACharges[w][f][z] < ACurrent[f][t])
							ACurrent[f][t] = A[z][t] + ACharges[w][f][z];
					}
				}			
			}

			A = ACurrent;
			currCharges += pow(2, w);
		}

		//Final A found
		if(currCharges == charges)
			break;
	}

	//Print final A matrix
	if(argc == 2 && strcmp(argv[1], "P3") == 0)
	{
		for(int i = 0; i < A.size(); i++)
		{
			for(int j = 0; j < A[i].size(); j++)
				printf("%7d ", A[i][j]);
			
			cout << endl;
		}
	}

	//Print shortest path from node 0 to N
	cout << A[0][nodes - 1] << endl;

	return 0;
}
