#include <cmdparser.h>
#include <fstream>
#include <cstdio>
#include <kaldi-io.h>

int main (int argc, char* argv[]) {

  CmdParser cmd(argc, argv);
  cmd.add("kaldi-ark-in")
     .add("frame-counts");

  cmd.addGroup("Example: ./count-frames data/example.39.ark frames.txt");

  if(!cmd.isOptionLegal())
    cmd.showUsageAndExit();

  KaldiArchive ark(cmd[1]);
  saveFrameCounts(cmd[2], ark);

  return 0;
}
