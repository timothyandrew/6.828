#include "kernel/types.h"
#include "user/user.h"

void pipeline(int p[]) {
  int pid = fork();

  if (pid == 0) {
    // Child
    close(p[1]);
    char buf[1];
    int ret = read(p[0], buf, 1);
    if (ret < 0) {
      fprintf(2, "Failed to read\n");
      exit(1);
    } else if (ret == 0) {
      wait((int *)0);
      exit(0);
    }
    int prime = buf[0];
    printf("prime %d\n", prime);

    int subp[2];
    if (pipe(subp) != 0) {
      fprintf(2, "Failed to create pipe\n");
      exit(1);
    }
    pipeline(subp);
    close(subp[0]);

    for (;;) {
      int ret = read(p[0], buf, 1);

      if (ret < 0) {
        fprintf(2, "Failed to read\n");
        exit(1);
      } else if (ret == 0) {
        close(subp[1]);
        wait((int *)0);
        exit(0);
      }
      int n = buf[0];

      if (n % prime != 0) {
        write(subp[1], buf, 1);
      }
    }

  } else if (pid > 0) {
    // Parent
    return;
  } else {
    // Error
    fprintf(2, "Fork failed\n");
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  int p[2];

  if (pipe(p) != 0) {
    fprintf(2, "Failed to create pipe\n");
    exit(1);
  }

  pipeline(p);

  for (int i = 2; i <= 35; i++) {
    char buf[1] = {i};
    write(p[1], buf, 1);
  }
  close(p[1]);

  wait((int *)0);
  exit(0);
}
