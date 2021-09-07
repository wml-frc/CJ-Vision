# Global/General Config
CC = g++
ENTRY = cpp/entry/main.cpp

MKDIR = mkdir -p
OUTDIR = build
PROG = build/cjVision

OBJ = $(OUTDIR)/main.o $(OUTDIR)/test.o

COMPILE = $(CC) $(OUTDIR)/$(PROG) -o
# COMPILE_OBJ = $(CC) -o

$(PROG): $(OUTDIR) $(OBJ)
	g++ $(OBJ) -o $(PROG)

$(OUTDIR)/main.o: $(ENTRY) $(OUTDIR)/test.o
	g++ -c $(ENTRY) -o $(OUTDIR)/main.o

$(OUTDIR)/test.o: cpp/entry/test.cpp
	g++ -c cpp/entry/test.cpp -o $(OUTDIR)/test.o

$(OUTDIR):
	$(MKDIR) $(OUTDIR)


clean:
	rm -rf ${OUTDIR}