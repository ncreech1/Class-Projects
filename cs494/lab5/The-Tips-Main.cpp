#include "The-Tips.h"
#include <string.h>
#include <signal.h>
#include <unistd.h>

void alarm_handler(int dummy)
{
  fprintf(stderr, "Timed Out!\n");
  exit(0);
}

int main(int argc, char **argv)
{
  int i, j, k;
  int N, E;
  int print;
  class TheTips TheClass;
  double retval;
  vector <string> clues;
  vector <int> probability;

  signal(SIGALRM, alarm_handler);
  alarm(4);
  if (argc == 2) {

     if (atoi(argv[1]) == 0) {
        clues.push_back("Y");
        probability.push_back(50);
      }
    
     if (atoi(argv[1]) == 1) {
        clues.push_back("YN");
        clues.push_back("NY");
        probability.push_back(100);
        probability.push_back(50);
      }
    
     if (atoi(argv[1]) == 2) {
        clues.push_back("YYY");
        clues.push_back("NYY");
        clues.push_back("NNY");
        probability.push_back(100);
        probability.push_back(0);
        probability.push_back(0);
      }
    
     if (atoi(argv[1]) == 3) {
        clues.push_back("NNN");
        clues.push_back("NNN");
        clues.push_back("NNN");
        probability.push_back(0);
        probability.push_back(0);
        probability.push_back(0);
      }
    
     if (atoi(argv[1]) == 4) {
        clues.push_back("NYYNYYNNNN");
        clues.push_back("NNNNYNNNYN");
        clues.push_back("YNNYYYYYNN");
        clues.push_back("YYNYNNNNYN");
        clues.push_back("NYNNNNNNNY");
        clues.push_back("YNYYNNYNNY");
        clues.push_back("NYNNYYYYYY");
        clues.push_back("NYYYYNNNNN");
        clues.push_back("YYNYNNYYYN");
        clues.push_back("NNYYNYNYYY");
         probability.push_back(11);
         probability.push_back(66);
         probability.push_back(99);
         probability.push_back(37);
         probability.push_back(64);
         probability.push_back(45);
         probability.push_back(21);
         probability.push_back(67);
         probability.push_back(71);
         probability.push_back(62);
      }
    
     if (atoi(argv[1]) == 5) {
        clues.push_back("NNY");
        clues.push_back("NNY");
        clues.push_back("NNN");
        probability.push_back(50);
        probability.push_back( 50);
        probability.push_back( 1);
      }
    

      print = 1;
  } else if (argc != 5 || (strcmp(argv[4], "Y") != 0 && strcmp(argv[4], "N") != 0)) { 
    fprintf(stderr, "usage: a.out nnodes nedges seed print(Y|N)\n"); 
    exit(1); 
  } else {
    srandom(time(0));
    N = atoi(argv[1]);
    E = atoi(argv[2]);
    srandom(atoi(argv[3]));
    clues.resize(N);
    for (i = 0; i < N; i++) clues[i].resize(N, 'N');
    for (i = 0; i < E; i++) {
      clues[random()%N][random()%N] = 'Y';
    }
    for (i = 0; i < N; i++) probability.push_back(random()%10+1);
    for (k = 20; k < N; k += 20) {
      i = random()%N;
      for (j = 0; j < clues[i].size(); j++) clues[i][j] = 'N';
    }
    print = (argv[4][0] == 'Y');
  }

  retval = TheClass.solve(clues, probability, print);
  printf("%.10lf\n", retval);

  exit(0);
}
