CC=cc
CXX=g++
RM=rm -f
CPPFLAGS=-g -fPIC -std=gnu++11 -Wall -Wextra -I. -I /usr/include/atem
LDFLAGS=-g 
LDLIBS=-latemkontroll

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
