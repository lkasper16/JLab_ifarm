################################################################################ definition of the compiler options
#	-I location of directory containing include files
#	-L location of directory containing libraries
#       -lname include the library from -L location called libname.a
#	   -lg2c is the library containing info on converting fortran to C
#          -lf   is the library containing the intrinsic for HPUX only.
#	-shared make a shared library as output
#	-fPIC produce position independent code
#        necessary on some platforms (including HPUX) for -shared
# 	-fpic ^^ same(?)
#	-O optimizes
#	-g produces output for the debugger
#       -pg produces output for gprof profiler
#       note: if you want to see all warnings and ensure ansi standard 
#             compatibility, use:
#             -pipe -ansi -pedantic -fnonnull-objects \
#             -W -Wall -Wwrite-strings -Wpointer-arith -Wnested-externs \
#             -Woverloaded-virtual -Wbad-function-cast -fnonnull-objects
#       The proper order for cernlib libraries is:
#       -lpawlib -lgraflib -lgrafX11 -lmathlib -lkernlib -lpacklib -ljetset74
#
# makefile syntax:
#        for target thedir/target.suf from source anotherdir/source.suf2
#        ${*D}  = thedir
#        ${*F}  = target
#        $*     = thedir/target
#        $@     = thedir/target.suf
#        $<     = anotherdir/source.suf2
#  
#
################################################################################ Compiler options

#EVIO_DIR = /home/hdtrdops/SRS/GemViewForJLab/evio_3.09
#  get environment variable EVIO ! no space after ${EVIO} ! important 
EVIO_DIR = ${EVIO}
EVIO_INC = $(EVIO_DIR)/common/include/
EVIO_LIB = $(EVIO_DIR)/Linux-x86_64/lib/
EVIO_LINK = -L$(EVIO_LIB) -levio -levioxx  -lexpat

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

GEMDIR = gemrecon
GEMINC = $(GEMDIR)
GEMOBJ = $(GEMDIR)
GEMLIB = $(GEMDIR)
GEM_LINK =  -L$(GEMDIR) -lgem 

DEBUGFLAGS   = -O0 -D _DEBUG
RELEASEFLAGS = -O2 -D NDEBUG -combine -fwhole-program
CFLAGS       = -fPIC -shared -pedantic -Wall -Wextra -march=native -ggdb3
LDFLAGS      = -shared


INCLUDES    = -I.   -I$(EVIO_INC) -I$(GEMINC)

CXX     =  g++ 
F77	=  gfortran

CXXFLAGS     =    -std=c++11 -g  $(INCLUDES) $(ROOTCFLAGS)

LINK_FLAGS   =   $(DFLG) -fno-second-underscore

LINK_LIBS    =  $(ROOTLIBS)  $(GEM_LINK) $(EVIO_LINK) 

ifeq "$(UNAME)" "Linux"
ifeq "$(F77)" "gfortran"
    LINK_LIBS     += -lgfortran
else
    LINK_LIBS     += -lg2c 
endif
endif


HEADERS   = *.h
PROGRAMS  = DAQCrateAnalyzer2

#OBJGEM =  $(GEMOBJ)/GEMRawDecoder.o $(GEMOBJ)/GemFEC.o $(GEMOBJ)/GemHistContainer.o $(GEMOBJ)/GemView.o $(GEMOBJ)/GemMapping.o $(GEMOBJ)/GemConfiguration.o $(GEMOBJ)/GemInputHandler.o $(GEMOBJ)/GemException.o $(GEMOBJ)/GEMOnlineHitDecoder.o $(GEMOBJ)/GEMPedestal.o $(GEMOBJ)/GEMHit.o $(GEMOBJ)/GEMCluster.o $(GEMOBJ)/GEMRawPedestal.o

###############################################################################
#
.SUFFIXES:      .o .cc .cxx .f .exe
all:	$(PROGRAMS)

DAQCrateAnalyzer2: DAQCrateAnalyzer2.o
	@echo "Building $@ ..."
	$(CXX) $(LINK_FLAGS) DAQCrateAnalyzer2.o  $(LINK_LIBS) -o $@

###############################################################################
# instructions for building a .o file from a .cxx file
#
.cc.o:         $(HEADERS) $<
	@echo "Compiling $< with $(CXX) ..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

###############################################################################
# instructions for building a .o file from a .cxx file
#
.c.o:         $(HEADERS) $<
	@echo "Compiling $< with $(CXX) ..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

###############################################################################
# instructions for building a .o file from a .f file
#
.f.o:           $<
	@echo "Compiling $< with $(F77) ..."
	@$(F77) $(FLAGS) -c $< -o $@

###############################################################################
# gmake clean       removes all garbage from HepMC directories.
#
clean:
	rm -f *.o

###############################################################################
# gmake distclean       removes all compiled libraries, executables, +garbage
#                       to prepare the package for distribution
distclean: 
	$(MAKE) clean --no-print-directory
	rm -f *.exe
	rm -f *.dat

test:
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "INCLUDES = $(INCLUDES)"
	@echo "EVIO_DIR = $(EVIO_DIR)"
	@echo "EVIO_INC = $(EVIO_INC)"
	@echo "EVIO_LIB = $(EVIO_LIB)"
	@echo "HEADERS = $(HEADERS)"
