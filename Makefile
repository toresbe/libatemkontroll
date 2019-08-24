CC=cc
CXX=g++
RM=rm -f
MAKE=make
CPPFLAGS=-g -pedantic -fPIC -std=gnu++11 -Wall -Wextra -I. 
LDFLAGS=-shared -pthread

TARGET_LIB=libatemkontroll.so
SRCS=$(wildcard network/*.cpp) $(wildcard features/*.cpp) atem.cpp
OBJS=$(subst .cpp,.o,$(SRCS)) 

PREFIX=/usr
DEST_DIR=$(PREFIX)/share

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
	cp $(TARGET_LIB) $(DESTDIR)$(PREFIX)/lib/$(TARGET_LIB)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include/
	mkdir -p $(DESTDIR)$(PREFIX)/include/atem/network
	mkdir -p $(DESTDIR)$(PREFIX)/include/atem/features
	cp atem.hpp $(DESTDIR)$(PREFIX)/include/
	cp $(wildcard network/*.hpp) $(DESTDIR)$(PREFIX)/include/atem/network
	cp $(wildcard features/*.hpp) $(DESTDIR)$(PREFIX)/include/atem/features
