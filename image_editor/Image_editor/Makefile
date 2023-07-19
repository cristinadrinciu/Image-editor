# Compiler setup
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Define targets
TARGETS = image_editor

# Build both image_editor and commands
build: $(TARGETS)

# Build image_editor with both image_editor.c and commands.c
image_editor: image_editor.c commands.c 
	$(CC) $(CFLAGS) image_editor.c commands.c -g -lm -o image_editor

pack:
	zip -FSr Image_editor.zip README.md Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
