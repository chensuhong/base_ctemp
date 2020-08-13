srcdir := src
incdir := include

HEADERS += $(incdir)/WFCTAEvent.h
HEADERS += $(incdir)/EventLinkDef.h

SOURCES += $(srcdir)/WFCTAEvent.cpp
SOURCES += $(srcdir)/EventDict.cpp
#SOURCES += $(srcdir)/WFCTAMap.cpp
#SOURCES += $(srcdir)/WFCTAShow.cpp
#SOURCES += $(srcdir)/WFCTAAdcShow.cpp
#SOURCES += $(srcdir)/WFCTAOccupencyShow.cpp


FLAGS = `${ROOTSYS}/bin/root-config --cflags --libs` -g -lMinuit -lMatrix
SHOW_FLAGS = `${ROOTSYS}/bin/root-config --cflags --glibs`
DEFINES  := -I. -I$(incdir)

#zip: zip.cc KM2AEvent.cpp WFCTAEvent.cpp KM2AEventDict.cpp
#	g++ -g -mcmodel=medium -o $@ $^ ${FLAGS}

#all:main.exe maptest.exe drawCimage.exe drawOccupency.exe
all:drawCimage.exe
#main.exe: main.cc $(SOURCES)
#	g++ -g -mcmodel=medium $(DEFINES) -o $@ $^ ${FLAGS}

#maptest.exe: maptest.cc $(SOURCES)
#	g++ -g -mcmodel=medium $(DEFINES) -o $@ $^ ${FLAGS}

drawCimage.exe: drawCimage.cc $(SOURCES)
	g++ -g -mcmodel=medium $(DEFINES) -o $@ $^ ${FLAGS}

#drawOccupency.exe: drawOccupency.cc $(SOURCES)
#	g++ -g -mcmodel=medium $(DEFINES) -o $@ $^ ${FLAGS}

$(srcdir)/EventDict.cpp: $(HEADERS)
	${ROOTSYS}/bin/rootcint -f $@ -c $^
	cp $(srcdir)/EventDict_rdict.pcm .

#clean:
#	rm $(srcdir)/EventDict_rdict.pcm $(srcdir)/EventDict.cpp -f *.o
