#ifndef CAT_H
#define CAT_H

#include <stdbool.h>

typedef struct {
  bool show_line_nums;
  bool blank_lines;
  bool show_end_lines;
  bool show_tab;
  bool squaze_lines;
  const char *FILE_NAME;
} Options;

Options parse_args(int argc, char *argv[]);

void print_file(const char *FILE_NAME, const Options *opts);

#endif
