#include <stdio.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "class/Gtk/init.h"
#include "class/Gtk/file_menu.h"
#include "class/Gtk/edit_menu.h"
#include "class/Gtk/text_editor.h"

void add_int(GtkWidget *widget, struct text_editor *texteditor)
{
	gtk_widget_show(widget);

	GtkTextIter iter;
	GtkTextMark *cursor;

	gtk_text_buffer_get_iter_at_mark(texteditor->buffer, &iter, 
			gtk_text_buffer_get_insert(texteditor->buffer));

	cursor = gtk_text_buffer_get_mark(texteditor->buffer, "insert");
	
	gtk_text_buffer_get_iter_at_mark(texteditor->buffer, &iter, cursor);

	gtk_text_buffer_insert(texteditor->buffer, &iter, "int", 3);
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

	GtkWidget *button_test = GTK_WIDGET(gtk_builder_get_object(builder, "int_button"));

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

	g_signal_connect(texteditor->buffer, "changed", G_CALLBACK(status_bar_update), texteditor);

	g_signal_connect(button_test, "clicked", G_CALLBACK(add_int), texteditor);
	
	// Hidding object
	gtk_widget_hide(texteditor->text_view);
	gtk_widget_hide(texteditor->file_label);
	gtk_widget_hide(texteditor->status);

	// Show the window
	gtk_widget_show(window);
	gtk_main();

	free_text_editor(texteditor);
	free_menu_bar(menubar);
	return 0;
}
