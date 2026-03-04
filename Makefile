CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lrpm -lrpmio

SRC = src/main.c src/rpm_reader.c src/cli.c
OBJ = $(SRC:.c=.o)

TARGET = rpm-inspect

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean