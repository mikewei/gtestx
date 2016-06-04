all: gtestx_test

# change to your gtest source directory
GTEST_SRC_DIR = /usr/src/gtest

gtestx_test: gtestx_test.cc ../src/gtestx.cc $(GTEST_SRC_DIR)/src/gtest-all.cc
	g++ -Wall -std=c++0x -I../include -I$(GTEST_SRC_DIR)/include -I$(GTEST_SRC_DIR) -o $@ $^ -pthread -lgflags

clean:
	rm -f gtestx_test

.PHONY: all clean
