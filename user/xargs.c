#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void execute(char *cmd, char **argv) {
  int pid = fork();

  if (pid == 0) {
    // Child
    int ret = exec(cmd, argv);
    if (ret < 0) {
      fprintf(2, "Exec failed\n");
      exit(1);
    }
  } else if (pid > 0) {
    // Parent
    wait((int *) 0);
  } else {
    // Error
    fprintf(2, "Fork failed\n");
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    fprintf(2, "usage: xargs <command>\n");
    exit(1);
  }

  char buf[128];
  char *p = buf;

  for (;;) {
    int ret = read(0, p, 1);
    if(ret < 0) {
      fprintf(2, "Failed to read\n");
      exit(1);
    }

    if (*p == 10 || ret == 0) {
      if (p == buf) {
        break;
      }

      char* args[8];
      int count = 0;

      args[0] = argv[1];
      count++;

      if (argc > 2) {
        for (int i = 2; i < argc; i++) {
          args[i - 1] = argv[i];
          count++;
        }
      }

      args[count++] = buf;
      args[count] = 0;

      *p++ = 0;
      execute(argv[1], args);

      p = buf;
    } else {
      p += 1;
    }

    if (ret == 0) {
      break;
    }
  }

  exit(0);
}
