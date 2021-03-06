#include <string.h>
#include <stdlib.h>

#include <gtk/gtk.h>

#include "init.h"

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
}

void init_text_editor(struct text_editor *texteditor, GtkBuilder *builder, GtkWidget *window)
{
	texteditor->text_view = GTK_WIDGET(gtk_builder_get_object(builder, "editor_view"));
	texteditor->file_label = GTK_WIDGET(gtk_builder_get_object(builder, "file_label"));
	texteditor->buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "buffer"));
	texteditor->status = GTK_WIDGET(gtk_builder_get_object(builder, "text_editor_info_label"));
	texteditor->window = window;
	texteditor->filename = malloc(sizeof(gchar) * 500);
	memset(texteditor->filename, 0, 500);
	texteditor->tip_window = GTK_WIDGET(gtk_builder_get_object(builder, "combo_window"));
	/*texteditor->tip_window = gtk_window_new(GTK_WINDOW_POPUP);
	//gtk_container_set_border_width(GTK_CONTAINER(texteditor->tip_window), 1);

	GtkWidget *label = gtk_label_new("combo_label");
	gtk_label_set_text(GTK_LABEL(label), "test");

	gtk_container_add(GTK_CONTAINER(texteditor->tip_window), label);
	*/
	/* 
	** ==========
	** CREATE TAG
	** ==========
	*/
	gtk_text_buffer_create_tag(texteditor->buffer, "size20",
		"size", 20 * PANGO_SCALE, 
		NULL);

	gtk_text_buffer_create_tag(texteditor->buffer, "blue",
		"foreground", "blue",
		NULL);

	gtk_text_buffer_create_tag(texteditor->buffer, "none",
		"foreground", "black",
		NULL);
}

// Free function
void free_menu_bar(struct menu_bar *menubar)
{
	free(menubar);
}

void free_text_editor(struct text_editor *texteditor)
{
	free(texteditor->filename);
	free(texteditor);
}
