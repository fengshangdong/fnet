#include "../Timestamp.h"
#include <vector>
#include <stdio.h>

using fnet::Timestamp;

int main()
{
  Timestamp now(Timestamp::now());
  printf("%s\n", now.toString().c_str());
  printf("%s\n", now.toFormattedString().c_str());

  now = addTime(now, 3600);
  printf("%s\n", now.toString().c_str());
  printf("%s\n", now.toFormattedString().c_str());
}

