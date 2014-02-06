#include <kaldi-io.h>
  
KaldiArchive::KaldiArchive(): _dim(0) {
}

KaldiArchive::KaldiArchive(string filename): _dim(0) {
  this->read(filename);
}

size_t KaldiArchive::getFeatureDimension(istream& is) {

  size_t pos = is.tellg();
  
  string line;
  getline(is, line);
  getline(is, line);
  line = trim(line);

  is.seekg(-pos, is.cur);

  return split(line, ' ').size();
}

void KaldiArchive::read(istream& is) {

  _dim = 0;
  _data.clear();
  _offset.clear();
  _docid.clear();

  string line;

  while (getline(is, line)) {
    if (line.find("[") != string::npos) {
      _docid.push_back(split(trim(line), ' ')[0]);
      _offset.push_back(_data.size());
      continue;
    }

    if (line.find("]") != string::npos)
      line.resize(line.find("]"));

    float x;
    stringstream ss(line);

    if (_dim == 0)
      _dim = split(trim(line), ' ').size();

    while (ss >> x)
      _data.push_back(x);
  }
  _offset.push_back(_data.size());
}

void KaldiArchive::read(string filename) {
  if (filename == "-")
    this->read(cin);
  else {
    ifstream fin(filename.c_str());
    this->read(fin);
    fin.close();
  }
}

void KaldiArchive::write(string filename) {

  if (filename == "-")
    this->write(cout);
  else {
    ofstream fout(filename.c_str());
    this->write(fout);
    fout.close();
  }
}

void KaldiArchive::write(ostream& os) {
  for (size_t i=0; i<_docid.size(); ++i) {
    os << _docid[i] << " [";

    size_t t = (_offset[i+1] - _offset[i]) / _dim;

    for (size_t j=0; j<t; ++j) {
      os << "\n  ";
      for (size_t k=0; k<_dim; ++k)
	os << _data[_offset[i] + j*_dim + k] << " ";
    }
    os << "]\n";
  }
}

vector<string>& split(const string &s, char delim, vector<string>& elems) {
  stringstream ss(s);
  string item;
  while(getline(ss, item, delim))
    elems.push_back(item);
  return elems;
}

vector<string> split(const string &s, char delim) {
  vector<string> elems;
  return split(s, delim, elems);
}
