#include <cmdparser.h>
#include <fstream>
#include <cstdio>
#include <kaldi-io.h>

map<string, vector<int> > readLabels(const string& filename);

int main (int argc, char* argv[]) {

  CmdParser cmd(argc, argv);
  cmd.add("kaldi-ark-in")
     .add("label-in")
     .add("svm-out", false)
     .add("mapping-out", false);

  cmd.addGroup("Options:")
     .add("-i", "ignore missing labels (set those to 0)", "false");

  cmd.addGroup("Example: ./kaldi-to-svm data/example.39.ark example.labels");

  if(!cmd.isOptionLegal())
    cmd.showUsageAndExit();


  string input_fn   = cmd[1];
  string label_fn   = cmd[2];
  string output_fn  = cmd[3];
  string mapping_fn = cmd[4];
  bool ignore	  = cmd["-i"];

  map<string, vector<int> > labels = readLabels(label_fn);

  KaldiArchive ark(input_fn);

  size_t N = ark.docid().size();
  size_t dim = ark.dim();
  const vector<string>& docids = ark.docid();
  const vector<float>& data = ark.data();
  const vector<size_t>& offset = ark.offset();

  FILE* fid = (output_fn.empty() || output_fn == "-") ? stdout : fopen(output_fn.c_str(), "w");

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

  if (!mapping_fn.empty()) {
    ofstream fout(mapping_fn.c_str());

    for (size_t i=0; i<docids.size(); ++i)
      fout << docids[i] << " " << (offset[i+1] - offset[i]) / dim << endl;
    fout.close();
  }

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

