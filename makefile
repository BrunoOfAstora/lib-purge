SRC = src
INC = inc

CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -I$(INC)

SOURCES = $(wildcard $(SRC)/*.c)

OBJECTS = $(SOURCES:.c=.o)

objects: $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
