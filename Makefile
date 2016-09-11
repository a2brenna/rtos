INCLUDE_DIR=$(shell echo ~)/local/include
LIBRARY_DIR=$(shell echo ~)/local/lib
DESDTIR=/
PREFIX=/usr

CXX=clang++
CXXFLAGS=-L${LIBRARY_DIR} -I${INCLUDE_DIR} -O2 -g -std=c++14 -fPIC -Wall -Wextra -march=native

all: test

test: src/test.cc ephemeral_store.o types.o
	${CXX} ${CXXFLAGS} -o test src/test.cc ephemeral_store.o types.o -lstdc++

ephemeral_store.o: src/ephemeral_store.h src/ephemeral_store.cc src/types.h
	${CXX} ${CXXFLAGS} -o ephemeral_store.o -c src/ephemeral_store.cc

types.o: src/types.h src/types.cc
	${CXX} ${CXXFLAGS} -o types.o -c src/types.cc

clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a