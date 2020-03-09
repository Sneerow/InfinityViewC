#ifndef FILE_MENU_H
#define FILE_MENU_H

#include "init.h"

// File menu functions
void new_file(GtkWidget *widget, struct text_editor *texteditor);
void close_file(GtkWidget *widget, struct text_editor *texteditor);
void save_file(GtkWidget *widget, struct text_editor *texteditor);
void saveas_file(GtkWidget *widget, struct text_editor *texteditor);
void open_file(GtkWidget *widget, struct text_editor *texteditor);

#endif
