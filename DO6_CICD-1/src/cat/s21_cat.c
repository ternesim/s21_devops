#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  char **files;
  int files_len;
};






void input(int argc, char **argv, struct flags **flag_container);
void output(struct flags *flags, char *program_name);
void map_char(int c, struct flags *flags);

int main(int argc, char **argv) {
  struct              flags *flags;
  input(argc, argv, &flags);
  output(flags, argv[0]);
  free(flags);
  exit(0);
}

void input(int argc, char **argv, struct flags **flag_container) {
  struct flags *flags = (struct flags *)calloc(1, sizeof(struct flags));

  int c;
  while (1) {
    static struct option long_options[] = {
        {"number-nonblank", no_argument, 0, 'b'},
        {"number", no_argument, 0, 'n'},
        {"squeeze-blank", no_argument, 0, 's'},
        {0, 0, 0, 0}};

    int option_index = 0;
    c = getopt_long(argc, argv, "bnsETetv?", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 'b':
        flags->b++;
        break;

      case 'v':
        flags->v++;
        break;

      case 's':
        flags->s++;
        break;

      case 'n':
        flags->n++;
        break;

      case 't':
        flags->v++;
        flags->t++;
        break;

      case 'T':
        flags->t++;
        break;

      case 'e':
        flags->v++;
        flags->e++;
        break;

      case 'E':
        flags->e++;
        break;

      case '?':
        printf("usage: cat [-bnsETet] [file ...]\n");
        exit(1);
    }
  }

  flags->files = argv + optind;
  flags->files_len = argc - optind;
  *flag_container = flags;
}

void map_char(int c, struct flags *flags) {
  FILE *out = stdout;
  if (c < 0) c += 256;
  if (c == 10) {
    if (flags->e) fprintf(out, "$");
    fprintf(out, "\n");
  } else if (c == 9) {
    if (flags->t)
      fprintf(out, "^I");
    else
      fprintf(out, "\t");
  } else if (flags->v) {
    if (c < 32)
      fprintf(out, "^%c", c + 64);
    else if (c < 127) {
      putc(c, stdout);
    } else if (c == 127)
      fprintf(out, "^?");
    else if (c < 160)
      fprintf(out, "M-^%c", c - 64);
    else if (c < 255)
      fprintf(out, "M-%c", c - 128);
    else
      fprintf(out, "M-^?");
  } else
    fprintf(out, "%c", c);
}

void output(struct flags *flags, char *program_name) {
  int current_file_index = 0;
  int line_number = 1;
  while (current_file_index < flags->files_len) {
    FILE *fp = fopen(flags->files[current_file_index], "r");
    if (fp == NULL) {
      fprintf(stderr, "%s: %s: No such file or directory", program_name,
              flags->files[current_file_index]);
      free(flags);
      exit(1);
    }

    int prev_char = '\n';
    int next_char;
    int current_char = getc(fp);

    while (current_char != EOF) {
      next_char = getc(fp);

      FILE *out = stdout;
      if (flags->s && (prev_char == '\n') && (next_char == '\n')) continue;
      if (flags->n && (flags->b == 0) && (prev_char == '\n'))
        fprintf(out, "%*d\t", 6, line_number++);
      if (flags->b && (current_char != '\n') && (prev_char == '\n'))
        fprintf(out, "%*d\t", 6, line_number++);

      map_char(current_char, flags);

      prev_char = current_char;
      current_char = next_char;
    }

    current_file_index++;
    fclose(fp);
  }
}
