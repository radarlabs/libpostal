#include <stdio.h>
#include <stdlib.h>
#include <libpostal/libpostal.h>

bool isValidUTF8(const char *str) {
  int c, i, ix, n, j;
  for (i = 0, ix = strlen(str); i < ix; i++) {
    c = (unsigned char)str[i];
    if (0x00 <= c && c <= 0x7f)
      n = 0;
    else if ((c & 0xE0) == 0xC0)
      n = 1;
    else if (c == 0xed && i < (ix - 1) && ((unsigned char)str[i + 1] & 0xa0) == 0xa0)
      return false;
    else if ((c & 0xF0) == 0xE0)
      n = 2;
    else if ((c & 0xF8) == 0xF0)
      n = 3;
    else
      return false;
    for (j = 0; j < n && i < ix; j++) {
      if ((++i == ix) || (((unsigned char)str[i] & 0xC0) != 0x80)) return false;
    }
  }
  return true;
}

int main(int argc, char **argv) {
    // Setup (only called once at the beginning of your program)
    if (!libpostal_setup() || !libpostal_setup_language_classifier()) {
        exit(EXIT_FAILURE);
    }

    size_t num_expansions;
    libpostal_normalize_options_t options = libpostal_get_default_options();

    printf("test \n");
    printf("Marktstra\xdf""e 5, 6850 Dornbirn \n");

    printf("isValidUTF8Test: \n");
    bool testBool = isValidUTF8("Marktstra\xdf""e 5, 6850 Dornbirn \n");
    printf(testBool ? "true\n" : "false\n");
    char **emptyExpansions = NULL;

    char **expansions = testBool ? libpostal_expand_address("Marktstra\xdf""e 5, 6850 Dornbirn \n", options, &num_expansions) : emptyExpansions;

    for (size_t i = 0; i < num_expansions; i++) {
        printf("%s\n", expansions[i]);
    }

    // Free expansions
    libpostal_expansion_array_destroy(expansions, num_expansions);

    // Teardown (only called once at the end of your program)
    libpostal_teardown();
    libpostal_teardown_language_classifier();
}