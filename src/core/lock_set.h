// File: core/lock_set.h - Define lock set.

#ifndef CORE_LOCK_SET_H_
#define CORE_LOCK_SET_H_

#include <map>

#include "core/basictypes.h"
#include "core/atomic.h"

// Lock set.
class LockSet {
 public:
  LockSet() {}
  ~LockSet() {}

  bool Empty() { return set_.empty(); }
  bool Match(LockSet *ls);
  void Add(address_t addr) { set_[addr] = GetNextLockVersion(); }
  void Remove(address_t addr) { set_.erase(addr); }
  void Clear() { set_.clear(); }
  bool Disjoint(LockSet *ls);
  bool Disjoint(LockSet *rmt_ls1, LockSet *rmt_ls2);
  bool DisjointWithVersion(LockSet *ls);
  std::string ToString();

 private:
  typedef uint64 lock_version_t;
  typedef std::map<address_t, lock_version_t> LockVersionMap;

  static lock_version_t GetNextLockVersion() {
    return ATOMIC_ADD_AND_FETCH(&curr_lock_version_, 1);
  }

  LockVersionMap set_;

  static lock_version_t curr_lock_version_;

  // using default copy constructor and assignment operator
};

#endif

