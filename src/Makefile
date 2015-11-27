CXX = clang++
CFLAGS = -g -std=c++11

all: test
	./test -s scene.json -o output.pnm; open output.pnm


test: ray_tracer.o parser.o jsoncpp.o main.o
	$(CXX) $(CFLAGS) pnm_image.o jsoncpp.o parser.o ray_tracer.o sphere.o point_light.o main.o -o test

main.o: main.cpp
	$(CXX) $(CFLAGS) -g -c  main.cpp 

parser.o: jsoncpp.o parser.cpp
	$(CXX) $(CFLAGS) -g -c  parser.cpp 

jsoncpp.o:
	$(CXX) $(CFLAGS) -g -c dist/jsoncpp.cpp 

ray_tracer.o: pnm_image.o ray_tracer.cpp point_light.cpp sphere.cpp
	$(CXX) $(CFLAGS) -c ray_tracer.cpp sphere.cpp point_light.cpp

pnm_image.o: pnm_image.cpp
	$(CXX) $(CFLAGS) -c pnm_image.cpp
tests:  pnm_tests
	$(CXX) $(CFLAGS) pnm_image.o pnm_tests.cpp -o unittest
pnm_tests: pnm_image.o
	$(CXX) $(CFLAGS) -c pnm_tests.cpp
