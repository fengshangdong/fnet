CC = g++
CXXFLAGS = -I.. -g -Wall -march=native -std=c++11
LDFLAGS = -L. -lfhttp  -L.. -lfnet -lpthread

HTTP_SRC = HttpServer.cpp HttpResponse.cpp HttpContext.cpp
OBJS = $(patsubst %.cpp,%.o,$(HTTP_SRC))

HTTP_LIB = libfhttp.a
TEST = httpServer_test 

all: $(TEST)

$(TEST): $(HTTP_LIB)
	$(CC) $(CXXFLAGS) -o $@ $(filter %.cpp,$^) $(LDFLAGS) 

$(HTTP_LIB): $(HTTP_SRC)
	$(CC) $(CXXFLAGS) -c $^
	ar rcs $@ $(OBJS)

httpServer_test:     HttpServer_test.cpp

.PHONY: clean
clean:
	@-rm -rf $(TEST) $(HTTP_LIB) $(OBJS)
