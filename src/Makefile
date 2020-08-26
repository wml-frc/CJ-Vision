# Build entrypoint

CC = g++
PROG = cjVision
ENTRY = cpp/entry/main.cpp
CPP = cpp/*.cpp
HEADERS = -Iinclude
CFLAGS = 

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

all: 
	$(CC) $(CFLAGS) -o $(PROG) $(HEADERS) $(ENTRY) $(CPP) $(LIBS)

clean:
	rm -rf output