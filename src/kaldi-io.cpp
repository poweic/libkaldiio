#include <kaldi-io.h>

vector<string> readKaldi(string filename, float* &dataPtr, size_t* &offsetPtr, size_t& N, size_t& dim) {

  vector<string> docid;

  FILE* f = fopen(filename.c_str(), "r");
  
  if (f == NULL)
    return docid;

  size_t line_buffer = 65536;
  char line[line_buffer];

#pragma GCC diagnostic ignored "-Wunused-result"
  fgets(line, line_buffer, f);
  fgets(line, line_buffer, f);

  dim = 0;
  char* tok = strtok(line, " ");
  do { ++dim; } while (strtok(NULL, " ")[0] != '\n');

  fseek(f, 0, SEEK_SET);

  vector<float> data;
  vector<size_t> offset;

  while(fgets(line, line_buffer, f)) {

    if (string(line).find("[") != string::npos) {
      docid.push_back(strtok(line, " "));
      offset.push_back(data.size());
      continue;
    }

    tok = strtok(line, " ");
    data.push_back(atof(tok));
    while((tok = strtok(NULL, " "))) {
      if (tok[0] == ']' || tok[0] == '\n')
	break;
      data.push_back(atof(tok));
    }
  }
  offset.push_back(data.size());

  dataPtr = new float[data.size()];
  memcpy(dataPtr, data.data(), sizeof(float) * data.size());

  offsetPtr = new size_t[offset.size()];
  memcpy(offsetPtr, offset.data(), sizeof(size_t) * offset.size());

  N = offset.size() - 1;

  fclose(f);

  return docid;
}

void print(FILE* fid, float* m, int N) {
  for (int i=0; i<N; ++i) {
    for (int j=0; j<N; ++j)
      fprintf(fid, "%.6f ", m[i * N + j]);
    fprintf(fid, "\n");
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
