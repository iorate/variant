
# iorate::variant
#
# Copyright iorate 2017.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

ifeq ($(OS),Windows_NT)
EXEEXT := .exe
endif

CXX := g++
CXXFLAGS := -std=c++1z -Wall -Wextra -pedantic-errors
LIBS := -lboost_unit_test_framework-mt
OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
PROGRAM := variant_test$(EXEEXT)

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

run: $(PROGRAM)
	./$(PROGRAM)

clean:
	rm -rf $(PROGRAM) $(OBJS)

$(filter-out main.o,$(OBJS)): ../variant.hpp common.hpp
