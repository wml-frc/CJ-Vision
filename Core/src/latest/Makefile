# Global/General Config
CC = g++

MKDIR = mkdir -p
OUTDIR = build
PROG_NAME = cjVision
PROG = build/$(PROG_NAME)
CFLAGS = -pthread

#
# Entry
#
ENTRY_SRC = entry/main.cpp
ENTRY_DIR = entry
ENTRY_HEADERS = -Ientry

# recursive wildcard function
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

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
#
UDP_TRANSFERNT_LIB_LOC = $(COMMON_LOC)/libs/UDP_TransferNT/lib/src
UDP_TRANSFERNT_LIB_SRCS = $(call rwildcard,$(UDP_TRANSFERNT_LIB_LOC)/main/,*.cpp)
UDP_TRANSFERNT_LIB_SRC_DIR = $(UDP_TRANSFERNT_LIB_LOC)/main/cpp

UDP_TRANSFERNT_LIB_HEADERS = -I$(UDP_TRANSFERNT_LIB_LOC)/main/public

#
# spdlog files
# NOTE: spdlog is a header only lib
SPDLOG_LIB_LOC = $(COMMON_HEADERS)/libs/spdlog
SPDLOG_LIB_HEADERS = $(SPDLOG_LIB_LOC)/include


# Obejcts from cpp files
ENTRY_OBJ := $(patsubst $(ENTRY_DIR)/%.cpp, $(OUTDIR)/%.o, $(ENTRY_SRC))
CJ_VISION_OBJ := $(patsubst $(CJ_VISION_SRC_DIR)/%.cpp, $(OUTDIR)/%.o, $(CJ_VISION_SRCS))
COMMON_OBJ := $(patsubst $(COMMON_SRC_DIR)/%.cpp, $(OUTDIR)/%.o, $(COMMON_SRCS))
UDP_TRANSFERNT_LIB_OBJ := $(patsubst $(UDP_TRANSFERNT_LIB_SRC_DIR)/%.cpp, $(OUTDIR)/%.o, $(UDP_TRANSFERNT_LIB_SRCS))

# All
ALL_HEADERS = $(CJ_VISION_HEADERS) $(COMMON_HEADERS) $(UDP_TRANSFERNT_LIB_HEADERS) $(SPDLOG_LIB_HEADERS)
ALL_OBJ = $(ENTRY_OBJ) $(CJ_VISION_OBJ) $(COMMON_OBJ) $(UDP_TRANSFERNT_LIB_OBJ)
ALL_DEP = $(ENTRY_DEP) $(CJ_VISION_DEP) $(COMMON_DEP) $(UDP_TRANSFERNT_LIB_DEP)
ALL_SRCS = $(ENTRY_SRC) $(CJ_VISION_SRCS) $(COMMON_SRCS) $(UDP_TRANSFERNT_LIB_SRCS)

# Compile script
MK_OUT = mkdir -p $@; rm -rf $@

COMPILE_OBJ = $(CC) -c -o $@ $(CFLAGS) $< $(ALL_HEADERS)
COMPILE_PROG = $(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(ALL_OBJ) $(ALL_HEADERS)

$(PROG): $(ALL_OBJ)
	@echo Building program
	$(COMPILE_PROG)

$(OUTDIR)/%.o: $(ENTRY_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
	$(COMPILE_OBJ)

$(OUTDIR)/%.o: $(CJ_VISION_SRC_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
	$(COMPILE_OBJ)

$(OUTDIR)/%.o: $(COMMON_SRC_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
	$(COMPILE_OBJ)

$(OUTDIR)/%.o: $(UDP_TRANSFERNT_LIB_SRC_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
	$(COMPILE_OBJ)


clean:
	rm -rf ${OUTDIR}