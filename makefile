CXX = clang++
DEBUG = -g
CPPFLAGS = $(DEBUG) -std=gnu++11
LDFLAGS = $(DEBUG) 
LDLIBS = 
RM = rm -f

SRCS = neural.cpp
OBJS = $(subst .cpp, .o, $(SRCS))

all: $(SRCS) main

main: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LDLIBS) -o stock

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean: $(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .dependtool

include .depend