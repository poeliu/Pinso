// File: core/offline_tool.cc - The abstract implementation of offline
// tools.

#include "core/offline_tool.h"

OfflineTool *OfflineTool::tool_ = NULL;

OfflineTool::OfflineTool()
    : kernel_lock_(NULL),
      knob_(NULL),
      debug_file_(NULL),
      sinfo_(NULL) {
  // empty
}

OfflineTool::~OfflineTool() {
  // empty
}

void OfflineTool::Initialize() {
  logging_init(CreateMutex());
  kernel_lock_ = CreateMutex();
  tool_ = this;
}

void OfflineTool::PreSetup() {
  knob_ = new CmdlineKnob;
  knob_->RegisterStr("debug_out", "Debug messages output file.", "stdout");
  knob_->RegisterStr("sinfo_in", "Static info input file.", "sinfo.out");
  knob_->RegisterStr("sinfo_out", "Static info output file.", "sinfo.out");

  HandlePreSetup();
}

void OfflineTool::PostSetup() {
  // setup debug output
  if (knob_->ValueStr("debug_out").compare("stderr") == 0) {
    debug_log->ResetLogFile();
    debug_log->RegisterLogFile(stderr_log_file);
  } else if (knob_->ValueStr("debug_out").compare("stdout") == 0) {
    debug_log->ResetLogFile();
    debug_log->RegisterLogFile(stdout_log_file);
  } else {
    debug_file_ = new FileLogFile(knob_->ValueStr("debug_out"));
    debug_file_->Open();
    debug_log->ResetLogFile();
    debug_log->RegisterLogFile(debug_file_);
  }

  // load static info
  sinfo_ = new StaticInfo(CreateMutex());
  sinfo_->Load(knob_->ValueStr("sinfo_in"));

  HandlePostSetup();
}

void OfflineTool::Parse(int argc, char *argv[]) {
  knob_->Parse(argc, argv);
}

void OfflineTool::Start() {
  HandleStart();
}

void OfflineTool::Exit() {
  HandleExit();

  // save static info
  sinfo_->Save(knob_->ValueStr("sinfo_out"));

  // close debug file if exists
  if (debug_file_)
    debug_file_->Close();

  // finalize logging
  logging_fini();
}

void OfflineTool::HandlePreSetup() {
  // empty
}

void OfflineTool::HandlePostSetup() {
  // empty
}

void OfflineTool::HandleStart() {
  // empty
}

void OfflineTool::HandleExit() {
  // empty
}

