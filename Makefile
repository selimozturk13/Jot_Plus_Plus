CC = g++
CFLAGS = -std=c++14
FLTK_FLAGS = -lfltk -lfltk_images -lsqlite3
MAC_FLAGS = -I/usr/local/include -L/usr/local/lib $(FLTK_FLAGS)
LINUX_FLAGS = $(FLTK_FLAGS) -lX11 -lpthread
SRC = jotplusplus.cpp
TARGET = jotplusplus

# OS tespiti
OS_NAME := $(shell uname -s)

# Derleme
$(TARGET): $(SRC)
ifeq ($(OS_NAME),Darwin)
	$(CC) $(CFLAGS) $(MAC_FLAGS) -o $(TARGET) $(SRC)
	@echo "Compiled successfully on macOS"
else ifeq ($(OS_NAME),Linux)
	$(CC) $(CFLAGS) $(LINUX_FLAGS) -o $(TARGET) $(SRC)
	@echo "Compiled successfully on Linux"
else
	@echo "jotplusplus cannot work on $(OS_NAME)"
endif

# Temizlik
.PHONY: clean

clean:
	rm -f notes.db $(TARGET) *.txt
	@echo "Cleaned successfully."