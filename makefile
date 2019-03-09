CC = g++
CXXFLAGS = -g -Wall -march=native -std=c++11 -I. -pthread
LDFLAGS = -L. -lpthread -lfnet
NET_SRC = Thread.cpp ThreadPool.cpp EventLoop.cpp Timestamp.cpp Channel.cpp Poller.cpp
NET_LIB = libfnet.a
OBJS = $(patsubst %.cpp,%.o,$(NET_SRC))
TEST = \
         test1           \
         test2           \
         test3           \
#				 timestamp_test          \
				 threadpool_test \
				 requestinventory_test   \
				 requestinventory_test2  \
				 observer_safe   \
				 factory         \
				 exitdeadlock    \
				 destruct        \
				 counter         \
				 atomic_test     \
				 fork_test       \
				 observer        \
				 thread_test     \
				 copyonwrite_test\

all: $(TEST)

$(TEST): $(NET_LIB)
	$(CC) $(CXXFLAGS) -o $@ $(filter %.cpp,$^) $(LDFLAGS)

$(NET_LIB): $(NET_SRC)
	$(CC) $(CXXFLAGS) -c $^
	ar rcs $@ $(OBJS)

atomic_test:							test/Atomic_test.cpp
fork_test:								test/Fork_test.cpp
observer:									test/Observer.cpp
thread_test:							test/Thread_test.cpp
copyonwrite_test:					test/CopyOnWrite_test.cpp
counter:									test/Counter.cpp
destruct:									test/destruct.cpp
exitdeadlock:							test/ExitDeadLock.cpp
factory:									test/Factory.cpp
observer_safe:						test/Observer_safe.cpp
requestinventory_test:		test/RequestInventory_test.cpp
requestinventory_test2:		test/RequestInventory_test2.cpp
threadpool_test:					test/ThreadPool_test.cpp
timestamp_test:           test/Timestamp_test.cpp
test1:										test/test1.cpp
test2:										test/test2.cpp
test3:										test/test3.cpp


.PHONY: clean
clean:
	@-rm -rf $(TEST) $(NET_LIB) $(OBJS)
