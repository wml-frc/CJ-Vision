# Build entrypoint

# Global/General Config
CC = g++
PROG = cjVision
ENTRY = cpp/entry/main.cpp
USER_CPP = ../../Coproc/src/main/cpp/*.cpp
USER_HEADERS = -I../../Coproc/src/main/include
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

# global compiling attributes
COMMON_COMPILE = $(CC) $(CFLAGS) -o $(OUTDIR)/$(PROG) $(HEADERS) $(USER_HEADERS) $(USER_CPP) $(ENTRY) $(CPP)

all: # Maker for Desktop
	$(MAKE) all1 || $(MAKE) all4

all1:
	$(MKDIR) $(OUTDIR)
	$(COMMON_COMPILE) $(LIBS) $(LOCAL_DEFS)

all4:
	$(MKDIR) $(OUTDIR)
	$(COMMON_COMPILE) $(LIBS4) $(LOCAL_DEFS)

coprocessor: # Maker for Coprocessor
	$(MAKE) coproc1 || coproc4

coproc1:
	$(MKDIR) $(OUTDIR)
	$(COMMON_COMPILE) $(LIBS) $(COPROC_DEFS)

coproc4:
	$(MKDIR) $(OUTDIR)
	$(COMMON_COMPILE) $(LIBS4) $(COPROC_DEFS)

clean:
	rm -rf $(OUTDIR)/cjVision