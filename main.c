#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void printUsage() { printf("usage: timer [HH:MM:SS]\nexample: timer 15:00\n"); }

struct Duration {
  int units[3];
};

struct Duration durFromString(char *str) {
  int stringLen = strlen(str);
  struct Duration dur = {};
  int hours = (int)str[0] - 0x30 + (int)str[1] - 0x30;
  int currUnit = 2;
  int pos = 0;

  for (int i = stringLen - 1; i >= 0; i--) {
    if (pos > 2) {
      printUsage();
      exit(-1);
    }

    if ((int)str[i] < 0x30 && (int)str[i] > 0x39) {
      printUsage();
      exit(-1);
    }

    if (str[i] == ':') {
      currUnit--;
      pos = 0;
      continue;
    }

    dur.units[currUnit] += ((int)str[i] - 0x30) * pow(10, pos);
    pos++;
    printf("%d\n", dur.units[currUnit]);
  }

  return dur;
}

int getDurSeconds(struct Duration dur) {
  int seconds = dur.units[0] * 3600 + dur.units[1] * 60 + dur.units[2];

  return seconds;
}

int main(int argc, char *argv[]) {

  if (argc < 2 || strlen(argv[1]) < 1 || strlen(argv[1]) > 8) {
    printUsage();
    return -1;
  }

  char *formattedDuration = argv[1];

  printf("%s\n", formattedDuration);
  struct Duration dur = durFromString(formattedDuration);
  printf("%d:%d:%d\n", dur.units[0], dur.units[1], dur.units[2]);


  time_t endSeconds = time(NULL) + getDurSeconds(dur);
  printf("current: %ld\nend: %ld\n", time(NULL), endSeconds);

  while(time(NULL) < endSeconds){
    sleep(1);
    printf("\rleft: %ld\n", endSeconds-time(NULL));
    fflush(stdout);
  }

  return 0;
}
