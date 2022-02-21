#include "The-Tips.h"
#include <cstring>
#include <emmintrin.h>

/* Nicholas Creech
 * 11/28/21
 * Lab 5: Part 1
 */ 

double TheTips::solve(vector <string> Cl, vector <int> probability, int print)
{
  int i, j, v;
  uint64_t **C;
  vector < vector < __m128i > > M;
  vector <double> p;
  double x;
  uint64_t val, one;
  int numLongs; //Number of longs (64 bits) needed for each row

  C = (uint64_t**)malloc(Cl.size() * sizeof(uint64_t*));
  M.resize(Cl.size());
  numLongs = (Cl.size() % 64 == 0) ? Cl.size() / 64 : Cl.size() / 64 + 1;
  one = 1;
  
  /* Change the Y/N's to 1/0's */
  for (i = 0; i < Cl.size(); i++) 
  {
	  C[i] = (uint64_t*)malloc(numLongs * sizeof(uint64_t));
	  M[i].resize(numLongs);

	  for (j = 0; j < Cl.size(); j++) 
	  {
		  //Initialize each long
		  if(j % 64 == 0)
			  C[i][j / 64] = 0;

		  if(Cl[i][j] == 'Y') 
			  C[i][j / 64] |= (one << (63 - (j % 64)));
	  }

	  C[i][i / 64] |= (one << (63 - (i % 64)));
	
	  //Convert all uint64_t to __m128i
	  for(j = 0; j < Cl.size(); j++)
	  {
		  if(j % 64 == 0)
			  M[i][j / 64] = _mm_loadl_epi64((__m128i*)(&C[i][j / 64]));
	  }
  }
  
  /* Print the adjacency matrix before doing Floyd-Warshall */
  if (print) 
  {
	  printf("The Adjacency Matrix:\n\n");
	  printf("    ");
	  
	  for (i = 0; i < Cl.size(); i++) 
		  printf("%X", i&0xf);
	  
	  printf("\n");
	  printf("   -");
	  
	  for (i = 0; i < Cl.size(); i++) 
		  printf("-");
    
	  printf("\n");
	  
	  for (i = 0; i < Cl.size(); i++) 
	  {
		  printf("%X | ", i&0xf);
		  for (j = 0; j < numLongs; j++)
		  {
			  memcpy(&val, &M[i][j], sizeof(val));
			  printf("%lu", val);
		  }
	  
		  printf("\n");
	  }
	  
	  printf("\n");
  }


  //Do Floyd-Warshall
  for (v = 0; v < Cl.size(); v++) 
  {
	  for (i = 0; i < Cl.size(); i++) 
	  {
		  //Convert __m128i to uint64_t for long that holds current bit v
		  memcpy(&val, &M[i][v / 64], sizeof(val));

		  //Check if bit v is one
		  if ((val >> (63 - (v % 64))) & one) 
		  {
			  //OR all longs for the row using SIMD instructions
			  for(j = 0; j < numLongs; j++)
				  M[i][j] = _mm_or_si128(M[i][j], M[v][j]);
		  }
	  }
  }

  if (print) 
  {
	  printf("Probabilities:\n\n");
	  
	  for (i = 0; i < probability.size(); i++) 
		  printf("%4d", probability[i]);
	  
	  printf("\n\n");
	  printf("The Floyd-Warshall Matrix:\n\n");
  
	  printf("    ");
	  
	  for (i = 0; i < Cl.size(); i++) 
		  printf("%X", i&0xf);
	  
	  printf("\n");
	  printf("   -");
	  
	  for (i = 0; i < Cl.size(); i++) 
		  printf("-");
	  
	  printf("\n");
	  
	  for (i = 0; i < Cl.size(); i++) 
	  {
		  printf("%X | ", i&0xf);

		  for (j = 0; j < numLongs; j++)
		  {
			  memcpy(&val, &M[i][j], sizeof(val));
			  printf("%lu", val);
		  }
	  
		  printf("\n");
	  }
	  
	  printf("\n");
  }

  /* Calculate the values of p from the probabilities and reachability: */
  p.resize(Cl.size(), 0);

  for (i = 0; i < Cl.size(); i++) 
  {
	  x = probability[i];
	  x /= 100.0;
 
	  for (j = 0; j < Cl.size(); j++) 
	  {
		  //Convert __m128i to uint64_t for long that holds current bit j
		  memcpy(&val, &M[i][j / 64], sizeof(val));
		  
		  //Check if bit j is one
		  if ((val >> (63 - (j % 64))) & one)
			  p[j] += ((1 - p[j]) * x);
	  }
  }

  if (print) 
  {
	  printf("\nThe Expected Values\n\n");
	  
	  for (i = 0; i < Cl.size(); i++) 
		  printf("I: %X    Prob: %7.5lf\n", i, p[i]);

	  printf("\n");
  }
    
  /* Calculate the final return value */
  x = 0;
  for (i = 0; i < Cl.size(); i++) 
	  x += p[i];

  for(i = 0; i < Cl.size(); i++)
	delete[] C[i];

  delete[] C;
  return x;
}
