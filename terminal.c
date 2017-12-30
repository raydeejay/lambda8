#include <stdlib.h>
#include <string.h>

#include "terminal.h"

terminal_t *newTerminal(int w, int h,
                        void (*render)(struct terminal* term),
                        void (*render_cursor)(struct terminal* term)) {
    terminal_t *term = malloc(sizeof(terminal_t));

    term->w = w;
    term->h = h;
    term->render = render;
    term->render_cursor = render_cursor;
    
    terminalClearScreen(term);
    
    return term;
}

int terminalRows(terminal_t *term) {
    return term->h;
}

int terminalColumns(terminal_t *term) {
    return term->w;
}

void terminalSetCursor(terminal_t *term, int row, int column) {
    term->row = row;
    term->column = column;
}

void terminalSetRow(terminal_t *term, int row) {
    term->row = row;
}

void terminalSetColumn(terminal_t *term, int column) {
    term->column = column;
}

int terminalRow(terminal_t *term) {
    return term->row;
}

int terminalColumn(terminal_t *term) {
    return term->column;
}

void terminalMoveRight(terminal_t *term) {
    term->column++;

    if (term->column == term->w) {
        term->column = 0;
        term->row++;
    }
}

void terminalMoveLeft(terminal_t *term) {
    term->column++;

    if (term->column == -1) {
        term->column = term->w - 1;
        term->row--;
    }
}


void terminalPutChar(terminal_t *term, char c) {
    term->buffer[term->row * term->w + term->column] = c;
    terminalMoveRight(term);
}

void terminalPutString(terminal_t *term, char *str) {
    int x = term->column,
        y = term->row;

    terminalClearScreen(term);

    for (int i = 0; ; ++i) {
        // exit when the string is finished
        if (!str[i]) break;
        
        if (str[i] == '\n' /* || x == term->w */) {
            x = 0;
            ++y;
        }
        else if (x < term->w && y < term->h) {
            term->buffer[y*term->w + x] = str[i];
            ++x;
        }
    }
}


void terminalCLEOL(terminal_t *term);
void terminalCLEOS(terminal_t *term);
void terminalClearLine(terminal_t *term);
void terminalClearScreen(terminal_t *term) {
    memset(term->buffer, ' ', MAX_TERMINAL_BUFFER);
    term->buffer[MAX_TERMINAL_BUFFER] = '\0';
    term->row = 0;
    term->column = 0;
}

void terminalDisplayCursor(terminal_t *T) {
    // plug something here, the terminal is abstract
}

/* void terminalPutString2(terminal_t *term, char *string) { */
/*     // do no wrapping, but clip chars instead */
/*     int x = 0, y = 0; */

/*     terminalClearScreen(term); */
    
/*     for (int i = 0; ; ++i) { */
/*         // exit when the string is finished */
/*         if (!string[i]) break; */
        
/*         if (string[i] == '\n' /\* || x == term->w *\/) { */
/*             x = 0; */
/*             ++y; */
/*         } */
/*         else if (x < term->w && y < term->h) { */
/*             term->buffer[y*term->w + x] = string[i]; */
/*             ++x; */
/*         } */
/*     } */
/* } */

char *terminalRender(terminal_t *term) {
    // hmm......

    // could used a plugged function to render?
    return term->buffer;
}
