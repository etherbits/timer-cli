#include "utils.c"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2 || strlen(argv[1]) < 1 || strlen(argv[1]) > 8) {
    printUsage();
    return -1;
  }

  char *formattedDuration = argv[1];

  struct Duration dur = durFromString(formattedDuration);
  time_t endSeconds = time(NULL) + getDurSeconds(dur);

  FILE *fp;

  if (argc > 3 && !strcmp(argv[2], "--waybar")) {
    fp = fopen(argv[3], "w");
  }

  while (time(NULL) < endSeconds) {
    struct Duration currDur = durFromSeconds(endSeconds - time(NULL));
    struct FormattedDuration fd = getFormattedDuration(currDur);
    if (argc < 3 || strcmp(argv[2], "--waybar")) {
      printf("\r%s:%s:%s", fd.h, fd.m, fd.s);
      fflush(stdout);
    } else {
      fseek(fp, 0, SEEK_SET);
      fprintf(fp, "{\"text\": \"%s\\n%s\\n%s\"}", fd.h, fd.m, fd.s);
      fflush(fp);
    }

    sleep(1);
  }

  return 0;
}
