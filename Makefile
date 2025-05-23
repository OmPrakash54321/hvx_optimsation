# Compiler and flags
CC = aarch64-none-linux-gnu-gcc
CPP = aarch64-none-linux-gnu-g++
CFLAGS = -Wall -Wextra -static

# Source files
SRCS_C = c_code.c
SRCS_CPP = neon.cpp main.cpp
OBJS = $(SRCS_C:.c=.o) $(SRCS_CPP:.cpp=.o)

# Executable name
TARGET = c_vs_neon

# Default target
all: $(TARGET)

# Link object files into final binary
$(TARGET): $(OBJS)
	$(CPP) $(CFLAGS) -o $@ $^
	rm -f $(OBJS)

# Compile .c files into .o
%.o: %.c
	$(CPP) $(CFLAGS) -c $< -o $@

# Compile .cpp files into .o
%.o: %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets (not actual files)
.PHONY: all clean
