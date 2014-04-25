// File: core/analyzer.cc - Analyze (profile) program behavior.

#include "core/analyzer.h"

#ifndef MAX
#define MAX(a, b) (((a)>(b)) ? (a) : (b))
#endif

AnalysisDescriptor::AnalysisDescriptor()
    : hook_before_mem_(false),
      hook_after_mem_(false),
      hook_pthread_func_(false),
      hook_malloc_func_(false),
      hook_main_func_(false),
      hook_syscall_(false),
      hook_signal_(false),
      hook_atomic_inst_(false),
      track_inst_count_(false),
      skip_stack_access_(true),
      num_passes_needed_(1), 
      hook_call_stack_(false) {
  // empty
}

void AnalysisDescriptor::Merge(AnalysisDescriptor *desc) {
  hook_before_mem_ = hook_before_mem_ || desc->hook_before_mem_;
  hook_after_mem_ = hook_after_mem_ || desc->hook_after_mem_;
  hook_pthread_func_ = hook_pthread_func_ || desc->hook_pthread_func_;
  hook_malloc_func_ = hook_malloc_func_ || desc->hook_malloc_func_;
  hook_main_func_ = hook_main_func_ || desc->hook_main_func_;
  hook_syscall_ = hook_syscall_ || desc->hook_syscall_;
  hook_signal_ = hook_signal_ || desc->hook_signal_;
  hook_atomic_inst_ = hook_atomic_inst_ || desc->hook_atomic_inst_;
  track_inst_count_ = track_inst_count_ || desc->track_inst_count_;
  skip_stack_access_ = skip_stack_access_ && desc->skip_stack_access_;
  num_passes_needed_ = MAX(num_passes_needed_, desc->num_passes_needed_);
  hook_call_stack_ = hook_call_stack_ || desc->hook_call_stack_;
}

