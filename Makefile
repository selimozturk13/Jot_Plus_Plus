CC = g++
CFLAGS = -std=c++14
FLTK_FLAGS = -lfltk -lfltk_images -lsqlite3
MAC_FLAGS = -I/usr/local/include -L/usr/local/lib $(FLTK_FLAGS)
LINUX_FLAGS = $(FLTK_FLAGS) -lX11 -lpthread
SRC = jotplusplus.cpp
TARGET = jotplusplus

OS_NAME := $(shell uname -s)

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


.PHONY: clean install static

clean:
	rm -f notes.db $(TARGET) *.txt
	@echo "Cleaned successfully."
static: $(SRC)
ifeq ($(OS_NAME),Darwin)
	$(CC) $(CFLAGS) /usr/local/lib/libfltk.a /usr/local/lib/libfltk_images.a /usr/local/lib/libsqlite3.a -o $(TARGET) $(SRC) -lpthread -framework Cocoa -framework OpenGL -framework IOKit
	@echo "Statically linked binary created for macOS"
else ifeq ($(OS_NAME),Linux)
	$(CC) $(CFLAGS) /usr/local/lib/libfltk.a /usr/local/lib/libfltk_images.a /usr/local/lib/libsqlite3.a -o $(TARGET) $(SRC) -lX11 -lpthread
	@echo "Statically linked binary created for Linux"
endif

install: $(TARGET)
ifeq ($(OS_NAME),Darwin)
	cp $(TARGET) /usr/local/bin/
	@echo "Installed successfully on macOS"
else ifeq ($(OS_NAME),Linux)
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installed successfully on Linux"
endif

