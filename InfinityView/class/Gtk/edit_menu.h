#ifndef EDIT_MENU_H
#define EDIT_MENU_H

#include "init.h"

void cut_to_clipboard(GtkWidget *widget, GtkTextBuffer *buffer);
void copy_to_clipboard(GtkWidget *widget, GtkTextBuffer *buffer);
void paste_to_clipboard(GtkWidget *widget, GtkTextBuffer *buffer);
void select_all_text(GtkWidget *widget, GtkTextBuffer *buffer);

#endif
