CXX = g++
CXXFLAGS = -Wall -std=c++11 -g $(shell root-config --cflags)
LDFLAGS = $(shell root-config --ldflags --libs)

SRCS = RooTrackerVtxBase.cpp JNuBeamFlux.cpp NRooTrackerVtx.cpp root_reader.cpp
OBJS = $(SRCS:.cpp=.o)
EXE = root_reader.exe
DICT = RootDict.cxx
DICT_OBJ = RootDict.o

all: $(EXE)

$(DICT): RooTrackerVtxBase.h JNuBeamFlux.h NRooTrackerVtx.h LinkDef.h
	rootcling -f $@ $^

$(DICT_OBJ): $(DICT)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXE): $(OBJS) $(DICT_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXE) $(DICT) $(DICT_OBJ) *.pcm *.o

# Explicitly define dependencies
RooTrackerVtxBase.o: RooTrackerVtxBase.cpp RooTrackerVtxBase.h
JNuBeamFlux.o: JNuBeamFlux.cpp JNuBeamFlux.h RooTrackerVtxBase.h
NRooTrackerVtx.o: NRooTrackerVtx.cpp NRooTrackerVtx.h JNuBeamFlux.h RooTrackerVtxBase.h
root_reader.o: root_reader.cpp RooTrackerVtxBase.h JNuBeamFlux.h NRooTrackerVtx.h