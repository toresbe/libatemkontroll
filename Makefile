CC=cc
CXX=g++
RM=rm -f
CPPFLAGS=-g -fPIC -std=gnu++11 -Wall -Wextra -I.
LDFLAGS=-g 
LDLIBS=$(shell pkg-config --libs ) -lboost_thread -lboost_system -lpthread -lboost_log

PROG=atemctl
SRCS=$(wildcard network/*.cpp) $(wildcard features/*.cpp) atemctl.cpp atem.cpp
OBJS=$(subst .cpp,.o,$(SRCS)) 

all:            $(PROG)

atemctl:      $(OBJS)
	$(CXX) $(LDFLAGS) -o $(PROG) $(OBJS) $(LDLIBS) 

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(PROG)
