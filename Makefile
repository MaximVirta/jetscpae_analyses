PROGRAM = HEPMCtoJTree

VERSION = JTKT
CXX = g++
CXXFLAGS = -g -O3 -Wall -Wno-deprecated 
LD = g++

CXXFLAGS += $(shell HepMC3-config --cflags)
LDFLAGS = $(shell HepMC3-config --libs)
SOFLAGS = -shared

CXXFLAGS += $(shell root-config --cflags) -fPIC
LDFLAGS += $(shell root-config --libs) -fPIC

HDRSDICT = src/JBaseTrack.h src/JBaseEventHeader.h

HDRS += $(HDRSDICT) nanoDict.h

SRC = $(HDRS:.h=.cxx)
OBJS = $(HDRS:.h=.o)

MAIN = HEPMCtoJTree.C

all: $(PROGRAM)

$(PROGRAM): $(OBJS) $(MAIN)
	@echo "Linking $(PROGRAM) ..."
	$(CXX) -lPhysics -L$(PWD) $(MAIN) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $(PROGRAM)
	@echo "done"

%.cxx:
%: %.cxx
	$(LINK.cc) $^ $(CXXFLAGS) $(LOADLIBES) $(LDLIBS) -o $@

%.o: %.cxx %.h
	$(COMPILE.cc) $(OUTPUT_OPTION) $<


clean:
	rm -f $(OBJS) core *Dict* $(PROGRAM).o *.d $(PROGRAM) $(PROGRAM).sl

cl:
	clean $(PROGRAM)

nanoDict.cc: $(HDRSDICT)
	@echo "Generating dictionary ..."
	@rm -f nanoDict.cc nanoDict.hh nanoDict.h
	@rootcling nanoDict.cc $(HDRSDICT)
