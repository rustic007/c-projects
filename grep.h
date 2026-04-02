#ifndef GREP_H
#define GREP_H

#include <stdbool.h>

typedef struct {
  bool ignore_case;
  bool number_lines;
  bool match_line_num_only;
  bool filter_pattern;
  const char* PATTERN;
  const char* FILE_NAME;
} Options;

Options parse_args(int argc, char* argv[]);

void print_file(const char* PATTERN, const char* FILE_NAME,
                const Options* opts);

#endif  // !GREP_H
