#include <stdio.h>
#include <string.h>

#include <gtk/gtk.h>

struct menu_bar {
	GtkWidget *new;
	GtkWidget *open;
	GtkWidget *save;
	GtkWidget *saveas;
	GtkWidget *quit;
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
	menubar->new = GTK_WIDGET(gtk_builder_get_object(builder, "new_menuItem"));	
	menubar->open = GTK_WIDGET(gtk_builder_get_object(builder, "open_menuItem"));	
	menubar->save = GTK_WIDGET(gtk_builder_get_object(builder, "save_menuItem"));	
	menubar->saveas = GTK_WIDGET(gtk_builder_get_object(builder, "saveas_menuItem"));	
	menubar->quit = GTK_WIDGET(gtk_builder_get_object(builder, "quit_menuItem"));	
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

void new_file(GtkWidget *widget, struct text_editor *texteditor)
{
	gtk_widget_show(widget);
	gtk_widget_show(texteditor->text_view);
	gtk_widget_show(texteditor->file_label);
	gtk_label_set_text(GTK_LABEL(texteditor->file_label), "Untitled");
	gtk_text_buffer_set_text(texteditor->buffer, "", -1);
}

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

	// Connect Signal
	g_signal_connect(menubar->quit, "activate", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(menubar->new, "activate", G_CALLBACK(new_file), texteditor);
	g_signal_connect(menubar->save, "activate", G_CALLBACK(save_file), texteditor);
	g_signal_connect(menubar->saveas, "activate", G_CALLBACK(saveas_file), texteditor);
	
	// Hidding object
	gtk_widget_hide(texteditor->text_view);
	gtk_widget_hide(texteditor->file_label);

	// Show the window
	gtk_widget_show(window);
	gtk_main();


	free(menubar);
	return 0;
}
