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

static void regex_print(const Options *opts, char line[], regex_t regex, const char* FILE_NAME, int line_num, char content[]) {
  regmatch_t match;
  int regex_result = regexec(&regex, line, 1, &match, 0);

  bool should_print = (regex_result == 0);

  if (opts->not_match) {
    should_print = !should_print;
  }

  if (should_print) {
    if (opts->show_list_file) {
      printf("%s: %s\n", FILE_NAME, line);
    } else if (opts->number_lines) {
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
    } else if (opts->only_pattern_match) {
      printf("%s\n", content);
    } else {
      printf("%s\n", line);
    }
  }
}

static void pattern_input_file(char content[], const Options *opts, const char* PATTERN) {
  int pfile = open(content, O_RDONLY);
  if (pfile < 0) {
    if (!opts->skip_error) {
      perror(PATTERN);
    }
    return;
  }

  memset(content, 0, 1024);
  ssize_t bytes = read(pfile, content, 1023);
  if (bytes <= 0) {
    if (!opts->skip_error) {
      fprintf(stderr, "Error reading pattern file\n");
    }
    close(pfile);
    return;
  }

  if (bytes > 0 && content[bytes - 1] == '\n') {
    content[bytes - 1] = '\0';
  } else {
    content[bytes] = '\0';
  }

  close(pfile);
}


Options parse_args(int argc, char* argv[]) {
  Options opts = {false, false, false, false, false, false,
                  false, false, false, false, NULL,  NULL};
  int opt;
  while ((opt = getopt(argc, argv, "incevlhsfo")) != -1) {
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
      case 'v':
        opts.not_match = true;
        break;
      case 'l':
        opts.list_file = true;
        break;
      case 'h':
        opts.show_list_file = true;
        break;
      case 's':
        opts.skip_error = true;
        break;
      case 'f':
        opts.file_input = true;
        break;
      case 'o':
        opts.only_pattern_match = true;
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

  if (fd < 0) {
    if (!opts->skip_error) {
      fprintf(stderr, "The content provided");
      return;
    }
  }

  char content[1024] = {0};
  strcpy(content, PATTERN);

  if (opts->file_input) {
    pattern_input_file(content, opts, PATTERN);
  }

  regex_t regex;
  int cflags = REG_EXTENDED;

  if (opts->ignore_case) {
    cflags |= REG_ICASE;
  }

  int compile_result = regcomp(&regex, content, cflags);
  if (compile_result != 0 && !opts->skip_error) {
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
  bool found_match = false;

  while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
    for (ssize_t i = 0; i < bytes_read; i++) {
      unsigned char ch = (unsigned char)buffer[i];

      if (ch == '\n' || line_len == sizeof(line) - 1) {
        line[line_len] = '\0';
        line_num++;
        regex_print(opts, line, regex, FILE_NAME, line_num, content);
        if (opts->list_file) {
          printf("%s", FILE_NAME);
          break;
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

    regex_print(opts, line, regex, FILE_NAME, line_num, content);
    if (opts->list_file) {
      printf("%s", FILE_NAME);
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
    fprintf(stderr, "Error no file provided");
    return 1;
  }

  grep_implement(opts.FILE_NAME, opts.PATTERN, &opts);
  return 0;
}


