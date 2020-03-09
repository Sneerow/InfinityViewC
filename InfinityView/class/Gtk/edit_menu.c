#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "init.h"

void cut_to_clipboard(GtkWidget *widget, GtkTextBuffer *buffer)
{
	gtk_widget_show(widget);

	GtkClipboard *clipboard;
	clipboard = gtk_clipboard_get(GDK_NONE);
	gtk_text_buffer_cut_clipboard(buffer, clipboard, TRUE);
}

void copy_to_clipboard(GtkWidget *widget, GtkTextBuffer *buffer)
{
	gtk_widget_show(widget);

	GtkClipboard *clipboard;
	clipboard = gtk_clipboard_get(GDK_NONE);
	gtk_text_buffer_copy_clipboard(buffer, clipboard);
}

void paste_to_clipboard(GtkWidget *widget, GtkTextBuffer *buffer)
{
	gtk_widget_show(widget);

	GtkClipboard *clipboard;
	clipboard = gtk_clipboard_get(GDK_NONE);
	gtk_text_buffer_paste_clipboard(buffer, clipboard, NULL, TRUE);
}

void select_all_text(GtkWidget *widget, GtkTextBuffer *buffer)
{
	gtk_widget_show(widget);

	GtkTextIter start;
	GtkTextIter end;
	gtk_text_buffer_get_bounds(buffer, &start, &end);
	gtk_text_buffer_select_range(buffer, &start, &end);
}
