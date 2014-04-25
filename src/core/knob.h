// File: core/knob.h - Define command line switches.

#ifndef CORE_KNOB_H_
#define CORE_KNOB_H_

#include "core/basictypes.h"

// The interface class for the command line switches.
class Knob {
 public:
  Knob() {}
  virtual ~Knob() {}

  virtual void RegisterBool(const std::string &name, const std::string &desc,
                            const std::string &val) = 0;
  virtual void RegisterInt(const std::string &name, const std::string &desc,
                           const std::string &val) = 0;
  virtual void RegisterStr(const std::string &name, const std::string &desc,
                           const std::string &val) = 0;
  virtual bool ValueBool(const std::string &name) = 0;
  virtual int ValueInt(const std::string &name) = 0;
  virtual std::string ValueStr(const std::string &name) = 0;

 private:
  DISALLOW_COPY_CONSTRUCTORS(Knob);
};

#endif

