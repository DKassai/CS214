#include <stdio.h>

#include "../src/frequency.h"


int main() {
    FrequencyMap *map = FM_create(10);
    FM_add(map, "this");
    FM_add(map, " ");
    FM_add(map, "is");
    FM_add(map, " ");
    FM_add(map, "this");
    FM_add(map, " ");
    FM_add(map, "and");
    FM_add(map, " ");
    FM_add(map, "that");
    FM_add(map, " ");
    FM_add(map, "is");
    FM_add(map, " ");
    FM_add(map, "that");

    printf("%s: %d\n", "this", FM_frequency(map, "this"));
    printf("%s: %d\n", "is", FM_frequency(map, "is"));
    printf("%s: %d\n", "that", FM_frequency(map, "that"));
    printf("%s: %d\n", "and", FM_frequency(map, "and"));
    printf("%s: %d\n", " ", FM_frequency(map, " "));
    printf("%s: %d\n", "foobar", FM_frequency(map, "foobar"));

    FM_destroy(map);
    return 0;
}

