#include "AP-Flow.h"
#include <emmintrin.h>

/* Nicholas Creech
 * 11/28/21
 * Lab 5: Part 2
 */ 

void APFlow::CalcFlow()
{
	int i, j, v, f;
	uint8_t *alli;
	uint8_t *vv;
	uint8_t *iv;
	uint8_t *fv;
	uint8_t *rv;
	__m128i *bflow;
	__m128i *balli;
	__m128i *bvv;
	__m128i *biv;
	__m128i *bfv;
	__m128i *brv;

	//Allocate memory for the necessary arrays
	alli = (uint8_t*)malloc(16 * sizeof(uint8_t));
	vv = (uint8_t*)malloc(16 * sizeof(uint8_t));
	iv = (uint8_t*)malloc(16 * sizeof(uint8_t));
	fv = (uint8_t*)malloc(16 * sizeof(uint8_t));
	rv = (uint8_t*)malloc(16 * sizeof(uint8_t));

	//Create pointers to access uint8_t arrays as __m128i type
	balli = (__m128i*)alli;
	bvv = (__m128i*)vv;
	biv = (__m128i*)iv;
	bfv = (__m128i*)fv;
	brv = (__m128i*)rv;

	for (i = 0; i < N*N; i++) 
		Flow[i] = Adj[i];

	for (v = 0; v < N; v++) 
	{
		for (i = 0; i < N; i++) 
		{
			/* Create a vector alli, which is 16 instances of Flow[i*N+v] */
			*balli = _mm_set1_epi8(Flow[i * N + v]);

			for (j = 0; j < N; j += 16) 
			{
				/* Load Flow[v*N+j] through Flow[v*N+j+15] to vector vv */
				bflow = (__m128i*)(Flow + (v * N + j));
				*bvv = _mm_load_si128(bflow);

				/* Create fv, which is the flow from i to each of j through j+15
				 * through v. This is simply the min of alli and vv. 
				 */
				*bfv = _mm_min_epu8(*balli, *bvv);
	
				/* Load Flow[i*N+j] through Flow[i*N+j+15] to vector iv */
				bflow = (__m128i*)(Flow + (i * N + j));
				*biv = _mm_load_si128(bflow);

				/* Create rv, which is the max of each value of fv and iv. */
				*brv = _mm_max_epu8(*bfv, *biv);

				/* Store rv into Flow[i*N+j] through Flow[i*N+j+15] */
				bflow = (__m128i*)(Flow + (i * N + j));
				*bflow = _mm_load_si128(brv);
			}   
		}
	}

	delete[] alli;
	delete[] vv;
	delete[] iv;
	delete[] fv;
	delete[] rv;
}
