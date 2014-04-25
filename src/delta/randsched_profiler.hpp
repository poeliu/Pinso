// File: delta/randsched_profiler.hpp - Define PSet profile controller.

#ifndef DELTA_RANDSCHED_PROFILER_HPP_
#define DELTA_RANDSCHED_PROFILER_HPP_

#include "core/basictypes.h"
#include "randsched/scheduler.hpp"
#include "delta/observer.h"
#include "idiom/iroot.h"
#include "ilist.h"

namespace delta {

class RandSchedProfiler : public randsched::Scheduler {

 public:
  RandSchedProfiler() : observer_(NULL), iroot_db_(NULL), ilist_(NULL){}
  ~RandSchedProfiler(){}

 private:
  void HandlePreSetup();
  void HandlePostSetup();
  //bool HandleIgnoreMemAccess(IMG img);
  void HandleProgramExit();

  Observer *observer_;
  idiom::iRootDB *iroot_db_;
  iList *ilist_;
  
  DISALLOW_COPY_CONSTRUCTORS(RandSchedProfiler);
};

} // namespace delta

#endif
