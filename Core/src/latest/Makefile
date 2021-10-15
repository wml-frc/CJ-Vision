# Global/General Config
CC = g++ -std=c++17

MKDIR = mkdir -p
OUTDIR = build
PROG_NAME = cjVision
PROG = build/$(PROG_NAME)
CFLAGS = -pthread

# Some weird ju ju happened now i have a bunch (Should be pkg-config opencv or opencv4)
OPENCV = `pkg-config opencv --cflags --libs`
OPENCV2 = `pkg-config opencv2 --cflags --libs`
OPENCV3 = `pkg-config opencv3 --cflags --libs`
OPENCV4 = `pkg-config opencv4 --cflags --libs`

LIBS = $(OPENCV)
LIBS4 = $(OPENCV4)

#
# Entry
#
ENTRY_SRC = entry/main.cpp
ENTRY_DIR = entry
ENTRY_HEADERS = -Ientry

# recursive wildcard function
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

#
# Coproc files
#
COPROC_LOC = ../../../../Coproc/src
COPROC_SRCS = $(call rwildcard,$(COPROC_LOC)/main/,*.cpp)
COPROC_SRC_DIR = $(COPROC_LOC)/main/cpp

COPROC_HEADERS = -I$(COPROC_LOC)/main/include

#
# CJ VISION FILES
#
CJ_VISION_SRCS = $(call rwildcard,main/,*.cpp)
CJ_VISION_SRC_DIR = main/cpp

CJ_VISION_HEADERS= -Imain/include

#
# COMMON FILES
#
COMMON_LOC = ../../common
COMMON_SRCS = $(call rwildcard,$(COMMON_LOC)/main/,*.cpp)
COMMON_SRC_DIR = $(COMMON_LOC)/main/cpp

COMMON_HEADERS = -I$(COMMON_LOC)/main/include

#
# UDP_TransferNT
# NOTE: UDP_TransferNT is a header only lib (no need for src like common)
#
UDP_TRANSFERNT_LIB_LOC = $(COMMON_LOC)/libs/UDP_TransferNT/UDP_TransferNT
UDP_TRANSFERNT_LIB_HEADERS = -I$(UDP_TRANSFERNT_LIB_LOC)/include

#
# spdlog files
# NOTE: spdlog is a header only lib
SPDLOG_LIB_LOC = $(COMMON_LOC)/libs/spdlog
SPDLOG_LIB_HEADERS = -I$(SPDLOG_LIB_LOC)/include


# Obejcts from cpp files
ENTRY_OBJ := $(patsubst $(ENTRY_DIR)/%.cpp, $(OUTDIR)/%.o, $(ENTRY_SRC))
COPROC_OBJ := $(patsubst $(COPROC_SRC_DIR)/%.cpp, $(OUTDIR)/%.o, $(COPROC_SRCS))
CJ_VISION_OBJ := $(patsubst $(CJ_VISION_SRC_DIR)/%.cpp, $(OUTDIR)/%.o, $(CJ_VISION_SRCS))
COMMON_OBJ := $(patsubst $(COMMON_SRC_DIR)/%.cpp, $(OUTDIR)/%.o, $(COMMON_SRCS))

# All
ALL_HEADERS = $(CJ_VISION_HEADERS) $(COMMON_HEADERS) $(UDP_TRANSFERNT_LIB_HEADERS) $(SPDLOG_LIB_HEADERS) $(COPROC_HEADERS)
ALL_OBJ = $(ENTRY_OBJ) $(CJ_VISION_OBJ) $(COMMON_OBJ) $(COPROC_OBJ)
ALL_SRCS = $(ENTRY_SRC) $(CJ_VISION_SRCS) $(COMMON_SRCS) $(COPROC_SRCS)

# Compile script
MK_OUT = mkdir -p $@; rm -rf $@

COMPILE_OBJ1 = $(CC) -c -o $@ $(CFLAGS) $< $(ALL_HEADERS) $(LIBS)
COMPILE_OBJ4 = $(CC) -c -o $@ $(CFLAGS) $< $(ALL_HEADERS) $(LIBS4)
COMPILE_OBJ = $(COMPILE_OBJ4) || $(COMPILE_OBJ1)

COMPILE_PROG1 = $(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(ALL_OBJ) $(ALL_HEADERS) $(LIBS)
COMPILE_PROG4 = $(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(ALL_OBJ) $(ALL_HEADERS) $(LIBS4)
COMPILE_PROG = $(COMPILE_PROG4) || $(COMPILE_PROG1)

# Build program with cv 1
$(PROG): $(ALL_OBJ)
	@echo Building program
ifdef coproc
	@echo coproc enabled
	$(COMPILE_PROG) -DCOPROC
else
	$(COMPILE_PROG)
endif

# entry
$(OUTDIR)/%.o: $(ENTRY_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
ifdef coproc
	@echo coproc enabled
	$(COMPILE_OBJ) -DCOPROC
else
	$(COMPILE_OBJ)
endif

# coproc
$(OUTDIR)/%.o: $(COPROC_SRC_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
ifdef coproc
	@echo coproc enabled
	$(COMPILE_OBJ) -DCOPROC
else
	$(COMPILE_OBJ)
endif

# core
$(OUTDIR)/%.o: $(CJ_VISION_SRC_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
ifdef coproc
	@echo coproc enabled
	$(COMPILE_OBJ) -DCOPROC
else
	$(COMPILE_OBJ)
endif

# common
$(OUTDIR)/%.o: $(COMMON_SRC_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
ifdef coproc
	@echo coproc enabled
	$(COMPILE_OBJ) -DCOPROC
else
	$(COMPILE_OBJ)
endif


clean:
	rm -rf ${OUTDIR}