CPP=g++
CFLAGS:=-Wall  -Wextra -g -c
TARGET_FOLDER=../build/
BUFFER=buffer/
OBJS=$(addprefix $(TARGET_FOLDER),main.o buffer.o)

rebuild_and_start: rebuild start

start:
	./$(TARGET_FOLDER)Syncronize

rebuild: clean build

build: main.o buffer.o
	$(CPP) $(OBJS) -o $(TARGET_FOLDER)Syncronize
	$(MAKE) clean_objs

main.o: main.cpp
	$(CPP) $(CFLAGS) $< -o $(TARGET_FOLDER)$@ -lpthread -lrt

buffer.o: $(BUFFER)buffer.cpp $(BUFFER)buffer.h
	$(CPP) $(CFLAGS) $< -o $(TARGET_FOLDER)$@

clean_objs:
	rm -rf $(OBJS)

clean: clean_objs
	rm -rf $(TARGET_FOLDER)*