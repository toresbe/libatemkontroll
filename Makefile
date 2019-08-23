CC=cc
CXX=g++
RM=rm -f
CPPFLAGS=-g -pedantic -fPIC -std=gnu++11 -Wall -Wextra -I.
LDFLAGS=-shared 

TARGET_LIB=libatemkontroll.so
SRCS=$(wildcard network/*.cpp) $(wildcard features/*.cpp) atem.cpp
OBJS=$(subst .cpp,.o,$(SRCS)) 

PREFIX=/usr
DEST_DIR=$(PREFIX)/share

all:            ${TARGET_LIB}

${TARGET_LIB}:      $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) ${TARGET_LIB} ${OBJS}

.PHONY: install
install: $(TARGET_LIB)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include
	cp $(TARGET_LIB) $(DESTDIR)$(PREFIX)/lib/$(TARGET_LIB)
	cp foo.h $(DESTDIR)$(PREFIX)/include/

