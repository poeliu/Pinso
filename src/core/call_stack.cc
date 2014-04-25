// File: pset/pset.cc - Implementation of PSet and PSet database.

#include "core/static_info.h"
#include "core/logging.h"
#include "core/call_stack.h"

bool CallStack::IsExistInStack(address_t target_pc) {
  for(CallerVector::iterator it = _call_stack.begin();
      it != _call_stack.end(); ++it) {
    if (target_pc == it->second)
      return true;
  }
  return false;
}

void CallStack::AdjustStack(address_t target_pc) {
   if (IsExistInStack(target_pc)) {
     Caller caller_ = _call_stack.back();
     while (target_pc != caller_.second) {
       _call_stack.pop_back();   
       caller_ = _call_stack.back(); 
     }
     if (target_pc == caller_.second)
       _call_stack.pop_back();
   }
 }

void CallStack::ProcessCall(Inst *inst, address_t return_pc) {
  Caller caller_;
  caller_ = std::pair<Inst *, address_t>(inst, return_pc);
  _call_stack.push_back(caller_);
  //DumpStack();
}

void CallStack::ProcessReturn(address_t target_pc) {
  AdjustStack(target_pc);
  //DumpStack();
}

std::vector<address_t> CallStack::GetCallerStack(int level) {
   int _vector_depth = Depth();
   std::vector<address_t> _path_vector;
   CallerVector::reverse_iterator it = _call_stack.rbegin();
   if (level > _vector_depth) {
     for(int i = 0; i < _vector_depth; ++i) {
         address_t c_inst = it->first->id();
         _path_vector.push_back(c_inst);
         ++it;
     }
   } else {
       for (int i = 0; i < level; ++i) {
         address_t c_inst = it->first->id();
         _path_vector.push_back(c_inst);
         ++it;
      } 
   }
   return _path_vector;
 }

void CallStack::DumpStack() {
  for(CallerVector::iterator it = _call_stack.begin();
      it != _call_stack.end(); ++it) {
    DEBUG_FMT_PRINT_SAFE("%x ", it->first->id());
  } 
  DEBUG_FMT_PRINT_SAFE("\n\n");
}

void CallStackInfo::CreateCallStack(thread_id_t thread_id) {
  CallStack *call_stack_ = new CallStack();
  call_stack_map_[thread_id] = call_stack_;
}

void CallStackInfo::RemoveCallStack(thread_id_t thread_id) {
  delete call_stack_map_[thread_id];
  call_stack_map_.erase(thread_id);
}

CallStack *CallStackInfo::GetCallStack(thread_id_t thread_id) {
  CallStackMap::iterator it = call_stack_map_.find(thread_id);
  if (it == call_stack_map_.end())
    return NULL;
  else
    return it->second; 
}
