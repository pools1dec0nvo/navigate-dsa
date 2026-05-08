TARGET=navigate
OBJ_FILES= main.o memory.o dfs.o map.o file.o
CC = gcc
CFLAGS = -Wall 
STD = -std=c99
MODE = -O3

.PHONY: default clean

default: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(STD) $(MODE) -o $@ $(OBJ_FILES)

memory.o:  memory.c
	$(CC) $(CFLAGS) $(STD) $(MODE) -c $<

dfs.o: dfs.c point.h dfscontent.h
	$(CC) $(CFLAGS) $(STD) $(MODE) -c $<

map.o: map.c
	$(CC) $(CFLAGS) $(STD) $(MODE) -c $<

file.o: file.c
	$(CC) $(CFLAGS) $(STD) $(MODE) -c $<

main.o: main.c point.h dfscontent.h memory.h file.h map.h dfs.h
	$(CC) $(CFLAGS) $(STD) $(MODE) -c $<

clean:
	rm -f $(TARGET) $(OBJ_FILES)