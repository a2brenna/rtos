INCLUDE_DIR=$(shell echo ~)/local/include
LIBRARY_DIR=$(shell echo ~)/local/lib
DESDTIR=/
PREFIX=/usr

CXX=g++
CXXFLAGS=-L${LIBRARY_DIR} -I${INCLUDE_DIR} -O2 -g -std=c++14 -fPIC -Wall -Wextra -march=native

all: test objstore librtosfs.so

install: all
	mkdir -p ${DESTDIR}/${PREFIX}/lib
	mkdir -p ${DESTDIR}/${PREFIX}/include/rtos
	install -m 444 librtosfs.so ${DESTDIR}/${PREFIX}/lib/
	install -m 444 src/object_store.h ${DESTDIR}/${PREFIX}/include/rtos/
	install -m 444 src/types.h ${DESTDIR}/${PREFIX}/include/rtos/
	install -m 444 src/fs_store.h ${DESTDIR}/${PREFIX}/include/rtos/
	install -m 444 src/encode.h ${DESTDIR}/${PREFIX}/include/rtos/
	install -m 444 src/ref_log.h ${DESTDIR}/${PREFIX}/include/rtos/

test: src/test.cc ephemeral_store.o fs_store.o types.o encode.o
	${CXX} ${CXXFLAGS} -o test src/test.cc ephemeral_store.o fs_store.o types.o encode.o -lboost_program_options -lsodium

objstore: src/objstore.cc fs_store.o types.o encode.o
	${CXX} ${CXXFLAGS} -o objstore src/objstore.cc fs_store.o types.o encode.o -lboost_program_options -lsodium

ephemeral_store.o: src/ephemeral_store.h src/ephemeral_store.cc src/types.h
	${CXX} ${CXXFLAGS} -o ephemeral_store.o -c src/ephemeral_store.cc

fs_store.o: src/fs_store.h src/fs_store.cc src/types.h
	${CXX} ${CXXFLAGS} -o fs_store.o -c src/fs_store.cc

encode.o: src/encode.h src/encode.cc
	${CXX} ${CXXFLAGS} -o encode.o -c src/encode.cc

ref_log.o: src/ref_log.h src/ref_log.cc
	${CXX} ${CXXFLAGS} -o ref_log.o -c src/ref_log.cc

librtosfs.so: fs_store.o types.o encode.o ref_log.o
	${CXX} ${CXXFLAGS} -shared -Wl,-soname,librtosfs.so -o librtosfs.so fs_store.o types.o encode.o ref_log.o

types.o: src/types.h src/types.cc
	${CXX} ${CXXFLAGS} -o types.o -c src/types.cc

clean:
	objstore
	test
	rm -f *.o
	rm -f *.so
	rm -f *.a
