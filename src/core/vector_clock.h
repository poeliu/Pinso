// File: core/vector_clock.h - Define vector clock.

#ifndef CORE_VECTOR_CLOCK_H_
#define CORE_VECTOR_CLOCK_H_

#include <map>

#include "core/basictypes.h"

// Vector clock.
class VectorClock {
 public:
  VectorClock() {}
  ~VectorClock() {}

  bool HappensBefore(VectorClock *vc);
  bool HappensAfter(VectorClock *vc);
  void Join(VectorClock *vc);
  void Increment(thread_id_t thread_id);
  bool Equal(VectorClock *vc);
  std::string ToString();

 private:
  typedef std::map<thread_id_t, timestamp_t> ThreadClockMap;

  ThreadClockMap map_;

  // using default copy constructor and assignment operator
};

#endif

