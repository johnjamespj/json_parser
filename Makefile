TARGET = ./build/out
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

SRC = $(wildcard ./libs/*/*.c)
OBJECTS = $(patsubst ./libs/%, ./build/%, $(patsubst %.c, %.o, $(SRC)))
HEADERS = $(wildcard ./libs/headers/*.h)

./build/%.o: ./libs/%.c $(HEADERS)
	mkdir -p ./build  $(patsubst ./libs/%, ./build/%, $(wildcard ./libs/*))
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@
	$(TARGET)

clean:
	-rm -r ./build/* 
	-rm -f $(TARGET)