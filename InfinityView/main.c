#include <stdio.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>

struct menu_bar {
	GtkWidget *new;
	GtkWidget *open;
	GtkWidget *save;
	GtkWidget *saveas;
	GtkWidget *close;
	GtkWidget *quit;
	
	GtkWidget *cut;
	GtkWidget *copy;
	GtkWidget *paste;
	GtkWidget *select_all;

	GtkWidget *font;
};

struct text_editor {
	GtkWidget *text_view;
	GtkWidget *file_label;
	GtkTextBuffer *buffer;
	GtkWidget *window;
};


// Init functions
void init_menu_bar(struct menu_bar *menubar, GtkBuilder *builder) 
{
	// File Item
	menubar->new = GTK_WIDGET(gtk_builder_get_object(builder, "new_menuItem"));	
	menubar->open = GTK_WIDGET(gtk_builder_get_object(builder, "open_menuItem"));	
	menubar->save = GTK_WIDGET(gtk_builder_get_object(builder, "save_menuItem"));	
	menubar->saveas = GTK_WIDGET(gtk_builder_get_object(builder, "saveas_menuItem"));	
	menubar->close = GTK_WIDGET(gtk_builder_get_object(builder, "close_menuItem"));	
	menubar->quit = GTK_WIDGET(gtk_builder_get_object(builder, "quit_menuItem"));	

	// Edit Item
	menubar->cut = GTK_WIDGET(gtk_builder_get_object(builder, "cut_menuItem"));	
	menubar->copy = GTK_WIDGET(gtk_builder_get_object(builder, "copy_menuItem"));	
	menubar->paste = GTK_WIDGET(gtk_builder_get_object(builder, "paste_menuItem"));	
	menubar->select_all = GTK_WIDGET(gtk_builder_get_object(builder, "select_all_menuItem"));

	// Options Item
	menubar->font = GTK_WIDGET(gtk_builder_get_object(builder, "font_menuItem"));
}

void init_text_editor(struct text_editor *texteditor, GtkBuilder *builder, GtkWidget *window)
{
	texteditor->text_view = GTK_WIDGET(gtk_builder_get_object(builder, "editor_view"));
	texteditor->file_label = GTK_WIDGET(gtk_builder_get_object(builder, "file_label"));
	texteditor->buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "buffer"));
	texteditor->window = window;
}

// ============
// GTK FUNCTION
// ============

// ====
// FILE
// ====

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

// Function to quick save a file
void save_file(GtkWidget *widget, struct text_editor *texteditor)
{
	gtk_widget_show(widget);

	// Check new file
	if (strcmp(gtk_label_get_text(GTK_LABEL(texteditor->file_label)), "Untitled") == 0)
	{
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
			gchar *filename;
			gchar *contents;
			GtkTextIter start;
			GtkTextIter end;
			filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			gtk_text_buffer_get_bounds(texteditor->buffer, &start, &end);
			contents = gtk_text_buffer_get_text(texteditor->buffer, &start, &end, FALSE);
			g_file_set_contents(filename, contents, -1, NULL);
			gtk_label_set_text(GTK_LABEL(texteditor->file_label), filename);

		}

		gtk_widget_destroy(dialog);
		return;
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
	}
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

	}

	gtk_widget_destroy(dialog);
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
	}

	gtk_widget_destroy(dialog);
}

// ====
// EDIT
// ====

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

// =======
// Options 
// =======

void select_font(GtkWidget *widget, struct text_editor *texteditor)
{
	gtk_widget_show(widget);

	GtkWidget *dialog = gtk_font_chooser_dialog_new("Select Font", GTK_WINDOW(texteditor->window));
	gint result;

	result = gtk_dialog_run(GTK_DIALOG(dialog));

	if (result == GTK_RESPONSE_OK || result == GTK_RESPONSE_APPLY)
	{
		PangoFontDescription *font_desc;
		font_desc = gtk_font_chooser_get_font_desc(GTK_FONT_CHOOSER(dialog));

		gtk_widget_modify_font(GTK_WIDGET(texteditor->text_view), font_desc);
	}

	gtk_widget_destroy(dialog);
}


int main(int argc, char *argv[])
{
	// ========
	// GTK INIT
	// ========

	// Init window
	GtkWidget *window;
	gtk_init(&argc, &argv);

	// Init builder using file
	GtkBuilder *builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "InfinityGlade.glade", NULL);

	// Init all widget needed
	window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
	struct menu_bar *menubar = malloc(sizeof(struct menu_bar) * 1);
	init_menu_bar(menubar, builder);
	struct text_editor *texteditor = malloc(sizeof(struct text_editor));
	init_text_editor(texteditor, builder, window);

	// Init shortcut (accel group)
	GtkAccelGroup *accel_group = NULL;
	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

	// Add shortcut
	
	gtk_widget_add_accelerator(menubar->new, "activate", accel_group, GDK_KEY_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(menubar->open, "activate", accel_group, GDK_KEY_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(menubar->save, "activate", accel_group, GDK_KEY_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(menubar->close, "activate", accel_group, GDK_KEY_w, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(menubar->quit, "activate", accel_group, GDK_KEY_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	
	gtk_widget_add_accelerator(menubar->copy, "activate", accel_group, GDK_KEY_c, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(menubar->paste, "activate", accel_group, GDK_KEY_v, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(menubar->cut, "activate", accel_group, GDK_KEY_x, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(menubar->select_all, "activate", accel_group, GDK_KEY_a, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	// Connect Signal
	g_signal_connect(menubar->quit, "activate", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(menubar->new, "activate", G_CALLBACK(new_file), texteditor);
	g_signal_connect(menubar->save, "activate", G_CALLBACK(save_file), texteditor);
	g_signal_connect(menubar->saveas, "activate", G_CALLBACK(saveas_file), texteditor);
	g_signal_connect(menubar->open, "activate", G_CALLBACK(open_file), texteditor);
	g_signal_connect(menubar->close, "activate", G_CALLBACK(close_file), texteditor);
	
	g_signal_connect(menubar->copy, "activate", G_CALLBACK(copy_to_clipboard), texteditor->buffer);
	g_signal_connect(menubar->paste, "activate", G_CALLBACK(paste_to_clipboard), texteditor->buffer);
	g_signal_connect(menubar->cut, "activate", G_CALLBACK(cut_to_clipboard), texteditor->buffer);
	g_signal_connect(menubar->select_all, "activate", G_CALLBACK(select_all_text), texteditor->buffer);

	g_signal_connect(menubar->font, "activate", G_CALLBACK(select_font), texteditor);

	// Hidding object
	gtk_widget_hide(texteditor->text_view);
	gtk_widget_hide(texteditor->file_label);

	// Show the window
	gtk_widget_show(window);
	gtk_main();


	free(menubar);
	return 0;
}
