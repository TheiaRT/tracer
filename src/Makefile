all: ray_tracer
	clang++ -g pnm_image.o ray_tracer.o main.cpp -o test

ray_tracer: pnm_image
	clang++ -g -c ray_tracer.cpp

pnm_image:
	clang++ -g -c pnm_image.cpp
