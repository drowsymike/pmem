TARGET = pmem
SRC_DIR = src
BUILD_DIR = build
SRC = $(SRC_DIR)/main.c
OBJ = $(BUILD_DIR)/main.o
#------------------------------------------------------------------------------
CC = gcc
#------------------------------------------------------------------------------
CFLAGS = \
	-O1 \
	-Wall \
	-Wextra \
	# -Werror
	-std=c17 \
	-Wconversion \
	-Wpedantic \
	-Wnull-dereference \
	-fanalyzer\
  -D_FORTIFY_SOURCE=2 \
	-fstack-protector-strong \
	-fstack-clash-protection \
	-fPIE \
  -z,relro \
	-fcf-protection=full \

#CFLAGS:
# -O1 - optimisation level
# - Wall - output all warnings
# -Wextra - strengthening code auditing during compilation
# -Werror - COMMENTARIED. the option what turnes all warmings to errors
# -std=c17 - choosen clang standard
# -Wconversion - warnings about implicit type conversions with possible data loss
# -Wpedantic - strict adherence to the standard
# -Wnull-dereference - null pointer dereference detection
# -fanalyzer - enabling static analysis
# -D_FORTIFY_SOURCE=2 - replacing system memory functions with safer analogs
# -fstack-protector-strong - stack canaries. protection of callback to the function return address
# -fstack-clash-protection - protecting program execution during stack overflow
# -fPIE - independence from the memory address of the binary file execution
# -z,relro - changing the operating mode of dynamic linkage tables of standard libc functions
# -fcf-protection=full - hardware support for a second program stack with return addresses

LDFLAGS = \
	-pie \
	-Wl,-z,relro \
	-Wl,-z,now \
#------------------------------------------------------------------------------
all: $(BUILD_DIR) $(TARGET)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(BUILD_DIR)/$(TARGET) $(LDFLAGS)
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)
clean:
	rm -rf $(BUILD_DIR) $(TARGET)