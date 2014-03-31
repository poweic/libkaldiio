#include <cmdparser.h>
#include <fstream>
#include <cstdio>
#include <kaldi-io.h>

void reformat(istream& in, const vector<string>& docids, const vector<int>& frames);

int main (int argc, char* argv[]) {

  CmdParser cmd(argc, argv);
  cmd.add("raw-data-in")
     .add("mapping-in")
     .add("kaldi-out", false);

  cmd.addGroup("Example: ./raw-to-kaldi data/example.39.dense mapin");

  if(!cmd.isOptionLegal())
    cmd.showUsageAndExit();

  string input_fn   = cmd[1];
  string mapping_fn = cmd[2];
  string output_fn  = cmd[3];

  // Read Document-#frames mapping
  vector<string> docids;
  vector<int> frames;

  ifstream fin(mapping_fn.c_str());
  string line;
  while (getline(fin, line)) {
    vector<string> tokens = split(line, ' ');
    docids.push_back(tokens[0]);
    frames.push_back(stoi(tokens[1]));
  }
  fin.close();

  // Read dense matrix from input/pipe
  if (input_fn.empty())
    return 0;

  if (input_fn == "-")
    reformat(cin, docids, frames);
  else {
    ifstream fin(input_fn.c_str());
    reformat(fin, docids, frames);
    fin.close();
  }

  return 0;
}

void reformat(istream& in, const vector<string>& docids, const vector<int>& frames) {

  string line;

  for (size_t i=0; i<frames.size(); ++i) {
    cout << docids[i] << "  [";

    for (size_t j=0; j<(size_t) frames[i]; ++j) {
      getline(in, line);
      cout << "\n  " << line;
    }
    cout << " ]\n";
  }
}
