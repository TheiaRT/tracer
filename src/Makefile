CXX = clang++
CFLAGS = -g -std=c++11

all: ray_tracer
	$(CXX) $(CFLAGS) pnm_image.o ray_tracer.o main.cpp -o test

ray_tracer: pnm_image
	$(CXX) $(CFLAGS) -c ray_tracer.cpp

pnm_image:
	$(CXX) $(CFLAGS) -c pnm_image.cpp
tests:  pnm_tests
	$(CXX) $(CFLAGS) pnm_image.o pnm_tests.cpp -o unittest
pnm_tests: pnm_image
	$(CXX) $(CFLAGS) -c pnm_tests.cpp
