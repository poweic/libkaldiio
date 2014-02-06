#include <cmdparser.h>
#include <fstream>
#include <kaldi-io.h>

map<string, vector<int> > readLabels(const string& filename);

int main (int argc, char* argv[]) {

  CmdParser cmd(argc, argv);
  cmd.addGroup("Input / Output:")
     .add("--ark", "filename of input feature archive (in Kaldi format)")
     .add("--label", "filename of input label", false)
     .add("--svm", "filename of output (in LibSVM format)", false);

  cmd.addGroup("Options:")
     .add("-i", "ignore missing labels (set those to 0)", "false");

  cmd.addGroup("Example: ./kaldi-to-svm --ark data/example.39.ark -svm example.dat");

  if(!cmd.isOptionLegal())
    cmd.showUsageAndExit();

  string input_fn = cmd["--ark"];
  string label_fn = cmd["--label"];
  string output_fn= cmd["--svm"];
  bool ignore	  = cmd["-i"];

  map<string, vector<int> > labels = readLabels(label_fn);

  size_t N, dim; float* data; size_t* offset;
  vector<string> docids = readKaldi(input_fn, data, offset, N, dim); 

  FILE* fid = output_fn.empty() ? stdout : fopen(output_fn.c_str(), "w");

  for (size_t i=0; i<N; ++i) {
    size_t length = (offset[i+1] - offset[i]) / dim;
    for (size_t j=0; j<length; ++j) {

      int y = 0;

      if (labels.count(docids[i]) > 0)
	y = labels[docids[i]][j];
      else {
	if (ignore)
	  continue;
      }

      fprintf(fid, "%d ", y);

      for (size_t k=0; k<dim; ++k) {
	float x = data[offset[i] + j*dim + k];
	if (x != 0)
	  fprintf(fid, "%lu:%g ", k + 1, x);
      }
      fprintf(fid, "\n");
    }
  }

  if (fid != stdout)
    fclose(fid);

  return 0;
}

map<string, vector<int> > readLabels(const string& filename) {

  map<string, vector<int> > labels;

  if (filename.empty())
    return labels;

  ifstream fin(filename.c_str());

  string line;
  while (getline(fin, line)) {
    vector<string> tokens = split(line, ' ');

    string docid = tokens[0];

    vector<int>& L = labels[docid];
    L.resize(tokens.size() - 1);

    for (size_t i=0; i<L.size(); ++i)
      L[i] = std::stoi(tokens[i+1]);
  }

  fin.close();

  return labels;
}

