#include <stdlib.h>
#include <string.h>

#include "gapbuffer.h"

gap_buffer_t *newGapBuffer(unsigned int size) {
    gap_buffer_t *gbuf = malloc(sizeof(gap_buffer_t));
    gbuf->size = size;
    gbuf->contents = malloc(gbuf->size);
    memset(gbuf->contents, 0, gbuf->size);
    gbuf->gap_start = 0;
    gbuf->gap_end = 16;
    return gbuf;
}

status_t expandGapBuffer(gap_buffer_t *gbuf) {
    unsigned int newSize = gbuf->size * 2;
    gbuf->contents = realloc(gbuf->contents, newSize);
    memset(gbuf->contents + gbuf->size, 0, gbuf->size);
    memmove(gbuf->contents + gbuf->size,
           gbuf->contents + gbuf->gap_end,
           gbuf->count - gbuf->gap_start);
    gbuf->gap_end = gbuf->size;
    gbuf->size = newSize;
    return 1;
}

status_t destroyGapBuffer(gap_buffer_t *gbuf) {
    free(gbuf->contents);
    gbuf->size = 0;
    gbuf->count = 0;
    return 1;
}

char *renderGapBuffer(gap_buffer_t *gbuf) {
    char *string = malloc(gbuf->count+1);
    memset(string, 0, gbuf->count+1);
    memmove(string, gbuf->contents, gbuf->gap_start);
    memmove(string + gbuf->gap_start,
            gbuf->contents + gbuf->gap_end,
            gbuf->count - gbuf->gap_start);

    return string;
}

int Convert_User_To_Gap(gap_buffer_t *gbuf, location_t point) {
    return (point < gbuf->gap_start)
        ? point
        : gbuf->gap_end - gbuf->gap_start + point;
}
