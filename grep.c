#include "grep.h"

#include <fcntl.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define RED "\x1b[31m"
#define RESET "\x1b[0m"

Options parse_args(int argc, char* argv[]) {
  Options opts = {false, false, false, false, NULL, NULL};
  int opt;
  int cflags = REG_EXTENDED;
  while ((opt = getopt(argc, argv, "ince")) != -1) {
    switch (opt) {
      case 'i':
        opts.ignore_case = true;
        break;
      case 'n':
        opts.number_lines = true;
        break;
      case 'c':
        opts.match_line_num_only = true;
        break;
      case 'e':
        opts.filter_pattern = true;
        break;
      default:
        break;
    }
  }

  if (argc > optind) {
    opts.PATTERN = argv[optind];
  }
  if (optind + 1 < argc) {
    opts.FILE_NAME = argv[optind + 1];
  }
  return opts;
}

void grep_implement(const char* FILE_NAME, const char* PATTERN,
                    const Options* opts) {
  int fd = open(FILE_NAME, O_RDONLY);

  regex_t regex;
  int cflags = REG_EXTENDED;

  if (opts->ignore_case) {
    cflags |= REG_ICASE;
  }

  int compile_result = regcomp(&regex, PATTERN, cflags);
  if (compile_result != 0) {
    char error_msg[100];
    regerror(compile_result, &regex, error_msg, sizeof(error_msg));
    fprintf(stderr, "Error compiling regex: %s\n", error_msg);
    close(fd);
    return;
  };

  char buffer[4096];
  char line[1024];
  size_t line_len = 0;
  int line_num = 0;
  ssize_t bytes_read;

  while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
    for (ssize_t i = 0; i < bytes_read; i++) {
      unsigned char ch = (unsigned char)buffer[i];

      if (ch == '\n' || line_len == sizeof(line) - 1) {
        line[line_len] = '\0';
        line_num++;

        regmatch_t match;
        if (regexec(&regex, line, 1, &match, 0) == 0) {
          if (opts->number_lines) {
            printf("%d: %s\n", line_num, line);
          } else if (opts->match_line_num_only) {
            printf("%d\n", line_num);
          } else if (opts->filter_pattern) {
            printf("%.*s", (int)match.rm_so, line);
            printf(RED);
            printf("%.*s", (int)(match.rm_eo - match.rm_so),
                   line + match.rm_so);
            printf(RESET);
            printf("%s\n", line + match.rm_eo);
          } else {
            printf("%s\n", line);
          }
        }

        line_len = 0;
      } else {
        line[line_len++] = ch;
      }
    }
  }

  if (line_len > 0) {
    line[line_len] = '\0';
    line_num++;

    if (regexec(&regex, line, 0, NULL, 0) == 0) {
      if (opts->number_lines) {
        printf("%d:%s\n", line_num, line);
      } else {
        printf("%s\n", line);
      }
    }
  }

  regfree(&regex);
  close(fd);
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s [-n] [-i] pattern <filename>", argv[0]);
    return 1;
  }

  Options opts = parse_args(argc, argv);

  if (opts.PATTERN == NULL) {
    fprintf(stderr, "grep: missing pattern\n");
    return 1;
  }

  if (opts.FILE_NAME == NULL) {
    fprintf(stderr, "Errror no file provided");
    return 1;
  }

  grep_implement(opts.FILE_NAME, opts.PATTERN, &opts);
  return 0;
}
