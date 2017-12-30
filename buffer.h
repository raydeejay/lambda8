#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFERNAMEMAX 256
#define FILENAMEMAX 256
#define MARKNAMEMAX 256

#include "terminal.h"
#include "gapbuffer.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef BUFFER_H_
#define BUFFER_H_

    typedef int flag_t;
    typedef int status_t;

/********************
 * LOCATION
 *******************/
    typedef int location_t;

/********************
 * MARK
 *******************/
    typedef struct mark {
        struct mark *next;
    
        location_t location;
        char *name;
        flag_t fixed;
    } mark_t;

/********************
 * MODE
 *******************/
    typedef struct editor_mode {
        struct editor_mode *next_mode;
    
        char *mode_name;
        status_t (*add_proc)();       /* also WHAT */
    } editor_mode_t;

/********************
 * BUFFER
 *******************/
    typedef struct buffer {
        struct buffer *next_chain_entry;
        char buffer_name[BUFFERNAMEMAX+1];

        location_t point;
        int cur_line;
        int num_chars;
        int num_lines;

        mark_t *marks;
        gap_buffer_t *contents;
    
        char file_name[FILENAMEMAX+1];
        time_t file_time;
        flag_t is_modified;

        editor_mode_t *modes;
    } buffer_t;

    extern status_t Mark_Create(buffer_t *buf, const char *name, flag_t fixed);
    extern void Mark_Delete(buffer_t *buf, const char* name);
    status_t Mark_To_Point(const char *name);
    status_t Point_To_Mark(const char *name);
    location_t Mark_Get(const char *name);
    status_t Mark_Set(const char *name, location_t loc);
    flag_t Is_Point_At_Mark(const char *name);
    flag_t Is_Point_Before_Mark(const char *name);
    flag_t Is_Point_After_Mark(const char *name);
    status_t Swap_Point_and_Mark(const char *name);

/********************
 * WORLD
 *******************/
    typedef struct world {
        buffer_t *buffer_chain;
        buffer_t *current_buffer;
    } world_t;

/********************
 * FUNCTIONS
 *******************/
    extern int Convert_User_To_Gap(gap_buffer_t *gbuf, location_t point);
    extern status_t Move_Gap_To_Point(buffer_t *buf);

    extern buffer_t* Buffer_Create(const char *buffer_name);
    extern status_t Buffer_Clear(buffer_t *buf);
    extern status_t Buffer_Delete(buffer_t *buf);

    status_t Buffer_Set_Current(const char *buffer_name);
    const char *Buffer_Set_Next(void);
    status_t Buffer_Set_Name(const char *buffer_name);
    const char *Buffer_Get_Name(void);

    extern status_t Point_Set(buffer_t *buf, location_t loc);
    extern status_t Point_Move(buffer_t *buf, int count);
    extern location_t Point_Get(buffer_t *buf);
    extern int Point_Get_Line(buffer_t *buf);
    extern location_t Buffer_Start(buffer_t *buf);
    extern location_t Buffer_End(buffer_t *buf);

    int Compare_Locations(location_t loc1, location_t loc2);
    int Location_To_Count(location_t loc);
    location_t Count_To_Location(int count);

    char Get_Char(void);
    void Get_String(char *string, int count);
    int Get_Num_Chars(void);
    int Get_Num_Lines(void);

    void Get_File_Name(char *filename, int size);
    status_t Set_File_Name(const char *filename);
    status_t Buffer_Write(char *string);
    status_t Buffer_Read(void);
    status_t Buffer_Insert(const char *filename);
    flag_t Is_File_Changed(void);
    extern void Set_Modified(buffer_t *buf, flag_t is_modified);
    extern flag_t Get_Modified(buffer_t *buf);

    status_t Mode_Append(const char *mode_name, status_t (*add_proc)(), flag_t is_front);
    status_t Mode_Delete(const char *mode_name);
    status_t Mode_Invoke(void);

    extern void Insert_Char(buffer_t *buf, char c);
    extern void Insert_String(buffer_t *buf, char *string);
    void Replace_Char(buffer_t *buf, char c);
    void Replace_String(char *string);
    extern status_t Delete(buffer_t *buf, int count);
    status_t Delete_Region(const char *mark_name);
    status_t Copy_Region(const char *buffer_name, char *mark_name);

    extern status_t Search_Forward(buffer_t *buf, char *string);
    extern status_t Search_Backward(buffer_t *buf, char *string);
    flag_t Is_A_Match(char *string);
    extern status_t Find_First_In_Forward(buffer_t *buf, char *string);
    extern status_t Find_First_In_Backward(buffer_t *buf, char *string);
    extern status_t Find_First_Not_In_Forward(buffer_t *buf, char *string);
    extern status_t Find_First_Not_In_Backward(buffer_t *buf, char *string);

    extern int Get_Column(buffer_t *buf);
    void Set_Column(int column, flag_t round);

    extern world_t *World_Init(void);
    extern status_t World_Finish(world_t *world);
    /* status_t World_Save(const char *file_name); */
    /* status_t World_Load(const char *file_name); */

    extern char *Buffer_Render(buffer_t *buf);
    extern void Buffer_Render_Screen(buffer_t *buf, terminal_t *term);
#endif

#ifdef __cplusplus
}
#endif
