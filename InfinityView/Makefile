# Define the default target
CC = gcc

CPPFLAGS = `pkg-config --libs --cflags gtk+-3.0`
CFLAGS = -std=c99 -Wall -Wextra -O3 -g 
LDFLAGS =
LDLIBS = `pkg-config --libs --cflags gtk+-3.0`

OBJ = main.o class/Gtk/init.o class/Gtk/file_menu.o class/Gtk/edit_menu.o class/Gtk/text_editor.o

all: main

# Define dependencies and compile information

main: $(OBJ)

clean:
	$(RM) $(OBJ)
	$(RM) main
