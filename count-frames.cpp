#include <cmdparser.h>
#include <fstream>
#include <cstdio>
#include <kaldi-io.h>

int main (int argc, char* argv[]) {

  CmdParser cmd(argc, argv);
  cmd.add("kaldi-ark-in");

  cmd.addGroup("Example: ./count-frames data/example.39.ark");

  if(!cmd.isOptionLegal())
    cmd.showUsageAndExit();

  string input_fn   = cmd[1];

  KaldiArchive ark(input_fn);

  size_t N = ark.docid().size();
  size_t dim = ark.dim();
  const vector<size_t>& offset = ark.offset();

  for (size_t i=0; i<N; ++i)
    cout << (offset[i+1] - offset[i]) / dim << endl;

  return 0;
}
