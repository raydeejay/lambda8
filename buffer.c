#include <stdio.h>
#include "buffer.h"

buffer_t* Buffer_Create(const char *buffer_name) {
    buffer_t *buf = malloc(sizeof(buffer_t));
    strncpy(buf->buffer_name, buffer_name, BUFFERNAMEMAX);
    buf->cur_line = 1;
    buf->num_lines = 1;
    buf->point = 0;
    buf->contents = newGapBuffer(32);
    return buf;
}

status_t Buffer_Clear(buffer_t *buf) {
    buf->cur_line = 1;
    buf->num_lines = 1;
    buf->point = 0;
    destroyGapBuffer(buf->contents);
    buf->contents = newGapBuffer(4000);
    return 1;
}

status_t Buffer_Delete(buffer_t *buf) {
    destroyGapBuffer(buf->contents);
    return 1;
}

status_t Point_Set(buffer_t *buf, location_t loc) {
    if (loc <= buf->contents->count)
        buf->point = loc;
    else if (loc < 0)
        buf->point = 0;
    else
        buf->point = buf->contents->count;

    return 1;
}

status_t Point_Move(buffer_t *buf, int count) {
    Point_Set(buf, buf->point + count);
    return 1;
}

location_t Point_Get(buffer_t *buf) {
    return buf->point;
}

int Point_Get_Line(buffer_t *buf) {
    // terribly inefficient
    int lines = 1;
    location_t pos = buf->point;

    while(Search_Backward(buf, "\n")) {
        ++lines;
    }

    buf->point = pos;
    return lines;
}

location_t Buffer_Start(buffer_t *buf) {
    return 0;
}

location_t Buffer_End(buffer_t *buf) {
    return buf->contents->count;
}

status_t Move_Gap_To_Point(buffer_t *buf) {
    gap_buffer_t *gbuf = buf->contents;

    if (gbuf->gap_end - gbuf->gap_start < 8) {
        expandGapBuffer(gbuf);
    }

    if (gbuf->gap_start < buf->point) {
        unsigned int count = buf->point - gbuf->gap_start;
        memmove(gbuf->contents + gbuf->gap_start,
               gbuf->contents + gbuf->gap_end,
               count);
        gbuf->gap_start += count;
        gbuf->gap_end += count;
    }
    else if (gbuf->gap_start > buf->point) {
        unsigned int count = gbuf->gap_start - buf->point;
        memmove(gbuf->contents + gbuf->gap_end - count,
               gbuf->contents + buf->point,
               count);
        gbuf->gap_start -= count;
        gbuf->gap_end -= count;
    }

    return 1;
}

status_t Mark_Create(buffer_t *buf, const char *name, flag_t fixed) {
    // create a mark
    mark_t *mark = malloc(sizeof(mark));
    mark->name = strdup(name);
    mark->next = NULL;
    mark->location = buf->point;

    // find the first mark with no next, if any
    mark_t *m = buf->marks;
    if (m) {
        while(m->next) {
            m = m->next;
        }
    }

    // attach the mark
    if (m) {
        m->next = mark;
    }
    else {
        buf->marks = mark;
    }

    return 1;
}

void Mark_Delete(buffer_t *buf, const char* name) {
    mark_t *mark = buf->marks;

    if (mark && !strcmp(mark->name, name)) {
        mark_t *tmp = mark->next;
        free(mark->name);
        free(mark);
        buf->marks = tmp;
    }
    else {
        while (mark) {
            if (mark->next && !strcmp(mark->next->name, name)) {
                mark_t *tmp = mark->next->next;
                free(mark->next->name);
                free(mark->next);
                mark->next = tmp;
            }
            else {
                mark = mark->next;
            }
        }
    }
}

void Set_Modified(buffer_t *buf, flag_t is_modified) {
    buf->is_modified = is_modified;
}

flag_t Get_Modified(buffer_t *buf) {
    return buf->is_modified;
}

void Insert_Char(buffer_t *buf, char c) {
    gap_buffer_t *gbuf = buf->contents;

    Move_Gap_To_Point(buf);
    gbuf->contents[gbuf->gap_start] = c;
    ++(gbuf->count);
    ++(gbuf->gap_start);
    ++(buf->num_chars);
    ++(buf->point);

    if (c == '\n') {
        ++(buf->cur_line);
        ++(buf->num_lines);
    }

    buf->is_modified = 1;
}

void Insert_String(buffer_t *buf, char *string) {
    gap_buffer_t *gbuf = buf->contents;
    int count = strlen(string);

    Move_Gap_To_Point(buf);

    while (gbuf->gap_end - gbuf->gap_start < count)
        expandGapBuffer(gbuf);

    gbuf = buf->contents;
    memcpy(&gbuf->contents[gbuf->gap_start], string, count);

    gbuf->count += count;
    gbuf->gap_start += count;
    buf->point += count;

    buf->is_modified = 1;
}

