// File: core/logging.h - Define logging utilities.

#ifndef CORE_LOGGING_H_
#define CORE_LOGGING_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "core/basictypes.h"
#include "core/sync.h"

// Define log file interfaces. It can be a file, a socket, or standard
// output.
class LogFile {
 public:
  LogFile(const std::string &name) : name_(name) {}
  virtual ~LogFile() {}

  virtual void Open() = 0;
  virtual void Close() = 0;
  virtual void Write(const std::string &msg) = 0;
  virtual void Flush() = 0;
  virtual bool IsOpen() = 0;

 protected:
  std::string name_;

 private:
  DISALLOW_COPY_CONSTRUCTORS(LogFile);
};

// Define file log file. The log will be written to a file.
class FileLogFile : public LogFile {
 public:
  FileLogFile(const std::string &name) : LogFile(name) {}
  ~FileLogFile() {}

  void Open() { out_.open(name_.c_str()); }
  void Close() { out_.close(); }
  void Write(const std::string &msg) { out_ << msg; }
  void Flush() { out_.flush(); }
  bool IsOpen() { return out_.is_open(); }

 private:
  std::ofstream out_;

  DISALLOW_COPY_CONSTRUCTORS(FileLogFile);
};

// Define standard log file. The log will be written to stdout/stderr.
class StdLogFile : public LogFile {
 public:
  StdLogFile(const std::string &name) : LogFile(name), out_(NULL) {}
  ~StdLogFile() {}

  void Open();
  void Close() {}
  void Write(const std::string &msg) { *out_ << msg; }
  void Flush() { out_->flush(); }
  bool IsOpen() { return (out_ ? out_->good() : false); }

 private:
  std::ostream *out_;

  DISALLOW_COPY_CONSTRUCTORS(StdLogFile);
};

// Define a log. A log can have multiple log files so that it can write
// to multiple output ports.
class LogType {
 public:
  LogType(bool enable, bool terminate, bool buffered,
          const std::string &prefix);
  ~LogType() {}

  void ResetLogFile() { log_files_.clear(); }
  void RegisterLogFile(LogFile *log_file) { log_files_.push_back(log_file); }
  void Message(const std::string &msg, bool print_prefix = true);
  bool On() { return enable_; }
  void Enable() { enable_ = true; }
  void Disable() { enable_ = false; }
  void CloseLogFiles();

 private:
  bool enable_;
  bool terminate_;
  bool buffered_;
  std::string prefix_;
  std::vector<LogFile *> log_files_;

  DISALLOW_COPY_CONSTRUCTORS(LogType);
};

// Global definitions
extern LogFile *stdout_log_file;
extern LogFile *stderr_log_file;
extern LogType *assertion_log;
extern LogType *debug_log;
extern Mutex *g_debug_lock;

extern void logging_init(Mutex *lock);
extern void logging_fini();

#define LOG_MSG(log,msg) do { \
    if ((log)->On()) \
      (log)->Message(msg); \
  } while (0)

#define LOG_FMT_MSG(log,fmt,...) do { \
    char buffer[512]; \
    snprintf(buffer, 512, (fmt), ## __VA_ARGS__); \
    if ((log)->On()) \
      (log)->Message(std::string(buffer)); \
  } while (0)

#define __ASSERTION(msg) LOG_MSG(assertion_log, (msg))
#define __DEBUG(msg) LOG_MSG(debug_log, (msg))
#define __DEBUG_FMT(fmt,...) LOG_FMT_MSG(debug_log, fmt, ## __VA_ARGS__)

#ifdef _DEBUG
#define DEBUG_ASSERT(cond) do { \
    if(!(cond)) { \
      std::stringstream ss; \
      ss << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << std::endl; \
      __ASSERTION(ss.str()); \
    } \
  } while (0)

#define DEBUG_PRINT(msg) __DEBUG(msg)
#define DEBUG_FMT_PRINT(fmt,...) __DEBUG_FMT(fmt, ## __VA_ARGS__)
#define DEBUG_FMT_PRINT_SAFE(fmt,...) do {\
    g_debug_lock->Lock(); \
    __DEBUG_FMT(fmt, ## __VA_ARGS__); \
    g_debug_lock->Unlock(); \
  } while (0)
#else
#define DEBUG_ASSERT(cond) do {} while (0)
#define DEBUG_PRINT(msg) do {} while (0)
#define DEBUG_FMT_PRINT(fmt,...) do {} while (0)
#define DEBUG_FMT_PRINT_SAFE(fmt,...) do {} while (0)
#endif

#endif

