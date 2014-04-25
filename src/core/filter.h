// File: core/filter.h - Define address filters.

#ifndef CORE_FILTER_H_
#define CORE_FILTER_H_

#include <map>

#include "core/basictypes.h"
#include "core/sync.h"

class RegionFilter {
 public:
  explicit RegionFilter(Mutex *lock) : internal_lock_(lock) {}
  ~RegionFilter() { delete internal_lock_; }

  void AddRegion(address_t addr, size_t size) { AddRegion(addr, size, true); }
  size_t RemoveRegion(address_t addr) { return RemoveRegion(addr, true); }
  bool Filter(address_t addr) { return Filter(addr, true); }

  void AddRegion(address_t addr, size_t size, bool locking);
  size_t RemoveRegion(address_t addr, bool locking);
  bool Filter(address_t addr, bool locking);

 private:
  Mutex *internal_lock_;
  std::map<address_t, size_t> addr_region_map_;

  DISALLOW_COPY_CONSTRUCTORS(RegionFilter);
};

#endif

