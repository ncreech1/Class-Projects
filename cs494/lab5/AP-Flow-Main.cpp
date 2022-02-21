#include "AP-Flow.h"
#include "MOA.h"
#include <string.h>
#include <signal.h>
#include <unistd.h>

void alarm_handler(int dummy)
{
  fprintf(stderr, "Timed Out!\n");
  exit(0);
}

unsigned int djb_hash(uint8_t *buf, int size)
{
  size_t i;
  unsigned int h;

  h = 5381;

  for (i = 0; i < size; i++) {
    h = (h << 5) + h + buf[i];
  }
  return h;
}


int main(int argc, char **argv)
{
  APFlow A;
  int N, i, j, v;
  unsigned int H;

  if (argc != 4) {
    fprintf(stderr, "usage: AP-Flow nodes seed(- to read from stdin) print(Y|N|H)\n");
    exit(1);
  }

  signal(SIGALRM, alarm_handler);
  alarm(4);
  N = atoi(argv[1]);
  if (N <= 0) return 1;
  A.N = N;

  A.Adj =  (uint8_t *) malloc(N*N*sizeof(uint8_t));
  A.Flow = (uint8_t *) malloc(N*N*sizeof(uint8_t));
  if (A.Flow == NULL) { perror("malloc"); exit(1); }

  if (strcmp(argv[2], "-") == 0) {
    for (i = 0; i < N*N; i++) {
      if (!(cin >> v)) {
        fprintf(stderr, "Didn't read entire adjacency matrix.\n");
        exit(1);
      }
      A.Adj[i] = v;
    }
  } else {
    MOA_Seed(atoi(argv[2]));
    for (i = 0; i < N*N; i++) {
      if (MOA_Random_W(4, 1) == 0) {
        A.Adj[i] = MOA_Random_W(8, 1);
      } else {
        A.Adj[i] = MOA_Random_W(7, 1);
      }
    }
  }
  
  for (i = 0; i < N; i++) A.Adj[i*N+i] = 255;

  if (argv[3][0] == 'Y') {
    printf("Adjacency Matrix:\n\n");
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) printf("%4d", A.Adj[i*N+j]);
      printf("\n");
    }
  }
  
  A.CalcFlow();

  if (argv[3][0] == 'Y') {
    printf("\nFlow Matrix:\n\n");
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) printf("%4d", A.Flow[i*N+j]);
      printf("\n");
    }
  } 

  if (argv[3][0] == 'H') {
    H = djb_hash(A.Flow, N*N*sizeof(uint8_t));
    printf("%08X\n", H);
  }

  exit(0);
}
