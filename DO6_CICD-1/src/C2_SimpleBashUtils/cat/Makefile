CC = gcc
CFLAGS = -Wall -Wextra -Werror

SOURCES = s21_cat.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = s21_cat


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
