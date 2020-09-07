# Build entrypoint

# Global/General Config
CC = g++
PROG = cjVision
ENTRY = cpp/entry/main.cpp
CPP = cpp/*.cpp
HEADERS = -Iinclude
CFLAGS = -pthread

# Some weird ju ju happened now i have a bunch (Should be pkg-config opencv)
OPENCV = `pkg-config opencv --cflags --libs`
OPENCV2 = `pkg-config opencv2 --cflags --libs`
OPENCV3 = `pkg-config opencv3 --cflags --libs`
OPENCV4 = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

# Directories
MKDIR = mkdir -p
OUTDIR = build

# Local compile
LOCAL_DEFS =

# Coprocessor compile
COPROC_DEFS = -DCOPROC

all: 
	$(MKDIR) $(OUTDIR)
	$(CC) $(LOCAL_DEFS) $(CFLAGS) -o $(OUTDIR)/$(PROG) $(HEADERS) $(ENTRY) $(CPP) $(LIBS)

coprocessor:
	$(MKDIR) $(OUTDIR)
	$(CC) $(COPROC_DEFS) $(CFLAGS) -o $(OUTDIR)/$(PROG) $(HEADERS) $(ENTRY) $(CPP) $(LIBS)

clean:
	rm -rf $(OUTDIR)/cjVision