// File: core/offline_tool.h - The abstract definition of offline tools.

#ifndef CORE_OFFLINE_TOOL_H_
#define CORE_OFFLINE_TOOL_H_

#include "core/basictypes.h"
#include "core/logging.h"
#include "core/sync.h"
#include "core/cmdline_knob.h"
#include "core/static_info.h"

class OfflineTool {
 public:
  OfflineTool();
  virtual ~OfflineTool();

  void Initialize();
  void PreSetup();
  void PostSetup();
  void Parse(int argc, char *argv[]);
  void Start();
  void Exit();

 protected:
  virtual Mutex *CreateMutex() { return new Mutex; }
  virtual void HandlePreSetup();
  virtual void HandlePostSetup();
  virtual void HandleStart();
  virtual void HandleExit();

  Mutex *kernel_lock_;
  CmdlineKnob *knob_;
  LogFile *debug_file_;
  StaticInfo *sinfo_;

  static OfflineTool *tool_;

 private:
  DISALLOW_COPY_CONSTRUCTORS(OfflineTool);
};

#endif

