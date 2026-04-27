CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

TARGET = history_db
SRCS = src/main.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
