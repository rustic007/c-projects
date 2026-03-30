#ifndef CAT_H
#define CAT_H

#include <stdbool.h>

typedef struct {
  bool show_line_nums;
  bool blank_lines;
  bool show_end_lines;
  bool show_tab;
  bool squaze_lines;
  const char *filename;
} Options;

Options parse_args(int argc, char *argv[]);

void print_file(const char *filename, bool show_line_num, bool blank_lines,
                bool show_end_lines, bool show_tab, bool squaze_lines);

#endif
