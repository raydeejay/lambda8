#define MAX_TERMINAL_BUFFER 4096

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _TERMINAL_H
#define _TERMINAL_H

    typedef struct terminal {
        int w;
        int h;
        int column;
        int row;
        char buffer[MAX_TERMINAL_BUFFER + 1];
        void (*render)(struct terminal* term);
        void (*render_cursor)(struct terminal* term);
    } terminal_t;

    extern terminal_t *newTerminal(int w, int h,
                                   void (*render)(struct terminal* term),
                                   void (*render_cursor)(struct terminal* term));

    extern int terminalRows(terminal_t *term);
    extern int terminalColumns(terminal_t *term);

    extern void terminalSetCursor(terminal_t *term, int row, int column);
    extern void terminalSetRow(terminal_t *term, int row);
    extern void terminalSetColumn(terminal_t *term, int column);
    extern int terminalRow(terminal_t *term);
    extern int terminalColumn(terminal_t *term);
    extern void terminalPutChar(terminal_t *term, char c);
    extern void terminalPutString2(terminal_t *term, char *str);

    extern void terminalCLEOL(terminal_t *term);
    extern void terminalCLEOS(terminal_t *term);
    extern void terminalClearLine(terminal_t *term);
    extern void terminalClearScreen(terminal_t *term);

    extern void terminalPutString(terminal_t *term, char *string);
    extern char *terminalRender(terminal_t *term);

    extern void terminalClear(terminal_t *term);

    extern void terminalKeyPressed(terminal_t *T, char key);
    extern void terminalNewline(terminal_t *T);
    extern void terminalBackspace(terminal_t *T);

#endif

#ifdef __cplusplus
}
#endif
