# Build entrypoint

# Global/General Config
CC = g++
PROG = cjVision
ENTRY = cpp/entry/main.cpp

# Dirs
USER_CPP = ../../Coproc/src/main/cpp/*.cpp
USER_HEADERS = -I../../Coproc/src/main/include
COPROC_CPP = ../../UserProgram/main/cpp/*.cpp
COPROC_HEADERS = -I../../UserProgram/main/include

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

# Local compile (don't need any for now)
LOCAL_DEFS =

# Coprocessor compile
COPROC_DEFS = -DCOPROC

# global compiling attributes
LOCAL_COMPILE = $(CC) $(CFLAGS) -o $(OUTDIR)/$(PROG) $(HEADERS) $(USER_HEADERS) $(USER_CPP) $(ENTRY) $(CPP)
COPROC_COMPILE = $(CC) $(CFLAGS) -o $(OUTDIR)/$(PROG) $(HEADERS) $(COPROC_HEADERS) $(COPROC_CPP) $(ENTRY) $(CPP)
DEBUG_COMPILE = $(CC) $(CFLAGS) -o $(OUTDIR)/$(PROG) $(HEADERS) $(ENTRY) $(CPP)

all: # Maker for Desktop
	$(MAKE) all1 || $(MAKE) all4

all1:
	$(MKDIR) $(OUTDIR)
	$(LOCAL_COMPILE) $(LIBS) $(LOCAL_DEFS)

all4:
	$(MKDIR) $(OUTDIR)
	$(LOCAL_COMPILE) $(LIBS4) $(LOCAL_DEFS)

debug: # debuger. (mostly used for azure when Coproc dir doesn't exist)
	$(MAKE) debug1 || $(MAKE) debug4

debug1:
	$(MKDIR) $(OUTDIR)
	$(DEBUG_COMPILE) $(LIBS) $(LOCAL_DEFS) -DDEBUG

debug4:
	$(MKDIR) $(OUTDIR)
	$(DEBUG_COMPILE) $(LIBS4) $(LOCAL_DEFS) -DDEBUG


coprocessor: # Maker for Coprocessor
	$(MAKE) coproc1 || coproc4

coproc1:
	$(MKDIR) $(OUTDIR)
	$(COPROC_COMPILE) $(LIBS) $(COPROC_DEFS)

coproc4:
	$(MKDIR) $(OUTDIR)
	$(COPROC_COMPILE) $(LIBS4) $(COPROC_DEFS)

clean:
	rm -rf $(OUTDIR)/cjVision