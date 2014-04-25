// File: idiom/scheduler_keep.hpp - Define the active scheduler controller.

#ifndef IDIOM_SCHEDULER_KEEP_HPP_
#define IDIOM_SCHEDULER_KEEP_HPP_

#include "idiom/scheduler_common.hpp"
#include "delta/observer.h"
#include "ilist.h"
#include "slist.h"

namespace delta {

// The controller for the idiom driven active scheduler.
class SchedulerKeep : public idiom::SchedulerCommon {
 public:
  SchedulerKeep();
  ~SchedulerKeep() {}

 protected:
  void HandlePreSetup();
  void HandlePostSetup();
  void HandleProgramExit();

  // functions to override
  void Choose();
  void TestSuccess();
  void TestFail();
  bool UseDecreasingPriorities();
  bool YieldWithDelay();

  Observer *observer_;
  iList *ilist_; 
  SuspectList *slist_;

 private:
  DISALLOW_COPY_CONSTRUCTORS(SchedulerKeep);
};

} // namespace delta

#endif
