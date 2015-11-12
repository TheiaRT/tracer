all: ray_tracer parser
	clang++ -std=c++11 -g pnm_image.o ray_tracer.o dist/jsoncpp.cpp main.cpp -o test

parser: 
	clang++ -std=c++11 -g dist/jsoncpp.cpp parser.cpp -o parsertest

ray_tracer: pnm_image
	clang++ -g -c ray_tracer.cpp

pnm_image:
	clang++ -g -c pnm_image.cpp
tests:  pnm_tests
	clang++ -g pnm_image.o pnm_tests.cpp -o unittest
pnm_tests: pnm_image
	clang++ -g -c pnm_tests.cpp
