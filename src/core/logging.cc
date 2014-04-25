// File: core/logging.h - Implementation of logging utilities.

#include "core/logging.h"

void StdLogFile::Open() {
  if (name_.compare("stdout") == 0)
    out_ = &std::cout;
  else if (name_.compare("stderr") == 0)
    out_ = &std::cerr;
}

LogType::LogType(bool enable, bool terminate, bool buffered,
                 const std::string &prefix)
    : enable_(enable),
      terminate_(terminate),
      buffered_(buffered),
      prefix_(prefix) {
  // empty
}

void LogType::Message(const std::string &msg, bool print_prefix) {
  if (!enable_)
    return;

  for (std::vector<LogFile *>::iterator it = log_files_.begin();
       it != log_files_.end(); ++it) {
    LogFile *log_file = *it;
    if (log_file->IsOpen()) {
      if (print_prefix)
        log_file->Write(prefix_);
      log_file->Write(msg);
      if (!buffered_)
        log_file->Flush();
    }
  }

  if (terminate_) {
    for (std::vector<LogFile *>::iterator it = log_files_.begin();
       it != log_files_.end(); ++it) {
      LogFile *log_file = *it;
      if (log_file->IsOpen()) {
        log_file->Flush();
      }
    }
    abort();
  }
}

void LogType::CloseLogFiles() {
  for (std::vector<LogFile *>::iterator it = log_files_.begin();
       it != log_files_.end(); ++it) {
    (*it)->Close();
  }
}

// standard output/error stream
LogFile *stdout_log_file = NULL;
LogFile *stderr_log_file = NULL;

// standard logs
LogType *assertion_log = NULL;
LogType *debug_log = NULL;

// global debug lock
Mutex *g_debug_lock = NULL;

// global functions
void logging_init(Mutex *lock) {
  // set debug lock
  g_debug_lock = lock;

  // create default log files
  stdout_log_file = new StdLogFile("stdout");
  stderr_log_file = new StdLogFile("stderr");
  stdout_log_file->Open();
  stderr_log_file->Open();

  // create default log types
  assertion_log = new LogType(true, true, false, "[ASSERT] ");
  debug_log = new LogType(true, false, false, "[DEBUG] ");

  // register default log files for default log types
  assertion_log->RegisterLogFile(stderr_log_file);
  debug_log->RegisterLogFile(stderr_log_file);
}

void logging_fini() {
  assertion_log->Disable();
  debug_log->Disable();

  stdout_log_file->Close();
  stderr_log_file->Close();
}

