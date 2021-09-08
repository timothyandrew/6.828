#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(2, "usage: sleep <ticks>\n");
    exit(1);
  }

  int ticks = atoi(argv[1]);
  int ret = sleep(ticks);

  if (ret == 0) {
    exit(0);
  }
  else {
    fprintf(2, "Sleep failed; %d ticks to go\n", ret);
    exit(1);
  }
}
