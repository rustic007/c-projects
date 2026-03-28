#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// you should have struct contains: bools and filename
// 3 functions main, print_file and parse_args
// main contains: the enough argc and argv provided also the exta functions
// parse_args returns object (it sets to default first), checks flags contains:
// -b -n with strcmp print_file: chccks not empty, writes lines based onl flags
// and counts line numbers.

typedef struct {
  bool show_line_nums;
  bool blank_lines;
  const char *filename;
} Options;

Options parse_args(int argc, char *argv[]) {
  Options opts = {false, false, NULL};

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-n") == 0) {
        opts.show_line_nums = true;
      } else if (strcmp(argv[i], "-b") == 0) {
        opts.blank_lines = true;
      }
    } else {
      opts.filename = argv[i];
    }
  }
  return opts;
}

void print_file(const char *filename, bool show_line_num, bool blank_lines) {
  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    printf("There is no such file: %s", filename);
    return;
  }

  char line[1024];
  int line_num = 1;

  while (fgets(line, sizeof(line), file) != NULL) {
    if (blank_lines && line[0] == '\n') {
      line_num++;
      continue;
    }

    if (show_line_num) {
      printf("%d:", line_num);
    }

    printf("%s", line);
    line_num++;
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

  print_file(opts.filename, opts.show_line_nums, opts.blank_lines);
  return 0;
}
