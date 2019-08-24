CC=cc
CXX=g++
RM=rm -f
CPPFLAGS=-g -std=gnu++11 -Wall -Wextra -I. -I ../../
LDFLAGS=-g 
LDLIBS=-latemkontroll -Wl,--whole-archive -lpthread -Wl,--no-whole-archive

PROG=atemctl
SRCS=atemctl.cpp
OBJS=$(subst .cpp,.o,$(SRCS)) 

all:            $(PROG)

atemctl:      $(OBJS)
	$(CXX) $(LDFLAGS) -o $(PROG) $(OBJS) $(LDLIBS) 

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(PROG)
