CXX = clang++
CFLAGS = -g -std=c++11

all: ray_tracer 
	$(CXX) $(CFLAGS) dist/jsoncpp.cpp pnm_image.o ray_tracer.o sphere.o point_light.o parser.cpp main.cpp -o test

parser: 
	$(CXX) $(CFLAGS) -g dist/jsoncpp.cpp point_light.cpp sphere.cpp parser.cpp parsermain.cpp -o parsertest
ray_tracer: pnm_image
	$(CXX) $(CFLAGS) -c ray_tracer.cpp sphere.cpp point_light.cpp

pnm_image:
	$(CXX) $(CFLAGS) -c pnm_image.cpp
tests:  pnm_tests
	$(CXX) $(CFLAGS) pnm_image.o pnm_tests.cpp -o unittest
pnm_tests: pnm_image
	$(CXX) $(CFLAGS) -c pnm_tests.cpp
