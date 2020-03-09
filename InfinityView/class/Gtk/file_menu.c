#include <string.h>

#include <gtk/gtk.h>

#include "file_menu.h"
#include "init.h"

// Function to create a new file
void new_file(GtkWidget *widget, struct text_editor *texteditor)
{
	gtk_widget_show(widget);

	gtk_widget_show(texteditor->text_view);
	gtk_widget_show(texteditor->file_label);
	gtk_label_set_text(GTK_LABEL(texteditor->file_label), "Untitled");
	gtk_text_buffer_set_text(texteditor->buffer, "", -1);
}

// Function to close a file
void close_file(GtkWidget *widget, struct text_editor *texteditor)
{
	gtk_widget_show(widget);

	gtk_widget_hide(texteditor->text_view);
	gtk_widget_hide(texteditor->file_label);
	gtk_label_set_text(GTK_LABEL(texteditor->file_label), "");
	gtk_text_buffer_set_text(texteditor->buffer, "", -1);
}

// Function to save a file
void saveas_file(GtkWidget *widget, struct text_editor *texteditor)
{

	gtk_widget_show(widget);

	// Check if file is open
	const gchar *check_name = gtk_label_get_text(GTK_LABEL(texteditor->file_label));
	if(strcmp(check_name, "") == 0)
	{
		return;
	}
	
	// Check new file
	gint response;
	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(texteditor->window), 
			GTK_FILE_CHOOSER_ACTION_SAVE,
			"_Save", GTK_RESPONSE_APPLY,
			"_Cancel", GTK_RESPONSE_CANCEL,
			NULL);

	response = gtk_dialog_run(GTK_DIALOG(dialog));

	if (response == GTK_RESPONSE_APPLY)
	{
		gchar *contents;
		GtkTextIter start;
		GtkTextIter end;
		gchar *filename;
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		gtk_text_buffer_get_bounds(texteditor->buffer, &start, &end);
		contents = gtk_text_buffer_get_text(texteditor->buffer, &start, &end, FALSE);
		g_file_set_contents(filename, contents, -1, NULL);
		gtk_label_set_text(GTK_LABEL(texteditor->file_label), filename);

		g_free(filename);
		g_free(contents);
	}

	gtk_widget_destroy(dialog);
}

// Function to quick save a file
void save_file(GtkWidget *widget, struct text_editor *texteditor)
{
	gtk_widget_show(widget);

	// Check new file
	if (strcmp(gtk_label_get_text(GTK_LABEL(texteditor->file_label)), "Untitled") == 0)
	{
		saveas_file(widget, texteditor);
	}
	else
	{
		GtkTextIter start;
		GtkTextIter end;
		gchar *contents;
		const gchar *filename = gtk_label_get_text(GTK_LABEL(texteditor->file_label));
		if(strcmp(filename, "") == 0)
		{
			return;
		}
		gtk_text_buffer_get_bounds(texteditor->buffer, &start, &end);
		contents = gtk_text_buffer_get_text(texteditor->buffer, &start, &end, FALSE);
		g_file_set_contents(filename, contents, -1, NULL);

		g_free(contents);
	}
}


// Function to open a file
void open_file(GtkWidget *widget, struct text_editor *texteditor)
{
	gtk_widget_show(widget);

	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(texteditor->window), 
			GTK_FILE_CHOOSER_ACTION_OPEN,
			"_Open", GTK_RESPONSE_ACCEPT,
			"_Cancel", GTK_RESPONSE_CANCEL,
			NULL);
	gint response;
	response = gtk_dialog_run(GTK_DIALOG(dialog));
	
	if (response == GTK_RESPONSE_ACCEPT)
	{
		gtk_widget_show(texteditor->text_view);
		gtk_widget_show(texteditor->file_label);

		gchar *contents;
		gchar *filename;
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		g_file_get_contents(filename, &contents, NULL, NULL);
		gtk_text_buffer_set_text(texteditor->buffer, contents, -1);
		gtk_label_set_text(GTK_LABEL(texteditor->file_label), filename);


		g_free(filename);
		g_free(contents);
	}

	gtk_widget_destroy(dialog);
}
