#include "../include/misc.h"

double Error(double sum, double sum2, int n) {
  if (n == 1)
    return 0;
  return sqrt(fabs(sum2 / (double)n - pow(sum / (double)n, 2)) / (double)(n-1));
}

void FileCheck(ofstream &file, string path) {
  if (!file) {
    cerr << "ERROR: unable to open file " + path << endl;
    exit(1);
  }
}

void FileCheck(ifstream &file, string path) {
  if (!file) {
    cerr << "ERROR: unable to open file " + path << endl;
    exit(1);
  }
}
