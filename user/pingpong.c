#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int p[2];

  if (pipe(p) != 0) {
    fprintf(2, "Failed to create pipe\n");
    exit(1);
  }

  int pid = fork();

  if (pid == 0) {
    // Child
    char byte[1];

    int ret = read(p[0], byte, 1);

    if (ret != 1) {
      fprintf(2, "Failed to read\n");
      exit(1);
    }

    fprintf(1, "%d: received ping\n", getpid());
    write(p[1], byte, 1);
    exit(0);
  } else if (pid > 0) {
    // Parent
    char byte[1] = {210};

    write(p[1], byte, 1);

    int ret = read(p[0], byte, 1);

    if (ret != 1) {
      fprintf(2, "Failed to read\n");
      exit(1);
    }

    fprintf(1, "%d: received pong\n");
    write(p[1], byte, 1);
    exit(0);
  } else {
    // Error
    fprintf(2, "Fork failed\n");
    exit(1);
  }

  exit(0);
}
