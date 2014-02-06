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

string read_docid(istream& is) {
  char buffer[256];
  is.read(buffer, 1);

  if (!is)
    return "";

  size_t n = 0;
  while(buffer[n] != ' ') {
    ++n;
    is.read(buffer + n, 1);
  }
  buffer[n] = '\0';
  return string(buffer);
}

bool KaldiArchive::is_binary(istream& is) {

  string docid = read_docid(is);

  char s[2]; 
  is.read(s, 2);

  is.putback(s[1]);
  is.putback(s[0]);

  is.putback(' ');
  for (int i=docid.size() - 1; i >=0; --i)
    is.putback(docid[i]);

  return (s[0] == '\0') && (s[1] == 'B');
}

void KaldiArchive::readBinary(istream& is) {

  _offset.push_back(0);
  while (true) {
    string docid = read_docid(is);

    if (docid.empty())
      break;
    _docid.push_back(docid);

    char s[2]; 
    is.read(s, 2);

    char c;
    is.read(&c, 1);
    is.read(&c, 1);
    is.read(&c, 1);
    is.read(&c, 1);

    int frame;
    is.read((char*) &frame, 4);

    is.read(&c, 1);

    int dim;
    is.read((char*) &dim, 4);
    _dim = (size_t) dim;

    float x;
    for(int i = 0; i < frame; i++) {
      for(int j = 0; j < dim; j++) {
	is.read((char*) &x, sizeof(float));
	_data.push_back(x);
      }
    }

    _offset.push_back(_data.size());
  }
}

void KaldiArchive::readText(istream& is) {

  string line;
  while (getline(is, line)) {
    if (line.find("[") != string::npos) {
      string docid = split(trim(line), ' ')[0];
      _docid.push_back(docid);
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

void KaldiArchive::read(istream& is) {

  _dim = 0;
  _data.clear();
  _offset.clear();
  _docid.clear();


  if (is_binary(is))
    readBinary(is);
  else
    readText(is);
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
