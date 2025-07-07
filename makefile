SRC = src
INC = inc
OBJ = obj

TARGET = libpurge.so

CC = gcc
CFLAGS = -g -O0 -Wall -Wextra -std=c11 -fPIC -I$(INC)

SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
DEP = $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(OBJ) 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ) $(TARGET)

install:
	sudo cp $(TARGET) /lib/
	sudo cp $(TARGET) /usr/local/lib/
	sudo cp $(INC)/*.h /usr/local/include/
	sudo ldconfig

-include $(DEP)
