#include <stdio.h>
#include <stdlib.h>
#include <libpostal/libpostal.h>

int main(int argc, char **argv) {
    // Setup (only called once at the beginning of your program)
    if (!libpostal_setup() || !libpostal_setup_language_classifier()) {
        exit(EXIT_FAILURE);
    }

    size_t num_expansions;
    libpostal_normalize_options_t options = libpostal_get_default_options();
/* U+00df  LATIN SMALL LETTER SHARP S  encoded in iso-8859-1 instead of UTF-8 
 *
 * prints 
 * WARN  invalid UTF-8 at transliterate (transliterate.c:791)
 * but libpostal_expand_address() never returns - endless loop
 */
    char **expansions = libpostal_expand_address("Marktstra\xdf""e 5", options, &num_expansions);

    for (size_t i = 0; i < num_expansions; i++) {
        printf("%s\n", expansions[i]);
    }

    // Free expansions
    libpostal_expansion_array_destroy(expansions, num_expansions);

    // Teardown (only called once at the end of your program)
    libpostal_teardown();
    libpostal_teardown_language_classifier();
}
