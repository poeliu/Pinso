// File: core/analyzer.h - Analyze (profile) program behavior.

#ifndef CORE_ANALYZER_H_
#define CORE_ANALYZER_H_

#include "core/basictypes.h"
#include "core/static_info.h"
#include "core/knob.h"

// Describe the corresponding analyzer. Define which program points
// to analyze.
class AnalysisDescriptor {
 public:
  AnalysisDescriptor();
  ~AnalysisDescriptor() {}

  void Merge(AnalysisDescriptor *desc);
  bool HookMem() { return hook_before_mem_ || hook_after_mem_; }
  bool HookBeforeMem() { return hook_before_mem_; }
  bool HookAfterMem() { return hook_after_mem_; }
  bool HookPthreadFunc() { return hook_pthread_func_; }
  bool HookMallocFunc() { return hook_malloc_func_; }
  bool HookMainFunc() { return hook_main_func_; }
  bool HookSyscall() { return hook_syscall_; }
  bool HookSignal() { return hook_signal_; }
  bool HookAtomicInst() { return hook_atomic_inst_; }
  bool TrackInstCount() { return track_inst_count_; }
  bool SkipStackAccess() { return skip_stack_access_; }
  int NumPassesNeeded() { return num_passes_needed_; }
  bool HookCallStack() {return hook_call_stack_; }

  void SetHookBeforeMem() { hook_before_mem_ = true; }
  void SetHookAfterMem() { hook_after_mem_ = true; }
  void SetHookPthreadFunc() { hook_pthread_func_ = true; }
  void SetHookMallocFunc() { hook_malloc_func_ = true; }
  void SetHookMainFunc() { hook_main_func_ = true; }
  void SetHookSyscall() { hook_syscall_ = true; }
  void SetHookSignal() { hook_signal_ = true; }
  void SetHookAtomicInst() { hook_atomic_inst_ = true; }
  void SetTrackInstCount() { track_inst_count_ = true; }
  void SetNoSkipStackAccess() { skip_stack_access_ = false; }
  void SetNumPassesNeeded(int num) { num_passes_needed_ = num; }
  void SetHookCallStack() {hook_call_stack_ = true; }

 private:
  bool hook_before_mem_;
  bool hook_after_mem_;
  bool hook_pthread_func_;
  bool hook_malloc_func_;
  bool hook_main_func_;
  bool hook_syscall_;
  bool hook_signal_;
  bool hook_atomic_inst_;
  bool track_inst_count_;
  bool skip_stack_access_;
  int num_passes_needed_; // only useful in offline analysis
  bool hook_call_stack_;

  DISALLOW_COPY_CONSTRUCTORS(AnalysisDescriptor);
};

// An analyzer is used to profile program bahavior.
// For example: an PSet analyzer which builds predecessor set (PSet).
class Analyzer {
 public:
  explicit Analyzer(Knob *knob) : knob_(knob) {}
  virtual ~Analyzer() {}

