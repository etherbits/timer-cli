#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void printUsage() { printf("usage: timer [HH:MM:SS]\nexample: timer 15:00\n"); }

struct Duration {
  char h, m, s;
};

struct FormattedDuration {
  char h[3], m[3], s[3];
};

struct Duration durFromString(char *str) {
  int stringLen = strlen(str);
  int hours = (int)str[0] - 0x30 + (int)str[1] - 0x30;
  int currUnit = 2;
  int pos = 0;
  char units[3] = {0, 0, 0};

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

    units[currUnit] += ((int)str[i] - 0x30) * pow(10, pos);
    pos++;
  }

  struct Duration dur = {units[0], units[1], units[2]};

  return dur;
}

struct Duration durFromSeconds(time_t s) {
  char h = s / 3600;
  s %= 3600;
  char m = s / 60;
  s %= 60;
  struct Duration dur = {h, m, (char)s};
  return dur;
}

void copyFormattedDurComponent(char *dest, char comp) {
  const char formattedComp[2] = {(char)(comp / 10 + 0x30),
                                 (char)(comp % 10 + 0x30)};
  strcpy(dest, formattedComp);
}

struct FormattedDuration getFormattedDuration(struct Duration dur) {
  char h[2], m[2], s[2];

  copyFormattedDurComponent(h, dur.h);
  copyFormattedDurComponent(m, dur.m);
  copyFormattedDurComponent(s, dur.s);

  struct FormattedDuration formattedDuration = {
      {h[0], h[1], '\0'}, {m[0], m[1], '\0'}, {s[0], s[1], '\0'}};
  return formattedDuration;
}

int getDurSeconds(struct Duration dur) {
  int seconds = dur.h * 3600 + dur.m * 60 + dur.s;

  return seconds;
}
