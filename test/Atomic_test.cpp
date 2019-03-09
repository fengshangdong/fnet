#include "../Atomic.h"
#include <assert.h>

int main()
{
  fnet::Atomic a0;
  assert(a0.get() == 0);
  a0.increment();
  assert(a0.get() == 1);
  a0.increment();
  assert(a0.get() == 2);
  a0.decrement();
  assert(a0.get() == 1);
}
