#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>
#include <cstdio>
#include <stdint.h>
#include <cstdlib>
using namespace std;

class APFlow {
  public:
    int N;
    uint8_t *Adj;
    uint8_t *Flow;
    void CalcFlow();
};
