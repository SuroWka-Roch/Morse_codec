CC=gcc
CFLAGS=-lm
OBJ = morse.o test.o
TARGET = test.exe

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)
	chmod +x $(TARGET)

.PHONY: clean

clean:
	rm $(OBJ) $(TARGET)
