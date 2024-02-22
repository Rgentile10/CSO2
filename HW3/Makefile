CC=gcc
CFLAGS=-fsanitize=address -Og -g

# Define the executable name
TARGET=main

# Define source files
SRCS=fork_run.c main.c

# Define object files from source files
OBJS=$(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean

