#include "s7.h"

#include "lambda8.h"
#include "buffer.h"

#include "editor.h"

s7_pointer l8_insert_char(s7_scheme *sc, s7_pointer args) {
    const char *str = s7_string(s7_car(args));
    Insert_Char(gBuffer, str[0]);
    return s7_nil(sc);
}

s7_pointer l8_point_move(s7_scheme *sc, s7_pointer args) {
    int amount = s7_integer(s7_car(args));
    Point_Move(gBuffer, amount);
    return s7_nil(sc);
}

s7_pointer l8_get_column(s7_scheme *sc, s7_pointer args) {
    return s7_make_integer(sc, Get_Column(gBuffer));
}

s7_pointer l8_find_first_in_forward(s7_scheme *sc, s7_pointer args) {
    const char *str = s7_string(s7_car(args));
    Find_First_In_Forward(gBuffer, str);
    return s7_nil(sc);
}

s7_pointer l8_find_first_in_backward(s7_scheme *sc, s7_pointer args) {
    const char *str = s7_string(s7_car(args));
    Find_First_In_Backward(gBuffer, str);
    return s7_nil(sc);
}

s7_pointer l8_find_first_not_in_forward(s7_scheme *sc, s7_pointer args) {
    const char *str = s7_string(s7_car(args));
    Find_First_Not_In_Forward(gBuffer, str);
    return s7_nil(sc);
}

s7_pointer l8_find_first_not_in_backward(s7_scheme *sc, s7_pointer args) {
    const char *str = s7_string(s7_car(args));
    Find_First_Not_In_Backward(gBuffer, str);
    return s7_nil(sc);
}

typedef s7_pointer (*l8_func)(s7_scheme *sc, s7_pointer args);

struct { const char *name; l8_func fn; int nargs; int optargs; bool restargs; const char *doc; } l8_e_prims[] = {
    { "insert-char", l8_insert_char, 1, 0, false, "(insert-char ch) Inserts character ch at point" },
    { "point-move",  l8_point_move,  1, 0, false, "(point-move n) Moves the point by n" },
    { "get-column",  l8_get_column,  0, 0, false, "(get-column) Returns the current column" },
    { "find-first-in-forward",      l8_find_first_in_forward , 1, 0, false, "(finds)" },
    { "find-first-not-in-forward",  l8_find_first_not_in_forward , 1, 0, false, "(finds)" },
    { "find-first-in-backward",     l8_find_first_in_backward , 1, 0, false, "(finds)" },
    { "find-first-not-in-backward", l8_find_first_not_in_backward , 1, 0, false, "(finds)" },

    { NULL, NULL, 0, 0, false, NULL }
};

void initEditorLisp(s7_scheme *sc) {
    // init the editor's interpreter
    for (int i = 0; l8_e_prims[i].name; ++i) {
        s7_define_function(sc, l8_e_prims[i].name, l8_e_prims[i].fn, l8_e_prims[i].nargs, l8_e_prims[i].optargs, l8_e_prims[i].restargs, l8_e_prims[i].doc);
    }

    // bind the editor functions to the global environment
    s7_load(sc, "editor.lsp");
}
