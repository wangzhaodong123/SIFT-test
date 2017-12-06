INCLUDE_OPENCV = $(shell pkg-config --cflags opencv)
LIBDIR_OPENCV = $(shell pkg-config --libs opencv)

all:sift_test

sift_test:sift_test.o
	g++ -std=c++11 -o sift_test sift_test.o $(LIBDIR_OPENCV) -L /usr/local/lib/libopencv_nonfree.so.2.4
sift_test.o:sift_test.cpp
	g++ -std=c++11 -c sift_test.cpp $(INCLUDE_IPENCV)
clean:
	rm -f *.o sift_test