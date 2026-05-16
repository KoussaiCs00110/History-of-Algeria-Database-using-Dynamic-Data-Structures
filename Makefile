CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Console version (no raylib)
console: src/main.c src/linked_list.c src/stack.c src/bst.c src/recursion.c
	$(CC) $(CFLAGS) -o history_db $^ -lm

# GUI version (uses local libraylib.a)
gui: src/gui.c src/linked_list.c src/stack.c src/bst.c src/recursion.c
	$(CC) $(CFLAGS) -o history_gui $^ libraylib.a -lm -lpthread -ldl -lrt -lX11

clean:
	rm -f history_db history_gui
