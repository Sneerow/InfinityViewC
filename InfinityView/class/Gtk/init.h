#ifndef INIT_H
#define INIT_H


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
};

struct text_editor {
	GtkWidget *text_view;
	GtkWidget *file_label;
	GtkTextBuffer *buffer;
	GtkWidget *window;
	GtkWidget *status;
};

// Init struct using glade file
void init_menu_bar(struct menu_bar *menubar, GtkBuilder *builder);
void init_text_editor(struct text_editor *texteditor, GtkBuilder *builder, GtkWidget *window);

// Free struct
void free_menu_bar(struct menu_bar *menubar);
void free_text_editor(struct text_editor *texteditor);


#endif
