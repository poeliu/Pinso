// File: delta/profiler.hpp - Define PSet profile controller.

#ifndef DELTA_PROFILER_HPP_
#define DELTA_PROFILER_HPP_

#include "core/basictypes.h"
#include "core/execution_control.hpp"
#include "delta/observer.h"
#include "idiom/iroot.h"
#include "ilist.h"

namespace delta {

class Profiler : public ExecutionControl {

 public:
  Profiler() : observer_(NULL), iroot_db_(NULL), ilist_(NULL){}
  ~Profiler(){}

 private:
  void HandlePreSetup();
  void HandlePostSetup();
//  bool HandleIgnoreMemAccess(IMG img);
  void HandleProgramExit();

  Observer *observer_;
  idiom::iRootDB *iroot_db_;
  iList *ilist_;
  
  DISALLOW_COPY_CONSTRUCTORS(Profiler);
};

} // namespace delta

#endif
