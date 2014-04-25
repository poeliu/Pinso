// File: core/unistd_wrapper.hpp - Define unistd function wrappers.
//
// Here are the prototypes of some the unistd functions on my machine
// (x86_64 RHEL5 with glibc-2.5): (* means implemented)
//
// * unsigned int sleep (unsigned int __seconds);
//   int usleep (__useconds_t __useconds);

#ifndef CORE_UNISTD_WRAPPER_HPP_
#define CORE_UNISTD_WRAPPER_HPP_

#include "pin.H"

#include "core/basictypes.h"
#include "core/wrapper.hpp"

// Wrapper context for "sleep". The original function prototype is:
//
// unsigned int sleep (unsigned int seconds);
//
class UnistdSleepContext : public WrapperContext {
 public:
  UnistdSleepContext(THREADID tid, CONTEXT *ctxt, ADDRINT ret_addr,
                     unsigned int seconds)
      : WrapperContext(tid, ctxt, ret_addr),
        seconds_(seconds) {}
  ~UnistdSleepContext() {}

  unsigned int ret_val() { return ret_val_; }
  unsigned int seconds() { return seconds_; }
  void set_ret_val(unsigned int ret_val) { ret_val_ = ret_val; }

 private:
  unsigned int ret_val_;
  unsigned int seconds_;

  DISALLOW_COPY_CONSTRUCTORS(UnistdSleepContext);
};

// Wrapper for "sleep".
class UnistdSleepWrapper
    : public Wrapper<UnistdSleepContext, UnistdSleepWrapper> {
 public:
  static void Replace(IMG img, HandlerType handler);
  static void CallOriginal(UnistdSleepContext *context);

 private:
  UnistdSleepWrapper() {}
  ~UnistdSleepWrapper() {}

  static unsigned int __Wrapper(THREADID tid, CONTEXT *ctxt, ADDRINT ret_addr,
                                unsigned int seconds);

  DISALLOW_COPY_CONSTRUCTORS(UnistdSleepWrapper);
};

// Wrapper context for "usleep". The original function prototype is:
//
// int usleep (useconds_t useconds);
//
class UnistdUsleepContext : public WrapperContext {
 public:
  UnistdUsleepContext(THREADID tid, CONTEXT *ctxt, ADDRINT ret_addr,
                      useconds_t useconds)
      : WrapperContext(tid, ctxt, ret_addr),
        useconds_(useconds) {}
  ~UnistdUsleepContext() {}

  int ret_val() { return ret_val_; }
  useconds_t useconds() { return useconds_; }
  void set_ret_val(int ret_val) { ret_val_ = ret_val; }

 private:
  int ret_val_;
  useconds_t useconds_;

  DISALLOW_COPY_CONSTRUCTORS(UnistdUsleepContext);
};

// Wrapper for "usleep".
class UnistdUsleepWrapper
    : public Wrapper<UnistdUsleepContext, UnistdUsleepWrapper> {
 public:
  static void Replace(IMG img, HandlerType handler);
  static void CallOriginal(UnistdUsleepContext *context);

 private:
  UnistdUsleepWrapper() {}
  ~UnistdUsleepWrapper() {}

  static int __Wrapper(THREADID tid, CONTEXT *ctxt, ADDRINT ret_addr,
                       useconds_t useconds);

  DISALLOW_COPY_CONSTRUCTORS(UnistdUsleepWrapper);
};

// Global functions to install unistd wrappers
void register_unistd_wrappers(IMG img);

#endif

