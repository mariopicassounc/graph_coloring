CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LIBS = -lm
SRCS = APIG23.c APIParte2.c main.c
OBJS = $(SRCS:.c=.o)
DEPS = APIG23.h EstructuraGrafo23.h
TARGET = grafo

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(TARGET) $(OBJS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
