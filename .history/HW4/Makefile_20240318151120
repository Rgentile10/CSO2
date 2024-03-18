# Compiler and Linker Configurations
CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -g
AR := ar
ARFLAGS := rcs

# Source Files and Object Files
SRCS := mlpt.c main.c
OBJS := $(SRCS:.c=.o)

# Output
LIB := libmlpt.a
EXEC := mlpt_test

# Default target
all: $(LIB) $(EXEC)

# Rule to create the static library
$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# Rule to create the executable for testing
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target for removing compiled files
clean:
	rm -f $(OBJS) $(LIB) $(EXEC)

.PHONY: all clean
