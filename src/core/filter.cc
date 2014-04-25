// File: core/filter.cc - Define address filters.

#include "core/filter.h"

void RegionFilter::AddRegion(address_t addr, size_t size, bool locking) {
  ScopedLock locker(internal_lock_, locking);

  addr_region_map_[addr] = size;
}

size_t RegionFilter::RemoveRegion(address_t addr, bool locking) {
  ScopedLock locker(internal_lock_, locking);

  if (!addr) return 0;
  size_t size = 0;
  std::map<address_t, size_t>::iterator it = addr_region_map_.find(addr);
  if (it != addr_region_map_.end()) {
    size = it->second;
    addr_region_map_.erase(it);
  }
  return size;
}

bool RegionFilter::Filter(address_t addr, bool locking) {
  ScopedLock locker(internal_lock_, locking);

  if (addr_region_map_.begin() == addr_region_map_.end())
    return true;

  std::map<address_t,size_t>::iterator it = addr_region_map_.upper_bound(addr);
  if (it == addr_region_map_.begin())
    return true;

  it--;
  address_t region_start = it->first;
  size_t region_size = it->second;
  if (addr >= region_start && addr < region_start + region_size)
    return false;
  else
    return true;
}

