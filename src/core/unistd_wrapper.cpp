// File: core/unistd_wrapper.cpp - Implementation of unistd function
// wrappers.

#include "core/unistd_wrapper.hpp"

#ifndef _USING_CONST_CONTEXT
#define IARG_CONST_CONTEXT IARG_CONTEXT
#endif

void UnistdSleepWrapper::Replace(IMG img, HandlerType handler) {
  RTN rtn = FindRTN(img, func_name_);
  if (RTN_Valid(rtn)) {
    handler_ = handler;

    // set function prototype
    PROTO proto = PROTO_Allocate(PIN_PARG(unsigned int), CALLINGSTD_DEFAULT,
                                 func_name_,
                                 PIN_PARG(unsigned int),
                                 PIN_PARG_END());

    // replace the original function with the wrapper
    RTN_ReplaceSignature(rtn, (AFUNPTR)__Wrapper,
                         IARG_PROTOTYPE, proto,
                         IARG_THREAD_ID,
                         IARG_CONST_CONTEXT,
                         IARG_RETURN_IP,
                         IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
                         IARG_END);
  }
}

void UnistdSleepWrapper::CallOriginal(UnistdSleepContext *context) {
  DEBUG_ASSERT(ori_funptr_);

  unsigned int ret_val = 0;

  // call original function
  PIN_CallApplicationFunction(context->ctxt(), context->tid(),
      CALLINGSTD_DEFAULT, ori_funptr_,
      PIN_PARG(unsigned int), &ret_val,
      PIN_PARG(unsigned int), context->seconds(),
      PIN_PARG_END());

  // set return value
  context->set_ret_val(ret_val);
}

unsigned int UnistdSleepWrapper::__Wrapper(THREADID tid, CONTEXT *ctxt,
                                           ADDRINT ret_addr,
                                           unsigned int seconds) {
  UnistdSleepContext context(tid, ctxt, ret_addr, seconds);
  handler_(&context);
  return context.ret_val();
}

void UnistdUsleepWrapper::Replace(IMG img, HandlerType handler) {
  RTN rtn = FindRTN(img, func_name_);
  if (RTN_Valid(rtn)) {
    handler_ = handler;

    // set function prototype
    PROTO proto = PROTO_Allocate(PIN_PARG(int), CALLINGSTD_DEFAULT,
                                 func_name_,
                                 PIN_PARG(useconds_t),
                                 PIN_PARG_END());

    // replace the original function with the wrapper
    RTN_ReplaceSignature(rtn, (AFUNPTR)__Wrapper,
                         IARG_PROTOTYPE, proto,
                         IARG_THREAD_ID,
                         IARG_CONST_CONTEXT,
                         IARG_RETURN_IP,
                         IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
                         IARG_END);
  }
}

void UnistdUsleepWrapper::CallOriginal(UnistdUsleepContext *context) {
  DEBUG_ASSERT(ori_funptr_);

  int ret_val = -1;

  // call original function
  PIN_CallApplicationFunction(context->ctxt(), context->tid(),
      CALLINGSTD_DEFAULT, ori_funptr_,
      PIN_PARG(int), &ret_val,
      PIN_PARG(useconds_t), context->useconds(),
      PIN_PARG_END());

  // set return value
  context->set_ret_val(ret_val);
}

int UnistdUsleepWrapper::__Wrapper(THREADID tid, CONTEXT *ctxt,
                                   ADDRINT ret_addr, useconds_t useconds) {
  UnistdUsleepContext context(tid, ctxt, ret_addr, useconds);
  handler_(&context);
  return context.ret_val();
}

void register_unistd_wrappers(IMG img) {
  UnistdSleepWrapper::Register(img, "sleep", "libc");
  UnistdUsleepWrapper::Register(img, "usleep", "libc");
}

