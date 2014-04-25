// File: core/pin_sync.hpp - Define synchronizations that can be used in
// PIN tools.

#ifndef CORE_PIN_SYNC_HPP_
#define CORE_PIN_SYNC_HPP_

#include "pin.H"

#include "core/basictypes.h"
#include "core/sync.h"

// Define PIN mutex.
class PinMutex : public Mutex {
 public:
  PinMutex() { InitLock(&lock_); }
  ~PinMutex() {}

  void Lock() { GetLock(&lock_, 1); }
  void Unlock() { ReleaseLock(&lock_); }
  Mutex *Clone() { return new PinMutex; }

 protected:
  PIN_LOCK lock_;

 private:
  DISALLOW_COPY_CONSTRUCTORS(PinMutex);
};

#endif

