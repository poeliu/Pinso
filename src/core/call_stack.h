// File: calltracer/cstack.h - define callstack and activation database.  

#ifndef CORE_CALL_STACK_H_
#define CORE_CALL_STACK_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "core/basictypes.h"
#include "core/sync.h"

class Inst;
class Image;
class StaticInfo;

class CallStack {
 public:
  CallStack() {}
  ~CallStack() {}

  address_t Depth() {return _call_stack.size();};
  bool IsExistInStack(address_t target_pc);
  void AdjustStack(address_t target_pc);
  void ProcessCall(Inst *inst, address_t return_pc); 
  void ProcessReturn(address_t target_pc);
  std::vector<address_t> GetCallerStack(int level);
  void DumpStack();

 private:
  Mutex *internal_lock_;
  typedef std::pair<Inst *, address_t> Caller;
  typedef std::vector<Caller> CallerVector;

  CallerVector _call_stack;

 friend class CallStackInfo;

  DISALLOW_COPY_CONSTRUCTORS(CallStack);
};

class CallStackInfo {
 public:
  CallStackInfo() {}
  ~CallStackInfo() {}

  void CreateCallStack(thread_id_t thread_id);
  void RemoveCallStack(thread_id_t thread_id);
  CallStack *GetCallStack(thread_id_t thread_id);

 private:
  typedef std::map<thread_id_t, CallStack *> CallStackMap;
  
  CallStackMap call_stack_map_;

  DISALLOW_COPY_CONSTRUCTORS(CallStackInfo);
};

#endif
