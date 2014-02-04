#ifndef _KALDI_IO_H_
#define _KALDI_IO_H_

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <map>
#include <locale>
#include <sstream>
using namespace std;

// trim from start
static inline string ltrim(string s) {
  s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
  return s;
}

// trim from end
static inline string rtrim(string s) {
  s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
  return s;
}

// trim from both ends
static inline string trim(string s) {
  return ltrim(rtrim(s));
}

vector<string> split(const string &s, char delim);
vector<string>& split(const string &s, char delim, vector<string>& elems);

vector<string> readKaldi(string filename, float* &dataPtr, size_t* &offsetPtr, size_t& N, size_t& dim);

void print(FILE* fid, float* m, int N);

#endif
