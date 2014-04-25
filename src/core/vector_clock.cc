// File: core/vector_clock.cc - Implementation of vector clock.

#include "core/vector_clock.h"

#include <sstream>

#ifndef MAX
#define MAX(a, b) (((a)>(b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a)<(b)) ? (a) : (b))
#endif

bool VectorClock::HappensBefore(VectorClock *vc) {
  ThreadClockMap::iterator curr_it = map_.begin();
  ThreadClockMap::iterator vc_it = vc->map_.begin();
  for (; curr_it != map_.end(); ++curr_it) {
    thread_id_t curr_thd_id = curr_it->first;
    timestamp_t curr_thd_clk = curr_it->second;

    bool valid = false;
    for (; vc_it != vc->map_.end(); ++vc_it) {
      thread_id_t vc_thd_id = vc_it->first;
      timestamp_t vc_thd_clk = vc_it->second;

      if (vc_thd_id == curr_thd_id) {
        if (vc_thd_clk >= curr_thd_clk) {
          valid = true;
          ++vc_it;
        }
        break;
      } else if (vc_thd_id > curr_thd_id) {
        break;
      }
    }

    if (!valid)
      return false;
  }

  return true;
}

bool VectorClock::HappensAfter(VectorClock *vc) {
  ThreadClockMap::iterator curr_it = map_.begin();
  ThreadClockMap::iterator vc_it = vc->map_.begin();
  for (; vc_it != vc->map_.end(); ++vc_it) {
    thread_id_t vc_thd_id = vc_it->first;
    timestamp_t vc_thd_clk = vc_it->second;

    bool valid = false;
    for (; curr_it != map_.end(); ++curr_it) {
      thread_id_t curr_thd_id = curr_it->first;
      timestamp_t curr_thd_clk = curr_it->second;

      if (curr_thd_id == vc_thd_id) {
        if (curr_thd_clk >= vc_thd_clk) {
          valid = true;
          ++curr_it;
        }
        break;
      } else if (curr_thd_id > vc_thd_id) {
        break;
      }
    }

    if (!valid)
      return false;
  }

  return true;
}

void VectorClock::Join(VectorClock *vc) {
  for (ThreadClockMap::iterator it = vc->map_.begin();
       it != vc->map_.end(); ++it) {
    ThreadClockMap::iterator mit = map_.find(it->first);
    if (mit == map_.end()) {
      map_[it->first] = it->second;
    } else {
      mit->second = MAX(it->second, mit->second);
    }
  }
}

void VectorClock::Increment(thread_id_t thread_id) {
  ThreadClockMap::iterator it = map_.find(thread_id);
  if (it == map_.end()) {
    map_[thread_id] = 1;
  } else {
    it->second++;
  }
}

bool VectorClock::Equal(VectorClock *vc) {
  if (map_.size() != vc->map_.size())
    return false;

  ThreadClockMap::iterator curr_it = map_.begin();
  ThreadClockMap::iterator vc_it = vc->map_.begin();
  for (; curr_it != map_.end() && vc_it != vc->map_.end(); ++curr_it, ++vc_it) {
    if (curr_it->first != vc_it->first || curr_it->second != vc_it->second) {
      return false;
    }
  }

  return true;
}

std::string VectorClock::ToString() {
  std::stringstream ss;
  ss << "[";
  for (ThreadClockMap::iterator it = map_.begin(); it != map_.end(); ++it) {
    ss << "T" << std::hex << it->first << ":" << std::dec << it->second << " ";
  }
  ss << "]";
  return ss.str();
}