status_t Delete(buffer_t *buf, int count) {
    gap_buffer_t *gbuf = buf->contents;

    if(buf->point < gbuf->count) {
        Move_Gap_To_Point(buf);
        char c = gbuf->contents[gbuf->gap_end];
        gbuf->contents[gbuf->gap_end] = '\0';
        --(gbuf->count);
        ++(gbuf->gap_end);
        --(buf->num_chars);

        if (c == '\n') {
            --(buf->num_lines);
        }

        buf->is_modified = 1;
        return 1;
    }
    else return 0;
}

status_t Search_Forward(buffer_t *buf, const char *string) {
    char *str = renderGapBuffer(buf->contents);
    char *pos = strstr(str + buf->point, string);
    int success = 0;

    if (pos) {
        buf->point = pos - str;
    }

    free(str);
    return pos ? 1 : 0;
}

char *reverse(const char * __restrict const s)
{
    if (s == NULL)
        return NULL;
    size_t i, len = strlen(s);
    char *r = malloc(len + 1);

    for (i = 0; i < len; ++i)
        r[i] = s[len - i - 1];
    r[len] = 0;
    return r;
}

char *rstrstr(char *__restrict s1, char *__restrict s2)
{
    size_t s1len = strlen(s1);
    size_t s2len = strlen(s2);
    char *s;

    if (s2len > s1len)
        return NULL;
    for (s = s1 + s1len - s2len; s >= s1; --s)
        if (strncmp(s, s2, s2len) == 0)
            return s;
    return NULL;
}

status_t Search_Backward(buffer_t *buf, const char *string) {
    char *str = renderGapBuffer(buf->contents);
    char *sstr = strndup(str, buf->point); /* cut the string inorder to search backwards */

    char *pos = rstrstr(sstr, string);
    int success = 0;

    if (pos) {
        buf->point = pos - sstr;
    }

    free(sstr);
    free(str);
    return pos ? 1 : 0;
}

status_t Find_First_In_Forward(buffer_t *buf, const char *string) {
    char *str = Buffer_Render(buf);
    char *pos = strpbrk(str + buf->point + 1, string);

    if (pos) {
        buf->point = pos - str;
    }

    free(str);
    return pos ? 1 : 0;
}

char *strcpbrk(const char *s, const char *reject)
{
    while (*s != '\0')
    {
        const char *a = reject;
        int not_found = 0;
        int count = strlen(a);

        while (*a != '\0') {
            if (*a != *s)
                ++not_found;
            ++a;
        }
        
        if (not_found == count) {
            return (char *) s;
        }
        
        ++s;
    }
    
    return (char *) s;
}

char *strrpbrk(const char *s, const char *accept)
{
    const char *last = s + strlen(s) - 1;

    while (last >= s)
    {
        const char *a = accept;
        while (*a != '\0') {
            if (*a == *last) {
                return (char *) last;
            }
            ++a;
        }
        --last;
    }
    return (char *) s;
}

char *strcrpbrk(const char *s, const char *reject)
{
    const char *last = s + strlen(s) - 1;

    while (last >= s)
    {
        const char *a = reject;
        int count = strlen(a);
        int not_found = 0;

        while (*a != '\0') {
            if (*a != *last)
                ++not_found;
            ++a;
        }
        
        if (not_found == count) {
            return (char *) last;
        }
        
        --last;
    }
    
    return (char *) s;
}



status_t Find_First_In_Backward(buffer_t *buf, const char *string) {
    char *str = Buffer_Render(buf);
    char *sstr = strndup(str, buf->point); /* cut the string inorder to search backwards */

    char *pos = strrpbrk(sstr, string);

    if (pos) {
        buf->point = pos - sstr;
    }

    free(sstr);
    free(str);
    return pos ? 1 : 0;
}

status_t Find_First_Not_In_Forward(buffer_t *buf, const char *string) {
    char *str = Buffer_Render(buf);
    char *pos = strcpbrk(str + buf->point, string);

    if (pos) {
        buf->point = pos - str - 1;
    }

    free(str);
    return pos ? 1 : 0;
}

status_t Find_First_Not_In_Backward(buffer_t *buf, const char *string) {
    char *str = Buffer_Render(buf);
    char *sstr = strndup(str, buf->point); /* cut the string inorder to search backwards */

    char *pos = strcrpbrk(sstr, string);

    if (pos) {
        buf->point = pos - sstr;
    }

    free(sstr);
    free(str);

    return pos ? 1 : 0;
}


