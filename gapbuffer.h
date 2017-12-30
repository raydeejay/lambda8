#ifdef __cplusplus
extern "C" {
#endif

#ifndef GAPBUFFER_H_
#define GAPBUFFER_H_

    typedef int location_t;
    typedef int status_t;

    /********************
     * GAP BUFFER
     *******************/
    typedef struct gap_buffer {
        char *contents;
        unsigned int gap_start, gap_end;
        unsigned int size, count;
    } gap_buffer_t;

    extern gap_buffer_t *newGapBuffer(unsigned int size);
    extern status_t expandGapBuffer(gap_buffer_t *gbuf);
    extern status_t destroyGapBuffer(gap_buffer_t *gbuf);
    extern char *renderGapBuffer(gap_buffer_t *gbuf);

#endif

#ifdef __cplusplus
}
#endif
