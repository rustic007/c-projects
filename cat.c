#include "cat.h"
#include <stdio.h>
#include <string.h>
// you should have struct contains: bools and filename
// 3 functions main, print_file and parse_args
// main contains: the enough argc and argv provided also the exta functions
// parse_args returns object (it sets to default first), checks flags contains:
// -b -n with strcmp print_file: chccks not empty, writes lines based onl flags
// and counts line numbers.

Options parse_args(int argc, char *argv[]) {
  Options opts = {false, false, false, false, false, NULL};

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      for (size_t j = 1; j < strlen(argv[i]); j++) {
        switch (argv[i][j]) {
        case 'n':
          opts.show_end_lines = true;
          break;
        case 'b':
          opts.blank_lines = true;
          break;
        case 'e':
          opts.show_end_lines = true;
          break;
        case 't':
          opts.show_tab = true;
          break;
        case 's':
          opts.squaze_lines = true;
          break;          
        default:
          break;
        }
      }
    } else {
      opts.filename = argv[i];
    }
  }
  return opts;
}

void print_file(const char *filename, bool show_line_num, bool blank_lines,
                bool show_end_lines, bool show_tab, bool squaze_lines) {
  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    printf("There is no such file: %s", filename);
    return;
  }

  char line[1024];
  int line_num = 1;
  bool prev_is_blank = false;

  while (fgets(line, sizeof(line), file) != NULL) {

    bool is_blank = (line[0] == '\n');

    if (squaze_lines && is_blank && prev_is_blank) {
      continue;
    }

    prev_is_blank = is_blank;

    if (blank_lines && line[0] != '\n') {
      printf("%d:", line_num);
      line_num++;
    }

    if (show_line_num) {
      printf("%d:", line_num);
      line_num++;
    }

    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }

    for (int i = 0; line[i] != '\0'; i++) {
      if (show_tab && line[i] == '\t') {
        printf("^I");
      } else {
        printf("%c", line[i]);
      }
    }

    if (show_end_lines) {
      printf("$");
    }

    printf("\n");
  }
  fclose(file);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s [-n] [-b] <filename>", argv[0]);
    return 1;
  }

  Options opts = parse_args(argc, argv);

  if (opts.filename == NULL) {
    printf("Error occured to find the file: %s", argv[0]);
    return 1;
  }

  print_file(opts.filename, opts.show_line_nums, opts.blank_lines,
             opts.show_end_lines, opts.show_tab, opts.squaze_lines);
  return 0;
}
