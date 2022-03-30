CXX = g++
CFLAGS = -std=c++1z -lstdc++ -Wall -I../sql-parser/src/ -L../sql-parser/
LIB = -lsqlparser

TARGETS = main
ALL = autogen.o SECLink.o AttrConvert.o SqlParserLoader.o FreeConnexTree.o

all: $(ALL)
	$(CXX) $(CFLAGS) $(ALL) -o $(TARGETS) $(LIB)
	rm  *.o

autogen.o: FreeConnexTree.h SqlParserLoader.h AttrConvert.h SECLink.h
	$(CXX) $(CFLAGS) -c autogen.cpp

SECLink.o: SECLink.h
	$(CXX) $(CFLAGS) -c SECLink.cpp

AttrConvert.o: AttrConvert.h
	$(CXX) $(CFLAGS) -c AttrConvert.cpp

SqlParserLoader.o: SqlParserLoader.h
	$(CXX) $(CFLAGS) -c SqlParserLoader.cpp $(LIB)

FreeConnexTree.o: FreeConnexTree.h
	$(CXX) $(CFLAGS) -c FreeConnexTree.cpp 

.PHONY: clean

clean:
	rm  $(TARGETS) *.o