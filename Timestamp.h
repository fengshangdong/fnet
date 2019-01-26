#ifndef BASE_TIMESTAMP_H
#define BASE_TIMESTAMP_H

#include <stdint.h>
#include <string>

namespace fnet
{

class Timestamp
{
 public:
  Timestamp();

  explicit Timestamp(int64_t microSecondsSinceEpoch);

  std::string toString() const;
  std::string toFormattedString() const;

  static Timestamp now();

  static Timestamp invalid();

	int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  int64_t microSecondsSinceEpoch_;
};

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
  int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

}
#endif
