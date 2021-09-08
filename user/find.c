#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(char *path, char *pattern) {
  int fd;
  char buf[512], *p;
  struct stat st;
  struct dirent de;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s, error = %d\n", path, fd);
    exit(1);
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    exit(1);
  }

  if (st.type != T_DIR) {
    close(fd);
    return;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
      continue;
    }

    memcpy(p, de.name, DIRSIZ);
    // WHY?
    p[DIRSIZ] = 0;

    if (strcmp(de.name, pattern) == 0) {
      printf("%s\n", buf);
    }
    find(buf, pattern);
  }

  close(fd);
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(2, "usage: find <path> <pattern>\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
