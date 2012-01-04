#
# makefile for stref unit tests (using Boost.Test)
#
# platform: OS X, using Boost 1.48.0 and GCC 4.7 (from MacPorts)
#

CC = g++-mp-4.7
LIBS = /opt/local/lib/libboost_unit_test_framework-mt.a

OPTS = --pedantic -Wall --std=c++11 -I /opt/local/include

stref: stref.cpp stref.h
	$(CC) $(OPTS) $(LIBS) stref.cpp -o stref
