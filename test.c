#include <regex.h>
#include <stdio.h>
#include <string.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

int main() {
  regex_t regex;
  regmatch_t match;
  const char* pattern = "pattern";
  const char* text = "text with pattern here";

  regcomp(&regex, pattern, REG_EXTENDED);

  if (regexec(&regex, text, 1, &match, 0) == 0) {
    // Print before match
    printf("%.*s", (int)match.rm_so, text);

    // Print match in red
    printf(RED);
    printf("%.*s", (int)(match.rm_eo - match.rm_so), text + match.rm_so);
    printf(RESET);

    // Print after match
    printf("%s\n", text + match.rm_eo);
  }

  regfree(&regex);
  return 0;
}
