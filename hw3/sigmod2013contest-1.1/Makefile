# Copyright (c) 2013 KAUST - InfoCloud Group (All Rights Reserved)
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
# 
# Authors: Amin Allam  <amin.allam@kaust.edu.sa>,
#          Fuad Jamour <fuad.jamour@kaust.edu.sa>
#
# Current version: 1.0 (initial release)
	

# Build targets (your implementation targets should go in IMPL_O)
TEST_O=test_driver/test.o 
IMPL_O=ref_impl/core.o
BKTREE_O=../Indexes/BKTree.o
ENTRY_O=../Lists/entry.o
DOC_O=../Lists/doc.o
PAYLOAD_O=../Lists/payload.o
HASHTABLE_O=../Indexes/hashtable.o
Q_HASHTABLE_O=../Indexes/q_hashtable.o
JOBS_O=../Lists/jobs.o
ACUTEST_O=../acutest.o

# Compiler flags
CC  = gcc
CXX = g++ -std=c++11
CFLAGS=-O3 -fPIC -Wall -g -I. -I./include -pthread
CXXFLAGS=$(CFLAGS)
LDFLAGS=-lpthread

# The programs that will be built
PROGRAMS=testdriver

# The name of the library that will be built
LIBRARY=core

# Build all programs
all: $(PROGRAMS)

lib: $(IMPL_O) $(DOC_O) $(BKTREE_O) $(ENTRY_O) $(HASHTABLE_O) $(PAYLOAD_O) $(Q_HASHTABLE_O) $(JOBS_O)
	$(CXX) $(CXXFLAGS) -shared -o lib$(LIBRARY).so  $(IMPL_O) $(DOC_O) $(BKTREE_O) $(ENTRY_O) $(HASHTABLE_O) $(PAYLOAD_O) $(Q_HASHTABLE_O) $(JOBS_O)

testdriver: lib $(TEST_O) $(DOC_O) $(BKTREE_O) $(ENTRY_O) $(HASHTABLE_O) $(PAYLOAD_O) $(Q_HASHTABLE_O) $(JOBS_O)
	$(CXX) $(CXXFLAGS) -o testdriver $(TEST_O) $(DOC_O) $(BKTREE_O) $(ENTRY_O) $(HASHTABLE_O) $(PAYLOAD_O) $(Q_HASHTABLE_O) $(JOBS_O) ./lib$(LIBRARY).so

clean:
	rm -f $(PROGRAMS) $(DOC_O) $(BKTREE_O) $(ENTRY_O) $(HASHTABLE_O) $(PAYLOAD_O) $(JOBS_O) $(Q_HASHTABLE_O) lib$(LIBRARY).so
	find . -name '*.o' -print | xargs rm -f

test: lib $(ACUTEST_O) $(DOC_O) $(BKTREE_O) $(ENTRY_O) $(HAMMINGINDEX_O) $(HASHTABLE_O) $(PAYLOAD_O) $(QUERY_O) $(EDITDISTBKTREE_O) $(Q_HASHTABLE_O) $(JOBS_O)
	$(CXX) $(CXXFLAGS) -o test $(ACUTEST_O) $(DOC_O) $(BKTREE_O) $(ENTRY_O) $(HAMMINGINDEX_O) $(HASHTABLE_O) $(PAYLOAD_O) $(QUERY_O) $(EDITDISTBKTREE_O) $(Q_HASHTABLE_O) $(JOBS_O) ./lib$(LIBRARY).so