  virtual void Register() {}
  virtual bool Enabled() { return false; }
  virtual void ProgramStart() {}
  virtual void ProgramExit() {}
  virtual void ImageLoad(Image *image, address_t low_addr,
                         address_t high_addr, address_t data_start,
                         size_t data_size, address_t bss_start,
                         size_t bss_size) {}
  virtual void ImageUnload(Image *image, address_t low_addr,
                           address_t high_addr, address_t data_start,
                           size_t data_size, address_t bss_start,
                           size_t bss_size) {}
  virtual void SyscallEntry(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                            int syscall_num) {}
  virtual void SyscallExit(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                           int syscall_num) {}
  virtual void SignalReceived(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                              int signal_num) {}
  virtual void ThreadStart(thread_id_t curr_thd_id,
                           thread_id_t parent_thd_id) {}
  virtual void ThreadExit(thread_id_t curr_thd_id, timestamp_t curr_thd_clk) {}
  virtual void Main(thread_id_t curr_thd_id, timestamp_t curr_thd_clk) {}
  virtual void ThreadMain(thread_id_t curr_thd_id, timestamp_t curr_thd_clk) {}
  virtual void BeforeMemRead(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                             Inst *inst, address_t addr, size_t size) {}
  virtual void AfterMemRead(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                            Inst *inst, address_t addr, size_t size) {}
  virtual void BeforeMemWrite(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                              Inst *inst, address_t addr, size_t size) {}
  virtual void AfterMemWrite(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                             Inst *inst, address_t addr, size_t size) {}
  virtual void BeforeAtomicInst(thread_id_t curr_thd_id,
                                timestamp_t curr_thd_clk, Inst *inst,
                                std::string type, address_t addr) {}
  virtual void AfterAtomicInst(thread_id_t curr_thd_id,
                               timestamp_t curr_thd_clk, Inst *inst,
                               std::string type, address_t addr) {}
  virtual void BeforePthreadCreate(thread_id_t curr_thd_id,
                                   timestamp_t curr_thd_clk, Inst *inst) {}
  virtual void AfterPthreadCreate(thread_id_t curr_thd_id,
                                  timestamp_t curr_thd_clk, Inst *inst,
                                  thread_id_t child_thd_id) {}
  virtual void BeforePthreadJoin(thread_id_t curr_thd_id,
                                 timestamp_t curr_thd_clk, Inst *inst,
                                 thread_id_t child_thd_id) {}
  virtual void AfterPthreadJoin(thread_id_t curr_thd_id,
                                timestamp_t curr_thd_clk,Inst *inst,
                                thread_id_t child_thd_id) {}
  virtual void BeforePthreadMutexTryLock(thread_id_t curr_thd_id,
                                         timestamp_t curr_thd_clk, Inst *inst,
                                         address_t addr) {}
  virtual void AfterPthreadMutexTryLock(thread_id_t curr_thd_id,
                                        timestamp_t curr_thd_clk, Inst *inst,
                                        address_t addr, int ret_val) {}
  virtual void BeforePthreadMutexLock(thread_id_t curr_thd_id,
                                      timestamp_t curr_thd_clk, Inst *inst,
                                      address_t addr) {}
  virtual void AfterPthreadMutexLock(thread_id_t curr_thd_id,
                                     timestamp_t curr_thd_clk, Inst *inst,
                                     address_t addr) {}
  virtual void BeforePthreadMutexUnlock(thread_id_t curr_thd_id,
                                        timestamp_t curr_thd_clk, Inst *inst,
                                        address_t addr) {}
  virtual void AfterPthreadMutexUnlock(thread_id_t curr_thd_id,
                                       timestamp_t curr_thd_clk, Inst *inst,
                                       address_t addr) {}
  virtual void BeforePthreadCondSignal(thread_id_t curr_thd_id,
                                       timestamp_t curr_thd_clk, Inst *inst,
                                       address_t addr) {}
  virtual void AfterPthreadCondSignal(thread_id_t curr_thd_id,
                                      timestamp_t curr_thd_clk, Inst *inst,
                                      address_t addr) {}
  virtual void BeforePthreadCondBroadcast(thread_id_t curr_thd_id,
                                          timestamp_t curr_thd_clk, Inst *inst,
                                          address_t addr) {}
  virtual void AfterPthreadCondBroadcast(thread_id_t curr_thd_id,
                                         timestamp_t curr_thd_clk, Inst *inst,
                                         address_t addr) {}
  virtual void BeforePthreadCondWait(thread_id_t curr_thd_id,
                                     timestamp_t curr_thd_clk, Inst *inst,
                                     address_t cond_addr,
                                     address_t mutex_addr) {}
  virtual void AfterPthreadCondWait(thread_id_t curr_thd_id,
                                    timestamp_t curr_thd_clk, Inst *inst,
                                    address_t cond_addr,
                                    address_t mutex_addr) {}
  virtual void BeforePthreadCondTimedwait(thread_id_t curr_thd_id,
                                          timestamp_t curr_thd_clk, Inst *inst,
                                          address_t cond_addr,
                                          address_t mutex_addr) {}
  virtual void AfterPthreadCondTimedwait(thread_id_t curr_thd_id,
                                         timestamp_t curr_thd_clk, Inst *inst,
                                         address_t cond_addr,
                                         address_t mutex_addr) {}
  virtual void BeforePthreadBarrierInit(thread_id_t curr_thd_id,
                                        timestamp_t curr_thd_clk, Inst *inst,
                                        address_t addr, unsigned int count) {}
  virtual void AfterPthreadBarrierInit(thread_id_t curr_thd_id,
                                       timestamp_t curr_thd_clk, Inst *inst,
                                       address_t addr, unsigned int count) {}
  virtual void BeforePthreadBarrierWait(thread_id_t curr_thd_id,
                                        timestamp_t curr_thd_clk, Inst *inst,
                                        address_t addr) {}
  virtual void AfterPthreadBarrierWait(thread_id_t curr_thd_id,
                                       timestamp_t curr_thd_clk, Inst *inst,
                                       address_t addr) {}
  virtual void BeforeMalloc(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                            Inst *inst, size_t size) {}
  virtual void AfterMalloc(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                           Inst *inst, size_t size, address_t addr) {}
  virtual void BeforeCalloc(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                            Inst *inst, size_t nmemb, size_t size) {}
  virtual void AfterCalloc(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                           Inst *inst, size_t nmemb, size_t size,
                           address_t addr) {}
  virtual void BeforeRealloc(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                             Inst *inst, address_t ori_addr, size_t size) {}
  virtual void AfterRealloc(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                            Inst *inst, address_t ori_addr, size_t size,
                            address_t new_addr) {}
  virtual void BeforeFree(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                          Inst *inst, address_t addr) {}
  virtual void AfterFree(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                         Inst *inst, address_t addr) {}
  virtual void BeforeValloc(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                            Inst *inst, size_t size) {}
  virtual void AfterValloc(thread_id_t curr_thd_id, timestamp_t curr_thd_clk,
                           Inst *inst, size_t size, address_t addr) {}

  AnalysisDescriptor *desc() { return &desc_; }

 protected:
  Knob *knob_;
  AnalysisDescriptor desc_;

 private:
  DISALLOW_COPY_CONSTRUCTORS(Analyzer);
};

#endif

