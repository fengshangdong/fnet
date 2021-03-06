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

  void swap(Timestamp& that)
  {
    std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
  }

  static Timestamp now();

  bool valid() const { return microSecondsSinceEpoch_ > 0; }
  static Timestamp invalid();

  int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
  static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
  int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
  int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

}
#endif
