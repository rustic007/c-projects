#include "cat.h"

#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
// you should have struct contains: bools and filename
// 3 functions main, print_file and parse_args
// main contains: the enough argc and argv provided also the exta functions
// parse_args returns object (it sets to default first), checks flags contains:
// -b -n with strcmp print_file: chccks not empty, writes lines based onl flags
// and counts line numbers.

Options parse_args(int argc, char* argv[]) {
  Options opts = {false, false, false, false, false, NULL};
  int opt;
  while ((opt = getopt(argc, argv, "nbeEtTs")) != -1) {
    switch (opt) {
      case 'n':
        opts.show_line_nums = true;
        break;
      case 'b':
        opts.blank_lines = true;
        break;
      case 'E':
        opts.show_end_lines = true;
        break;
      case 'e':
        opts.show_end_lines = true;
        opts.show_tab = true;
        break;
      case 't':
        opts.show_tab = true;
        break;
      case 'T':
        opts.show_tab = true;
        break;
      case 's':
        opts.squaze_lines = true;
        break;
      default:
        break;
    }
  }

  if (optind < argc) {
    opts.FILE_NAME = argv[optind];
  }

  return opts;
}

void print_file(const char* FILE_NAME, const Options* opts) {
  int fd = open(FILE_NAME, O_RDONLY);

  if (fd < 0) {
    perror(FILE_NAME);
    return;
  }

  char buffer[4096];
  char line[1024];
  size_t line_len = 0;
  int line_num = 1;
  bool prev_is_blank = false;
  ssize_t bytes_read;

  while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
    for (ssize_t i = 0; i < bytes_read; i++) {
      unsigned char ch = (unsigned char)buffer[i];

      if (ch == '\n') {
        bool is_blank = (line_len == 0);

        if (opts->squaze_lines && is_blank && prev_is_blank) {
          prev_is_blank = true;
          continue;
        }

        prev_is_blank = is_blank;

        if (opts->blank_lines && !is_blank) {
          printf("%6d\t", line_num);
          line_num++;
        }

        if (opts->show_line_nums) {
          printf("%6d\t", line_num);
          line_num++;
        }

        for (size_t j = 0; j < line_len; j++) {
          unsigned char c = (unsigned char)line[j];
          if (opts->show_tab && c == '\t') {
            printf("^I");
          } else {
            printf("%c", c);
          }
        }

        if (opts->show_end_lines) {
          printf("$");
        }

        printf("\n");
        line_len = 0;
      } else if (ch == '\r' && opts->show_tab) {
        printf("^M");
      } else {
        if (line_len < sizeof(line) - 1) {
          line[line_len++] = ch;
        }
      }
    }
  }

  if (bytes_read < 0) {
    perror("read");
  }
  close(fd);
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s [-n] [-b] <filename>", argv[0]);
    return 1;
  }

  Options opts = parse_args(argc, argv);

  if (opts.FILE_NAME == NULL) {
    fprintf(stderr, "Error: no filename provided");
    return 1;
  }

  print_file(opts.FILE_NAME, &opts);
  return 0;
}
