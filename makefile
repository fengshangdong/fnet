CC = g++
CXXFLAGS = -g -Wall -march=native -std=c++11 -I. -pthread
LDFLAGS = -L. -lpthread -lfnet
NET_SRC =                       \
       Channel.cpp              \
       EventLoop.cpp            \
       EventLoopThread.cpp      \
       Poller.cpp               \
       Thread.cpp               \
       ThreadPool.cpp           \
       Timestamp.cpp            \
       Timer.cpp                \
       TimerQueue.cpp           \

NET_LIB = libfnet.a
OBJS = $(patsubst %.cpp,%.o,$(NET_SRC))
TEST = \
       atomic_test              \
       counter                  \
       copyonwrite_test         \
       destruct                 \
       exitdeadlock             \
       fork_test                \
       factory                  \
       observer                 \
       observer_safe            \
       reactor_test1            \
       reactor_test2            \
       reactor_test3            \
       reactor_test4            \
       reactor_test5            \
       reactor_test6            \
       requestinventory_test    \
       requestinventory_test2   \
       timestamp_test           \
       thread_test              \
       thread_test1             \
       thread_test2             \
       threadpool_test          \

all: $(TEST)

$(TEST): $(NET_LIB)
	$(CC) $(CXXFLAGS) -o $@ $(filter %.cpp,$^) $(LDFLAGS)

$(NET_LIB): $(NET_SRC)
	$(CC) $(CXXFLAGS) -c $^
	ar rcs $@ $(OBJS)

atomic_test:                      test/Atomic_test.cpp
counter:                          test/Counter.cpp
copyonwrite_test:                 test/CopyOnWrite_test.cpp
exitdeadlock:                     test/ExitDeadLock.cpp
destruct:                         test/destruct.cpp
factory:                          test/Factory.cpp
fork_test:                        test/Fork_test.cpp
observer:                         test/Observer.cpp
observer_safe:                    test/Observer_safe.cpp
reactor_test1:                    test/Reactor_test1.cpp
reactor_test2:                    test/Reactor_test2.cpp
reactor_test3:                    test/Reactor_test3.cpp
reactor_test4:                    test/Reactor_test4.cpp
reactor_test5:                    test/Reactor_test5.cpp
reactor_test6:                    test/Reactor_test6.cpp
requestinventory_test:            test/RequestInventory_test.cpp
requestinventory_test2:           test/RequestInventory_test2.cpp
thread_test:                      test/Thread_test.cpp
thread_test1:                     test/Thread_test1.cpp
thread_test2:                     test/Thread_test2.cpp
threadpool_test:                  test/ThreadPool_test.cpp
timestamp_test:                   test/Timestamp_test.cpp


.PHONY: clean
clean:
	@-rm -rf $(TEST) $(NET_LIB) $(OBJS)
