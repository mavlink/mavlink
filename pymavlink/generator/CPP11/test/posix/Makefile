
CXX ?= g++
CXXFLAGS ?= -std=c++11 -g -O0 -Wall -pthread -I/usr/src/gtest
GENDIR ?= /tmp/mavlink
GTESTSRC ?= /usr/src/gtest/src/gtest-all.cc

TARGETS = mavtest_common mavtest_ardupilotmega

all: $(TARGETS)

mavtest_common: mtest.cpp $(GENDIR)/common/common.hpp
	$(CXX) $(CXXFLAGS) -I $(GENDIR)/common -o $@ $< $(GTESTSRC)

mavtest_ardupilotmega: mtest.cpp $(GENDIR)/ardupilotmega/ardupilotmega.hpp
	$(CXX) $(CXXFLAGS) -I $(GENDIR)/ardupilotmega -o $@ $< $(GTESTSRC)

clean:
	rm -f $(TARGETS)
