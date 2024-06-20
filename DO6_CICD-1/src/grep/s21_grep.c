#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMATCH 1
#define BUFFER_SIZE 4096

struct flags {
  char **patterns;
  int patterns_length;
  char **reg_file_paths;
  int reg_file_paths_len;
  char **files;
  int files_len;
  char *program_name;
  int err_code;
  char *err_message;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
};

void parse_args(int argc, char **argv, struct flags **flag_container);
void free_flags(struct flags *flags);
void free_reegexlist(regex_t *reegex_list, int reegex_list_len);
void parse_args(int argc, char **argv, struct flags **flag_container);
void compile_reg(regex_t **reegx_list_ptr, int *len, struct flags *flags);
void output(regex_t *reegex_list, int reegex_list_len, struct flags *flags);

int main(int argc, char **argv) {
  struct flags *flags = NULL;
  parse_args(argc, argv, &flags);

  int reegex_list_len;
  regex_t *reegex_list = NULL;
  compile_reg(&reegex_list, &reegex_list_len, flags);
  if ((reegex_list_len == 0) && (flags->err_code == 0)) {
    flags->err_code = 6;
    flags->err_message = "Usage: grep [OPTION]... PATTERNS [FILE]...\n";
    free_reegexlist(reegex_list, reegex_list_len);
  }

  output(reegex_list, reegex_list_len, flags);

  free_flags(flags);
  free_reegexlist(reegex_list, reegex_list_len);

  exit(0);
}

void free_flags(struct flags *flags) {
  if (flags && flags->patterns) free(flags->patterns);
  if (flags && flags->reg_file_paths) free(flags->reg_file_paths);
  if (flags) free(flags);
}

void free_reegexlist(regex_t *reegex_list, int reegex_list_len) {
  if (reegex_list) {
    for (int i = 0; i < reegex_list_len; ++i) regfree(reegex_list + i);
    free(reegex_list);
  }
}

void parse_args(int argc, char **argv, struct flags **flag_container) {
  struct flags *flags = (struct flags *)calloc(1, sizeof(struct flags));
  char **patterns = (char **)calloc(argc, sizeof(char *));
  flags->patterns = patterns;
  char **reg_file_paths = (char **)calloc(argc, sizeof(char *));
  flags->reg_file_paths = reg_file_paths;
  flags->err_code = 0;
  flags->err_message = "";

  int c;
  int patterns_length = 0;
  int reg_file_paths_len = 0;
  while (1) {
    c = getopt(argc, argv, "e:ivclnhsof:?");

    if (c == -1) break;

    switch (c) {
      case 'e':
        patterns[patterns_length++] = optarg;
        break;

      case 'f':
        reg_file_paths[reg_file_paths_len++] = optarg;
        break;

      case 'i':
        flags->i++;
        break;

      case 'v':
        flags->v++;
        break;

      case 'c':
        flags->c++;
        break;

      case 'l':
        flags->l++;
        break;

      case 'n':
        flags->n++;
        break;

      case 'h':
        flags->h++;
        break;

      case 's':
        flags->s++;
        break;

      case 'o':
        flags->o++;
        break;

      case '?':
        flags->err_code = 1;
        flags->err_message = "Usage: grep [OPTION]... PATTERNS [FILE]...\n";
        break;
    }
  }

  flags->program_name = argv[0];
  flags->files = argv + optind;
  flags->files_len = argc - optind;
  flags->patterns_length = patterns_length;
  flags->reg_file_paths_len = reg_file_paths_len;
  *flag_container = flags;

  if ((flags->patterns_length == 0) && (flags->reg_file_paths_len == 0) &&
      (argc > 1)) {
    flags->patterns[0] = argv[optind];
    flags->patterns_length++;
    flags->files += 1;
    flags->files_len -= 1;
  }
}

