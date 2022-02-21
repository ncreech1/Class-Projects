/* These are wrappers around the 
   "Mother of All" random number generator from http://www.agner.org/random/ */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t MOA_Random_32();                    /* Returns a random 32-bit number */
uint64_t MOA_Random_64();                    /* Returns a random 64-bit number */
void     MOA_Random_128(uint64_t *x);        /* Returns a random 128-bit number */
uint32_t MOA_Random_W(int w, int zero_ok);   /* Returns a random w-bit number. (w <= 32)*/
void MOA_Fill_Random_Region (void *reg, int size);   /* reg should be aligned to 4 bytes, but
                                                        size can be anything. */
void     MOA_Seed(uint32_t seed);            /* Seed the RNG */
