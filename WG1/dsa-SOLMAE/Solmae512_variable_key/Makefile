SRCS = $(wildcard *.c )
OBJS = $(patsubst %.c, %.o, $(SRCS))
HEADERS = $(wildcard *.h)

TARGET  = main

CC      = gcc

#CFLAGS  = -fsanitize=address -g -Wall -Wextra -march=native -O0
#LDFLAGS = -fsanitize=address

CFLAGS  = -Wall -Wextra -march=native -O2
LDFLAGS = 
LDLIBS  = -lm

.PHONY: default all clean

default: $(TARGET)
all: default

%.o: %.c $(HEADERS)

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(TARGET)
