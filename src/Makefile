# Build entrypoint

CC = g++
PROG = cjVision
ENTRY = cpp/entry/main.cpp
CPP = cpp/*.cpp
HEADERS = -Iinclude
CFLAGS = 

OPENCV = `pkg-config opencv --cflags --libs`
OPENCV2 = `pkg-config opencv2 --cflags --libs`
OPENCV2 = `pkg-config opencv3 --cflags --libs`
OPENCV4 = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV) $(OPENCV2) $(OPENCV3) $(OPENCV4)

MKDIR = mkdir -p
OUTDIR = build

all: 
	$(MKDIR) $(OUTDIR)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$(PROG) $(HEADERS) $(ENTRY) $(CPP) $(LIBS)

clean:
	rm -rf $(OUTDIR)/cjVision