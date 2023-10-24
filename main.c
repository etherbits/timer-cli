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

  while (time(NULL) < endSeconds) {
    struct Duration currDur = durFromSeconds(endSeconds - time(NULL));
    struct FormattedDuration fd = getFormattedDuration(currDur);

    printf("\r%s:\r\n%s:\n%s", fd.h, fd.m, fd.s);
    fflush(stdout);

    sleep(1);
  }

  return 0;
}
