#include <cmdparser.h>
#include <fstream>
#include <cstdio>
#include <kaldi-io.h>

int main (int argc, char* argv[]) {

  CmdParser cmd(argc, argv);
  cmd.add("kaldi-ark-in")
     .add("frame-counts", false);

  cmd.addGroup("Example: ./count-frames data/example.39.ark frames.txt");

  if(!cmd.isOptionLegal())
    cmd.showUsageAndExit();

  string input_fn   = cmd[1];
  string output_fn  = cmd[2];

  KaldiArchive ark(input_fn);
  saveFrameCounts(output_fn, ark);

  return 0;
}
