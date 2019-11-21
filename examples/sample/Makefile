.PHONY: clean all

CXX = g++
CXX_FLAGS = -std=c++14 -o3 
CXX_LIBS_PATH = 
SRC_PATH = .
CXX_INC = 
CXX_LIBS = -lpq



OBJECTS = main.o ../../src/connection.o ../../src/result.o ../../src/pool.o ../../src/pooler.o ../../src/transaction.o ../../src/cursor.o

all: $(OBJECTS)
	$(CXX) $(CXX_FLAGS)  $(OBJECTS) $(CXX_LIBS_PATH) $(CXX_LIBS) -o simple


SUFFIXES: .cpp .o 

.cpp.o:
	$(CXX) $(CXX_FLAGS) $(CXX_INC) -c $*.cpp -o $*.o 

clean:
	rm -f $(OBJECTS) simple