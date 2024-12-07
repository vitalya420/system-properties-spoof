NDK_PATH := /home/vitaliy/Android/Sdk/ndk/27.0.12077973
SYSROOT := $(NDK_PATH)/toolchains/llvm/prebuilt/linux-x86_64/sysroot
CC := $(NDK_PATH)/toolchains/llvm/prebuilt/linux-x86_64/bin/i686-linux-android21-clang

CFLAGS := -fPIC -DANDROID -I$(SYSROOT)/usr/include
LDFLAGS := -ldl

SRC_FILES := hook.c hashmap.c props.c main.c
OBJ_FILES := $(SRC_FILES:.c=.o)
OUTPUT_FILE := build/libhook.so

# Create build directory if it doesn't exist
$(shell mkdir -p build)

all: $(OUTPUT_FILE)

# Rule to compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into shared library
$(OUTPUT_FILE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS)

# Clean up build artifacts
clean:
	rm -f $(OBJ_FILES) $(OUTPUT_FILE)

# Ensure build directory exists
build:
	mkdir -p build

# Phony targets
.PHONY: all clean build