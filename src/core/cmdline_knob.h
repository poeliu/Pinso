// File: core/cmdline_knob.h - Define command line switches.

#ifndef CORE_CMDLINE_KNOB_H_
#define CORE_CMDLINE_KNOB_H_

#include <map>

#include "core/basictypes.h"
#include "core/knob.h"

// Command line knob.
class CmdlineKnob : public Knob {
 public:
  CmdlineKnob();
  ~CmdlineKnob();

  void Parse(int argc, char *argv[]);
  void RegisterBool(const std::string &name, const std::string &desc,
                    const std::string &val);
  void RegisterInt(const std::string &name, const std::string &desc,
                   const std::string &val);
  void RegisterStr(const std::string &name, const std::string &desc,
                   const std::string &val);
  bool ValueBool(const std::string &name);
  int ValueInt(const std::string &name);
  std::string ValueStr(const std::string &name);

 private:
  typedef enum {
    KNOB_TYPE_INVALID = 0,
    KNOB_TYPE_BOOL,
    KNOB_TYPE_INT,
    KNOB_TYPE_STR,
  } KnobType;
  typedef std::pair<KnobType, void *> TypedKnob;
  typedef std::map<std::string, TypedKnob> KnobNameMap;

  bool Exist(const std::string &name);

  KnobNameMap knob_table_;

  DISALLOW_COPY_CONSTRUCTORS(CmdlineKnob);
};

#endif

