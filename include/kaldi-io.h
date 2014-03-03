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

class KaldiArchive {
public:
  KaldiArchive();
  KaldiArchive(string filename);

  bool is_binary(istream& is);

  void read(istream& is);
  void read(string filename);
  void readBinary(istream& is);
  void readText(istream& is);

  void write(ostream& os);
  void write(string filename);
  size_t getFeatureDimension(istream& is);

  const vector<float>& data() const { return _data; }
  const vector<size_t>& offset() const { return _offset; }
  const vector<string>& docid() const { return _docid; }

  size_t dim() const { return _dim; }

private:
  vector<float> _data;
  vector<size_t> _offset;
  vector<string> _docid;
  size_t _dim;
};


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

string peek_a_line(istream& is);

vector<string> split(const string &s, char delim);
vector<string>& split(const string &s, char delim, vector<string>& elems);

#endif
