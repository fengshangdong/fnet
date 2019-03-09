#include "../Mutex.h"
#include <string>

using std::string;

fnet::MutexLock g_mutex;
string g_str = "hello";
int32_t g_int32 = 123;
int64_t g_int64 = 4312;

string getString()
{
  fnet::MutexLockGuard lock(g_mutex);
  return g_str;
}

int32_t getInt32()
{
  fnet::MutexLockGuard lock(g_mutex);
  return g_int32;
}

int64_t getInt64()
{
  fnet::MutexLockGuard lock(g_mutex);
  return g_int64;
}

int main()
{
  getString();
  getInt32();
  getInt64();
}
