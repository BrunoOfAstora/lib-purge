SRC := src
INC := inc
OBJ := obj

TARGET := libpurge.so

CC := gcc
CFLAGS := -g -O0 -Wall -Wextra -std=c11 -fPIC $(addprefix -I,$(shell find $(INC) -type d))

SOURCES := $(shell find $(SRC) -name '*.c')
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
DEP := $(OBJECTS:.o=.d)

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

clean:
	@rm -rf $(OBJ) $(TARGET)

install:
	sudo cp $(TARGET) /usr/local/lib/
	sudo cp $(shell find $(INC) -name '*.h') /usr/local/include/
	sudo ldconfig

-include $(DEP)

