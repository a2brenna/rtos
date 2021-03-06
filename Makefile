INCLUDE_DIR=$(shell echo ~)/local/include
LIBRARY_DIR=$(shell echo ~)/local/lib
DESDTIR=/
PREFIX=/usr
CXX=g++
CXXFLAGS=-D_FILE_OFFSET_BITS=64 -L${LIBRARY_DIR} -I${INCLUDE_DIR} -O2 -g -std=c++14 -fPIC -Wall -Wextra -march=native

all: rtos rtosd librtosfs.so librrtos.so

install: all
	mkdir -p ${DESTDIR}/${PREFIX}/lib
	mkdir -p ${DESTDIR}/${PREFIX}/include/rtos
	install -m 444 librtosfs.so ${DESTDIR}/${PREFIX}/lib/
	install -m 444 librrtos.so ${DESTDIR}/${PREFIX}/lib/
	install -m 444 src/object_store.h ${DESTDIR}/${PREFIX}/include/rtos/
	install -m 444 src/types.h ${DESTDIR}/${PREFIX}/include/rtos/
	install -m 444 src/fs_store.h ${DESTDIR}/${PREFIX}/include/rtos/
	install -m 444 src/encode.h ${DESTDIR}/${PREFIX}/include/rtos/
	install -m 444 src/remote_store.h ${DESTDIR}/${PREFIX}/include/rtos/

rtosd: src/rtosd.cc fs_store.o types.o encode.o wire_protocol.o address_parse.o
	${CXX} ${CXXFLAGS} -o rtosd src/rtosd.cc fs_store.o types.o encode.o wire_protocol.o address_parse.o -lboost_program_options -lsodium -lsmplsocket -lpthread -lprotobuf

rtos: src/rtos.cc fs_store.o types.o encode.o remote_store.o wire_protocol.o address_parse.o
	${CXX} ${CXXFLAGS} -o rtos src/rtos.cc fs_store.o types.o encode.o remote_store.o wire_protocol.o address_parse.o -lboost_program_options -lsodium -lsmplsocket -lprotobuf

ephemeral_store.o: src/ephemeral_store.h src/ephemeral_store.cc src/types.h
	${CXX} ${CXXFLAGS} -o ephemeral_store.o -c src/ephemeral_store.cc

fs_store.o: src/fs_store.h src/fs_store.cc src/types.h
	${CXX} ${CXXFLAGS} -o fs_store.o -c src/fs_store.cc

remote_store.o: src/remote_store.h src/remote_store.cc src/types.h src/wire_protocol.pb.h
	${CXX} ${CXXFLAGS} -o remote_store.o -c src/remote_store.cc

encode.o: src/encode.h src/encode.cc
	${CXX} ${CXXFLAGS} -o encode.o -c src/encode.cc

librtosfs.so: fs_store.o types.o encode.o
	${CXX} ${CXXFLAGS} -shared -Wl,-soname,librtosfs.so -o librtosfs.so fs_store.o types.o encode.o

librrtos.so: remote_store.o types.o encode.o wire_protocol.o
	${CXX} ${CXXFLAGS} -shared -Wl,-soname,librrtos.so -o librrtos.so remote_store.o types.o encode.o wire_protocol.o

types.o: src/types.h src/types.cc
	${CXX} ${CXXFLAGS} -o types.o -c src/types.cc

address_parse.o: src/address_parse.h src/address_parse.cc
	${CXX} ${CXXFLAGS} -o address_parse.o -c src/address_parse.cc

wire_protocol.o: src/wire_protocol.pb.h
	${CXX} ${CXXFLAGS} -c src/wire_protocol.pb.cc -o wire_protocol.o

src/wire_protocol.pb.h: wire_protocol.proto
	protoc --cpp_out=src/ wire_protocol.proto

clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f src/*.pb.h
	rm -f src/*.pb.cc