int Get_Column(buffer_t *buf) {
    location_t pos = buf->point;

    status_t after_1st_line = Search_Backward(buf, "\n");

    if (after_1st_line) {
        int col = pos - buf->point - 1;
        buf->point = pos;
        return col;
    }
    else {
        return buf->point;
    }
}



char *Buffer_Render(buffer_t *buf) {
    return renderGapBuffer(buf->contents);
}

void Buffer_Render_Screen(buffer_t *buf, terminal_t *term) {
    // figure out the x and y
    char *str = renderGapBuffer(buf->contents);
    int count = strlen(str);

    // find a few newlines before the point
    int pos = buf->point;
    int newlines = 0;

    // move to the beginning of the line
    while (pos > 0 && (str[pos-1] != '\n')) {
        --pos;
    }

    // go backwards at most 10 lines
    while (pos > 0 && newlines < 10) {
        --pos;
        if (str[pos] == '\n') {
            ++newlines;
        }
    }

    // if there are less than 10 newlines, we end up at the beginning
    if (newlines < 10) {
        pos = 0;
        newlines = 0;
    }

    // count a number of lines equal to term->h minus 2,
    // leaving room for the modeline and minibuffer
    int lpos = pos;
    int lnewlines = 0;

    while (lpos < count && lnewlines < term->h - 2) {
        ++lpos;
        if (str[lpos] == '\n') { ++lnewlines; }
    }

    // get the substring which holds the text that will fill the framebuffer
    char *res = strndup(&str[pos], lpos - pos);
    free(str);

    // clear the screen completely
    // this is probably affordable because... it's 2018 and
    // the use case is meant to be SDL text terminals
    terminalClearScreen(term);

    // if the pointer is nowhere(?) we will leave it where it is
    int px = -1, py = -1;

    // write the characters to the terminal's framebuffer
    for (int i = 0, x = 0, y = 0; res[i]; i++) {
        // store the location of the point when we get to it
        if (buf->point == pos+i) {
            px = terminalColumn(term);
            py = terminalRow(term);
        }

        // when hitting a newline, move downwards and go back to column 0
        if (res[i] == '\n') {
            terminalSetColumn(term, 0);
            terminalSetRow(term, terminalRow(term) + 1);
            x = 0;
            ++y;
        }
        else {
            if (x < term->w && y < term->h) { terminalPutChar(term, res[i]); }
            ++x;
        }
    }

    free(res);

    // render the modeline
    // figure out how to specify the attributes
    char *modeline = malloc(term->w + 1);
    snprintf(modeline, term->w + 1,
             "[-%c %-30s                       %4d,%4d   --]",
             Get_Modified(buf) ? '*' : '-',
             buf->buffer_name,
             Point_Get_Line(buf),
             Get_Column(buf));
    memcpy(term->buffer + term->w * (term->h - 2), modeline, term->w);
    free(modeline);

    // render the minibuffer
    char *minibuffer = malloc(term->w + 1);
    snprintf(minibuffer, term->w + 1, ">>>>>");
    memcpy(term->buffer + term->w * (term->h - 1), minibuffer, term->w);
    free(minibuffer);


    // restore the cursor position to where the point is
    terminalSetCursor(term,
                      py == -1 ? terminalRow(term) : py,
                      px == -1 ? terminalColumn(term) : px);

    // render the framebuffer to screen using the callback
    term->render(term);

    // render cursor using the callback
    term->render_cursor(term);
}

world_t *World_Init() {
    world_t *w = malloc(sizeof(world_t));
    w->buffer_chain = Buffer_Create("*scratch*");
    w->current_buffer = w->buffer_chain;
    
    return w;
}

status_t World_Finish(world_t *world) {
}

#ifdef BUFFERTEST
void debug_buf(buffer_t *buf) {
    printf("POINT: %d   GS: %d   GE: %d   SIZE: %d\n",
           buf->point,
           buf->contents->gap_start,
           buf->contents->gap_end,
           buf->contents->size);
}

int main(int argc, char *argv[]) {
    buffer_t *buf = Buffer_Create("test");

    for (int i = 0; i < 50; ++i) {
        Insert_Char(buf, 'a');
        Insert_Char(buf, 'b');
        Insert_Char(buf, 'c');
    }

    Point_Set(buf, 15);

    for (int i = 0; i < 50; ++i) {
        Insert_Char(buf, 'x');
        Insert_Char(buf, 'y');
        Insert_Char(buf, 'z');
    }

    Point_Set(buf, 18);

    for (int i = 0; i < 50; ++i) {
        Insert_Char(buf, '1');
        Insert_Char(buf, '2');
        Insert_Char(buf, '3');
    }

    char *str = renderGapBuffer(buf->contents);
    debug_buf(buf);
    printf("%s\n", str);
    free(str);

    Buffer_Delete(buf);
    return 0;
}
#endif
