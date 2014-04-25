// File: core/lock_set.cc - Implementation of lock set.

#include "core/lock_set.h"

#include <sstream>

LockSet::lock_version_t LockSet::curr_lock_version_ = 0;

bool LockSet::Match(LockSet *ls) {
  if (set_.size() != ls->set_.size())
    return false;

  for (LockVersionMap::iterator it = set_.begin(); it != set_.end(); ++it) {
    LockVersionMap::iterator mit = ls->set_.find(it->first);
    if (mit == ls->set_.end())
      return false;
  }
  return true;
}

bool LockSet::Disjoint(LockSet *ls) {
  for (LockVersionMap::iterator it = set_.begin(); it != set_.end(); ++it) {
    LockVersionMap::iterator mit = ls->set_.find(it->first);
    if (mit != ls->set_.end())
      return false;
  }
  return true;
}

bool LockSet::Disjoint(LockSet *rmt_ls1, LockSet *rmt_ls2) {
  for (LockVersionMap::iterator it = set_.begin(); it != set_.end(); ++it) {
    LockVersionMap::iterator mit1 = rmt_ls1->set_.find(it->first);
    LockVersionMap::iterator mit2 = rmt_ls2->set_.find(it->first);
    if (mit1 != rmt_ls1->set_.end() && mit2 != rmt_ls2->set_.end()) {
      // compare the version number between two remote lock sets
      if (mit1->second == mit2->second)
        return false;
    }
  }
  return true;
}

bool LockSet::DisjointWithVersion(LockSet *ls) {
  for (LockVersionMap::iterator it = set_.begin(); it != set_.end(); ++it) {
    LockVersionMap::iterator mit = ls->set_.find(it->second);
    if (mit != ls->set_.end()) {
      // compare version number
      if (mit->second == it->second)
        return false;
    }
  }
  return true;
}

std::string LockSet::ToString() {
  std::stringstream ss;
  ss << "[";
  for (LockVersionMap::iterator it = set_.begin(); it != set_.end(); ++it) {
    ss << std::hex << "0x" << it->first << " ";
  }
  ss << "]";
  return ss.str();
}

