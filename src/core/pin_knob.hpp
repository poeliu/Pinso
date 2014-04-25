// File: core/pin_knob.hpp - Define command line switches.

#ifndef CORE_PIN_KNOB_HPP_
#define CORE_PIN_KNOB_HPP_

#include <map>

#include "pin.H"

#include "core/basictypes.h"
#include "core/knob.h"

class PinKnob : public Knob {
 public:
  PinKnob() {}
  ~PinKnob() {}

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

  DISALLOW_COPY_CONSTRUCTORS(PinKnob);
};

#endif
