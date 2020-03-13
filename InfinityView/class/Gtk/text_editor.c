#include <string.h>
#include <stdlib.h>

#include <gtk/gtk.h>

#include "init.h"

// Function to show window
void show_window(struct text_editor *texteditor, GtkTextIter pos)
{
    GdkWindow *win;
    GdkRectangle buf_loc;
    gint win_x;
    gint win_y;
    gint x;
    gint y;

    gtk_text_view_get_iter_location(GTK_TEXT_VIEW(texteditor->text_view), &pos, &buf_loc);

    gtk_text_view_buffer_to_window_coords(GTK_TEXT_VIEW(texteditor->text_view),
            GTK_TEXT_WINDOW_WIDGET, buf_loc.x, buf_loc.y, &win_x, &win_y);

    win = gtk_text_view_get_window(GTK_TEXT_VIEW((texteditor->text_view)), GTK_TEXT_WINDOW_WIDGET);
    gdk_window_get_origin(win, &x, &y);

    gtk_window_move(GTK_WINDOW(texteditor->tip_window), win_x + x, win_y + y + buf_loc.height);

    gtk_widget_show(texteditor->tip_window);

}

// Function to apply a tag
void buffer_tag_apply(struct text_editor *texteditor, const GtkTextIter *start, 
    const GtkTextIter *end, char *tag_name)
{
    gtk_text_buffer_apply_tag_by_name(texteditor->buffer, tag_name, start, end);
}

// Get the right tag to apply to a word
char *tag_to_apply(gchar *word)
{
    // TODO Appeler fonction de Maxime/Thomas
    // C'est a dire recuperar le type du mots envoye
    // Ex: "int" = 1 pour "keyword";
    int res = 0;
    if (strcmp(word, "int") == 0)
        res = 1;

    if (res == 1)
    {
        return "blue";
    }

    return "none";
}

// Function to get a word and apply a tag on it
void apply_tag_to_word(GtkTextIter start, struct text_editor *texteditor)
{
    if (gtk_text_iter_ends_word(&start))
    {
        GtkTextIter *end_word = gtk_text_iter_copy(&start);
        gboolean check = gtk_text_iter_backward_word_start(&start);

        if (check)
        {

            gchar *word = gtk_text_buffer_get_text(texteditor->buffer, &start, end_word, FALSE);
            
            gtk_text_buffer_remove_all_tags(texteditor->buffer, &start, end_word);

            // TODO
            // call: tag_to_apply instead of if(...)
            char *tagname = tag_to_apply(word);
            buffer_tag_apply(texteditor, &start, end_word, tagname);

            buffer_tag_apply(texteditor, &start, end_word, "size20");
        }
        gtk_text_iter_free(end_word);
    }

}

// Apply tag on every word of a file
void apply_tag_to_file(struct text_editor *texteditor)
{
    GtkTextIter start_file;
    gtk_text_buffer_get_start_iter(texteditor->buffer, &start_file);

    gboolean check = gtk_text_iter_forward_word_end(&start_file);
    while (check)
    {
        apply_tag_to_word(start_file, texteditor);
        check = gtk_text_iter_forward_word_end(&start_file);
    }
}

// Update the info bar aka where you are in the file
void status_bar_update(GtkWidget *widget, struct text_editor *texteditor)
{
	gtk_widget_show(texteditor->status);

	gint row;
	gint col;
	GtkTextIter iter;

	gtk_text_buffer_get_iter_at_mark(texteditor->buffer, &iter, 
			gtk_text_buffer_get_insert(texteditor->buffer));

	row = gtk_text_iter_get_line(&iter);
	col = gtk_text_iter_get_line_offset(&iter);

    apply_tag_to_word(iter, texteditor);

	gchar *info = g_strdup_printf("row: %d, col: %d", row, col);

	gtk_label_set_text(GTK_LABEL(texteditor->status), info);

    show_window(texteditor, iter);

	g_free(info);
}