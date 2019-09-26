CC=cc
CXX=g++
RM=rm -f
MAKE=make
CPPFLAGS=-g -pedantic -fPIC -std=gnu++11 -Wall -Wextra -I. 
LDFLAGS=-shared -pthread -ldl

TARGET_LIB=libatemkontroll.so
SRCS=$(wildcard network/*.cpp) $(wildcard features/*.cpp) atem.cpp loguru.cpp
OBJS=$(subst .cpp,.o,$(SRCS)) 

PREFIX=/usr

all:	${TARGET_LIB} utils

${TARGET_LIB}:      $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) ${TARGET_LIB} ${OBJS}

utils:	utils/
	$(MAKE) -C utils

.PHONY: install
install: $(TARGET_LIB)
	cp $(TARGET_LIB) $(PREFIX)/lib/$(TARGET_LIB)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include/
	mkdir -p $(PREFIX)/include/atem/network
	mkdir -p $(PREFIX)/include/atem/features
	cp atem.hpp $(PREFIX)/include/
	cp $(wildcard network/*.hpp) $(PREFIX)/include/atem/network
	cp $(wildcard features/*.hpp) $(PREFIX)/include/atem/features
	cp libatemkontroll.pc $(PREFIX)/lib/pkgconfig/
