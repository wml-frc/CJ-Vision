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
LIBS4 = $(OPENCV4)

# Directories
MKDIR = mkdir -p
OUTDIR = build

# Local compile
LOCAL_DEFS =

# Coprocessor compile
COPROC_DEFS = -DCOPROC

all:
	$(MAKE) all1 || $(MAKE) all4

all1:
	$(MKDIR) $(OUTDIR)
	$(CC) $(LOCAL_DEFS) $(CFLAGS) -o $(OUTDIR)/$(PROG) $(HEADERS) $(ENTRY) $(CPP) $(LIBS)

all4:
	$(MKDIR) $(OUTDIR)
	$(CC) $(LOCAL_DEFS) $(CFLAGS) -o $(OUTDIR)/$(PROG) $(HEADERS) $(ENTRY) $(CPP) $(LIBS4)

coprocessor:
	$(MAKE) coproc1 || coproc4

coproc1:
	$(MKDIR) $(OUTDIR)
	$(CC) $(COPROC_DEFS) $(CFLAGS) -o $(OUTDIR)/$(PROG) $(HEADERS) $(ENTRY) $(CPP) $(LIBS)

coproc4:
	$(MKDIR) $(OUTDIR)
	$(CC) $(COPROC_DEFS) $(CFLAGS) -o $(OUTDIR)/$(PROG) $(HEADERS) $(ENTRY) $(CPP) $(LIBS4)

clean:
	rm -rf $(OUTDIR)/cjVision