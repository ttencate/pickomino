#include "maths.h"

int nChooseK(int n, int k) {
  int out = 1;
  for (int i = 1; i < k; ++i) {
    out *= (n + 1 - i) / i;
  }
  return out;
}

long int factorial(int n) {
  long int f = 1;
  for (; n; --n) {
    f *= n;
  }
  return f;
}

long int power(int n, int k) {
  long int p = 1;
  for (; k; --k) {
    p *= n;
  }
  return p;
}
