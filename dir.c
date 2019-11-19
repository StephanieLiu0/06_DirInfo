#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

void list(char *directory) {

  // list directories
  DIR *ds = opendir(directory);
  struct dirent *file = readdir(ds);

  printf( "Directories:\n");
  for (; file != NULL; file = readdir(ds)) {
    if (file->d_type == DT_DIR)
      printf("\t%s\n", file->d_name);
  }

  // list regular files
  ds = opendir(directory);
  file = readdir(ds);

  printf( "Regular Files:\n");
  for (; file != NULL; file = readdir(ds)) {
    if (file->d_type == DT_REG)
      printf("\t%s\n", file->d_name);
  }

  closedir(ds);
}

long size(char *directory) {

  long size = 0;
  DIR *ds = opendir(directory);
  struct dirent *file = readdir(ds);

  for (; file != NULL; file = readdir(ds)) {
    if (file->d_type == DT_REG) {
      struct stat buff;
      stat(file->d_name, &buff);
      size += buff.st_size;
    }
  }

  closedir(ds);
  printf("Total Directory size: %ld Bytes\n", size);
  return size;
}

int main(int argc, char *argv[]) {
  
  // given no command line arguments
  if (argc < 2) {
    printf("Enter directory to scan: ");
    char directory[100];
    fgets(directory, 100, stdin);
    directory[strlen(directory)-1] = 0;
    argc++;
    argv[1] = directory;
  }

  for (int i = 1; i < argc; ++i) {
    char *directory = argv[i];
    printf("\nStatistics for directory: %s\n", directory);
    if (opendir(directory) == NULL) {
      printf("errno %d: %s\n", errno, strerror(errno));
    }
    else {
      size(directory);
      list(directory);
    }
    printf("\n");
  }
  return 0;
}
