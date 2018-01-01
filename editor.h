#ifndef L8_EDITOR_H_
#define L8_EDITOR_H_

#include "s7.h"

extern terminal_t *gTerminal;
extern buffer_t *gBuffer;
extern char *gScript;
extern char *gScriptFilename;

extern s7_pointer l8_insert_char(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_point_move(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_get_column(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_find_first_in_forward(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_find_first_in_backward(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_find_first_not_in_forward(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_find_first_not_in_backward(s7_scheme *sc, s7_pointer args);

extern void initEditorLisp(s7_scheme *sc);

#endif // L8_EDITOR_H_
