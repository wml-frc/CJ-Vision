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

#
# MJPEGWriter
#
MJPEG_WRITER_LOC = $(COMMON_LOC)/libs/MJPEGWriter
MJPEG_WRITER_SRCS = $(call rwildcard,$(MJPEG_WRITER_LOC)/main/,*.cpp)
MJPEG_WRITER_SRC_DIR = $(MJPEG_WRITER_LOC)/main/cpp

MJPEG_WRITER_HEADERS = -I$(MJPEG_WRITER_LOC)/main/include


#
# PropTop NN
#
PROPTOP_LOC = $(COMMON_LOC)/libs/PropTop/lib/src
PROPTOP_SRCS = $(call rwildcard,$(PROPTOP_LOC)/main/,*.cpp)
PROPTOP_SRC_DIR = $(PROPTOP_LOC)/main/cpp

PROPTOP_HEADERS = -I$(PROPTOP_LOC)/main/public



# Obejcts from cpp files
ENTRY_OBJ := $(patsubst $(ENTRY_DIR)/%.cpp, $(OUTDIR)/%.o, $(ENTRY_SRC))
COPROC_OBJ := $(patsubst $(COPROC_SRC_DIR)/%.cpp, $(OUTDIR)/%.o, $(COPROC_SRCS))
CJ_VISION_OBJ := $(patsubst $(CJ_VISION_SRC_DIR)/%.cpp, $(OUTDIR)/%.o, $(CJ_VISION_SRCS))
COMMON_OBJ := $(patsubst $(COMMON_SRC_DIR)/%.cpp, $(OUTDIR)/%.o, $(COMMON_SRCS))
MJPEG_WRITER_OBJ := $(patsubst $(MJPEG_WRITER_SRC_DIR)/%.cpp, $(OUTDIR)/%.o, $(MJPEG_WRITER_SRCS))
PROPTOP_OBJ := $(patsubst $(PROPTOP_SRC_DIR)/%.cpp, $(OUTDIR)/%.o, $(PROPTOP_SRCS))

# All
ALL_HEADERS = $(CJ_VISION_HEADERS) $(COMMON_HEADERS) $(UDP_TRANSFERNT_LIB_HEADERS) $(SPDLOG_LIB_HEADERS) $(COPROC_HEADERS) $(MJPEG_WRITER_HEADERS) $(PROPTOP_HEADERS)
ALL_OBJ = $(ENTRY_OBJ) $(CJ_VISION_OBJ) $(COMMON_OBJ) $(COPROC_OBJ) $(MJPEG_WRITER_OBJ) $(PROPTOP_OBJ)
ALL_SRCS = $(ENTRY_SRC) $(CJ_VISION_SRCS) $(COMMON_SRCS) $(COPROC_SRCS) $(MJPEG_WRITER_SRCS) $(PROPTOP_SRCS)

# Compile script
MK_OUT = mkdir -p $@; rm -rf $@

ifdef coproc
	CFLAGS += -DCOPROC
endif

COMPILE_OBJ1 = $(CC) -c -o $@ $(CFLAGS) $< $(ALL_HEADERS) $(LIBS)
COMPILE_OBJ4 = $(CC) -c -o $@ $(CFLAGS) $< $(ALL_HEADERS) $(LIBS4)
COMPILE_OBJ = $(COMPILE_OBJ4) || $(COMPILE_OBJ1)

COMPILE_PROG1 = $(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(ALL_OBJ) $(ALL_HEADERS) $(LIBS)
COMPILE_PROG4 = $(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(ALL_OBJ) $(ALL_HEADERS) $(LIBS4)
COMPILE_PROG = $(COMPILE_PROG4) || $(COMPILE_PROG1)

# Build program with cv 1
$(PROG): $(ALL_OBJ)
	$(COMPILE_PROG)

# entry
$(OUTDIR)/%.o: $(ENTRY_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
	$(COMPILE_OBJ)

# coproc
$(OUTDIR)/%.o: $(COPROC_SRC_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
	$(COMPILE_OBJ)

# core
$(OUTDIR)/%.o: $(CJ_VISION_SRC_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
	$(COMPILE_OBJ)

# common
$(OUTDIR)/%.o: $(COMMON_SRC_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
	$(COMPILE_OBJ)

# MJPEG Writer
$(OUTDIR)/%.o: $(MJPEG_WRITER_SRC_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
	$(COMPILE_OBJ)

# PropTop
$(OUTDIR)/%.o: $(PROPTOP_SRC_DIR)/%.cpp
	@echo Building objects
	$(MK_OUT)
	$(COMPILE_OBJ)

clean:
	rm -rf ${OUTDIR}