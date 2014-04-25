// File: core/debug_analyzer.cc - The analyzer for debug purpose.

#include "core/debug_analyzer.h"

void DebugAnalyzer::Register() {
  knob_->RegisterBool("enable_debug", "Whether enable debug analyzer", "0");
  knob_->RegisterBool("debug_mem", "Whether debug mem accesses", "0");
  knob_->RegisterBool("debug_atomic", "Whether debug atomic inst", "0");
  knob_->RegisterBool("debug_main", "Whether debug main functions", "1");
  knob_->RegisterBool("debug_pthread", "Whether debug pthread functions", "1");
  knob_->RegisterBool("debug_malloc", "Whether debug malloc functions", "1");
  knob_->RegisterBool("debug_syscall", "Whether debug system calls", "1");
  knob_->RegisterBool("debug_track_clk", "Whether track per thread clock", "1");
}

bool DebugAnalyzer::Enabled() {
  return knob_->ValueBool("enable_debug");
}

void DebugAnalyzer::Setup() {
  if (knob_->ValueBool("debug_mem"))
    desc_.SetHookBeforeMem();
  if (knob_->ValueBool("debug_atomic"))
    desc_.SetHookAtomicInst();
  if (knob_->ValueBool("debug_main"))
    desc_.SetHookMainFunc();
  if (knob_->ValueBool("debug_pthread"))
    desc_.SetHookPthreadFunc();
  if (knob_->ValueBool("debug_malloc"))
    desc_.SetHookMallocFunc();
  if (knob_->ValueBool("debug_syscall"))
    desc_.SetHookSyscall();
  if (knob_->ValueBool("debug_track_clk"))
    desc_.SetTrackInstCount();
}

