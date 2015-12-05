CXX = clang++
CFLAGS = -g -std=c++11  # -Wall # -Wpedantic # -Werror
LFLAGS = -lpthread

all: ray_tracer.o parser.o jsoncpp.o worker.o collector.o main.o
	$(CXX) $(CFLAGS) pnm_image.o jsoncpp.o parser.o ray_tracer.o sphere.o point_light.o main.o -o test
clean:
	rm -f *.o

spheres:
	./test -s scene.json -o output.pnm; open output.pnm
snowman: test
	./test -s snowman.json -o snowman.pnm; open snowman.pnm

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

parser.o: parser.cpp jsoncpp.o parser.cpp
	$(CXX) $(CFLAGS) -c parser.cpp

worker.o: worker.cpp pnm_image.o tcp_client.o ray_tracer.o
	$(CXX) $(CFLAGS) -c worker.cpp

collector.o: collector.cpp pnm_image.o work_queue.o tcp_server.o jsoncpp.o
	$(CXX) $(CFLAGS) -c collector.cpp

jsoncpp.o:
	$(CXX) $(CFLAGS) -c dist/jsoncpp.cpp

ray_tracer.o: pnm_image.o ray_tracer.cpp point_light.cpp sphere.cpp material.h scene_object.h sphere.h
	$(CXX) $(CFLAGS) -c ray_tracer.cpp sphere.cpp point_light.cpp
pnm_image.o: pnm_image.cpp
	$(CXX) $(CFLAGS) -c pnm_image.cpp
tests:  pnm_tests
	$(CXX) $(CFLAGS) pnm_image.o pnm_tests.cpp -o unittest
pnm_tests: pnm_image.o
	$(CXX) $(CFLAGS) -c pnm_tests.cpp
worker_test: collectormain.cpp jsoncpp.o collector.o worker.o work_queue.o tcp_client.o tcp_server.o pnm_image.o parser.o ray_tracer.o misc.o
	$(CXX) $(CFLAGS) jsoncpp.o work_queue.o tcp_client.o tcp_server.o pnm_image.o collector.o worker.o parser.o ray_tracer.o point_light.o sphere.o misc.o workermain.cpp -o worker_test
collector_test: collectormain.cpp jsoncpp.o collector.o worker.o work_queue.o tcp_client.o tcp_server.o pnm_image.o parser.o ray_tracer.o misc.o
	$(CXX) $(CFLAGS) jsoncpp.o work_queue.o tcp_client.o tcp_server.o pnm_image.o collector.o worker.o parser.o ray_tracer.o point_light.o sphere.o misc.o collectormain.cpp -o collector_test
