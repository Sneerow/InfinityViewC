#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include "init.h"

void buffer_tag_apply(struct text_editor *texteditor, const GtkTextIter *start, 
    const GtkTextIter *end, char *tag_name);

void apply_tag_to_word(GtkTextIter start, struct text_editor *texteditor);

void apply_tag_to_file(struct text_editor *texteditor);

void status_bar_update(GtkWidget *widget, struct text_editor *texteditor);

#endif