void compile_reg(regex_t **reegx_list_ptr, int *len, struct flags *flags) {
  if (flags->err_code) return;
  int reg_flags = 0;
  if (flags->i) reg_flags = REG_ICASE;
  int reegex_list_len = 0;
  regex_t *reegex_list =
      (regex_t *)calloc(flags->patterns_length, sizeof(regex_t));

  while ((reegex_list_len < flags->patterns_length) && (flags->err_code == 0)) {
    regex_t compiled_reg;
    if (regcomp(&compiled_reg, flags->patterns[reegex_list_len], reg_flags)) {
      free_reegexlist(reegex_list, reegex_list_len);
      flags->err_code = 3;
      flags->err_message = "Wrong pattern\n";
      break;
    }
    reegex_list[reegex_list_len++] = compiled_reg;
  }

  for (int j = 0; ((j < flags->reg_file_paths_len) && (flags->err_code == 0));
       j++) {
    FILE *fp = fopen(flags->reg_file_paths[j], "r");
    if (fp == NULL) {
      char str[80];
      sprintf(str, "%s: %s: No such file or directory\n", flags->program_name,
              flags->reg_file_paths[j]);
      flags->err_code = 6;
      flags->err_message = str;
      free_reegexlist(reegex_list, reegex_list_len);
      break;
    }

    size_t bufsize = BUFFER_SIZE;
    char *buffer = (char *)malloc(bufsize * sizeof(char));

    int lines_number = 0;
    if (flags->err_code == 0) {
      while (getline(&buffer, &bufsize, fp) != EOF) lines_number++;
      fseek(fp, 0, SEEK_SET);
    }

    reegex_list = (regex_t *)realloc(
        reegex_list, (reegex_list_len + lines_number) * sizeof(regex_t));
    while ((getline(&buffer, &bufsize, fp) != EOF) && (flags->err_code == 0)) {
      buffer[strcspn(buffer, "\n")] = 0;
      regex_t compiled_reg;
      if (regcomp(&compiled_reg, buffer, reg_flags)) {
        flags->err_code = 3;
        flags->err_message = "Wrong pattern\n";
        free_reegexlist(reegex_list, reegex_list_len);
        break;
      }
      reegex_list[reegex_list_len++] = compiled_reg;
    }

    free(buffer);
    fclose(fp);
  }

  *reegx_list_ptr = reegex_list;
  *len = reegex_list_len;
}

void output(regex_t *reegex_list, int reegex_list_len, struct flags *flags) {
  if (flags->err_code != 0) {
    fprintf(stderr, "%s", flags->err_message);
    int err = flags->err_code;
    free_flags(flags);
    exit(err);
  }

  int current_file_index = 0;
  int is_many_files = 0;
  if (flags->files_len > 1) is_many_files = 1;
  if (flags->h) is_many_files = 0;
  while (current_file_index < flags->files_len) {
    FILE *fp = fopen(flags->files[current_file_index], "r");
    if (fp == NULL) {
      if (flags->s != 1)
        fprintf(stderr, "%s: %s: No such file or directory\n",
                flags->program_name, flags->files[current_file_index]);
      current_file_index++;
      continue;
    }

    char *buffer = NULL;
    size_t bufsize = BUFFER_SIZE;
    buffer = (char *)malloc(bufsize * sizeof(char));

    int count_matches = 0;
    int line_number = 1;
    while (getline(&buffer, &bufsize, fp) != EOF) {
      buffer[strcspn(buffer, "\n")] = 0;

      int is_matched = 0;
      for (int i = 0; i < reegex_list_len; i++) {
        regex_t compiled_reg = reegex_list[i];
        if (regexec(&compiled_reg, buffer, 0, NULL, 0) == 0) {
          is_matched = 1;
          break;
        }
      }

      if (flags->v) is_matched -= 1;
      if (is_matched) {
        if ((flags->c) && (flags->l == 0))
          count_matches++;
        else if (flags->l) {
          printf("%s\n", flags->files[current_file_index]);
          break;
        } else if (flags->o) {
          char substr[BUFFER_SIZE];
          int rm_so = BUFFER_SIZE;
          int rm_eo = 0;
          char *str = buffer;
          while (1) {
            int res = 0;
            for (int i = 0; i < reegex_list_len; i++) {
              regmatch_t m[NMATCH];
              res += (regexec(reegex_list + i, str, NMATCH, m, 0) == 0);
              if (regexec(reegex_list + i, str, NMATCH, m, 0) != 0) {
                continue;
              }
              if (m->rm_so < rm_so) {
                rm_so = m[0].rm_so;
                rm_eo = m[0].rm_eo;
              }
            }
            if (res == 0) break;
            int len = rm_eo - rm_so;
            memcpy(substr, str + rm_so, len);
            substr[len] = '\0';
            if (is_many_files) printf("%s:", flags->files[current_file_index]);
            if (flags->n) printf("%d:", line_number);
            printf("%s\n", substr);

            str = (str + rm_eo);
            rm_so = BUFFER_SIZE;
            rm_eo = 0;
          }
        } else {
          if (is_many_files) printf("%s:", flags->files[current_file_index]);
          if (flags->n) printf("%d:", line_number);
          printf("%s\n", buffer);
        }
      }

      line_number++;
    }

    if ((flags->c) && (flags->l == 0)) {
      if (is_many_files) printf("%s:", flags->files[current_file_index]);
      printf("%d\n", count_matches);
    }

    fclose(fp);
    free(buffer);
    current_file_index++;
  }
}