# Build entrypoint

CC = g++
PROG = cjVision
ENTRY = cpp/entry/main.cpp
CPP = cpp/*.cpp
HEADERS = -Iinclude
CFLAGS = 

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

MKDIR = mkdir -p
OUTDIR = build

all: 
	$(MKDIR) $(OUTDIR)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$(PROG) $(HEADERS) $(ENTRY) $(CPP) $(LIBS)

clean:
	rm -rf output