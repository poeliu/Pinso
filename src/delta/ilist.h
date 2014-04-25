// File: delta/ilist.h - Define of ilist and ilist database.

#ifndef DELTA_ILIST_H_
#define DELTA_ILIST_H_

#include <tr1/unordered_set>

#include "core/basictypes.h"
#include "core/static_info.h"
#include "core/sync.h"
#include "delta/ilist.pb.h"

namespace delta { 

typedef uint32 iroot_id_t;

class iAccessor {
 public:
  explicit iAccessor(Inst *inst, std::vector<address_t> call_stack) :
           inst_(inst), call_stack_(call_stack){}
  ~iAccessor() {}

  Inst *inst_;
  std::vector<address_t> call_stack_;

 private:
  DISALLOW_COPY_CONSTRUCTORS(iAccessor);
};

class iAccessorGroup {
 public:
  iAccessorGroup() {}
  ~iAccessorGroup() {}
 
  void Add(Inst *inst, std::vector<address_t> callstack);
  void Update(Inst *inst, std::vector<address_t> callstack);
  void Save(std::ostream *out, iAccessor *curr_iAccessor);
  void Save(std::ostream *out, iAccessor *curr_iAccessor, 
                               iAccessor *prev_iAccessor);
  void Save(std::ostream *out, iAccessor *curr_iAccessor, 
                               iAccessor *prev_iAccessor,
                               iAccessor *remo_iAccessor);

 private:
  typedef std::tr1::unordered_map<iAccessor *, void *> iAccessorGroupMap;

  iAccessorGroupMap iAccessor_group_map_;

  DISALLOW_COPY_CONSTRUCTORS(iAccessorGroup);
};

class iAccessorDoubleGroup {
  public:
   iAccessorDoubleGroup() {}
   ~iAccessorDoubleGroup() {}
 
  void Add(Inst *prev_inst, std::vector<address_t> prev_callstack,
           Inst *remo_inst, std::vector<address_t> remo_callstack);
  void Update(Inst *prev_inst, std::vector<address_t> prev_callstack,
              Inst *remo_inst, std::vector<address_t> remo_callstack);
  void Save(std::ostream *out, iAccessor *curr_iAccessor);
  void Save(std::ostream *out, iAccessor *curr_iAccessor,
                               iAccessor *prev_iAccessor);

  private:
    typedef std::tr1::unordered_map<iAccessor *, iAccessorGroup *> iAccessorDoubleGroupMap;
    iAccessorDoubleGroupMap iAccessor_double_group_map_;

  DISALLOW_COPY_CONSTRUCTORS(iAccessorDoubleGroup);
};

class iAccessorTripleGroup {
 public:
   iAccessorTripleGroup() {} 
   ~iAccessorTripleGroup() {} 

  void Add(Inst *prev_inst, std::vector<address_t> prev_callstack,
           Inst *remo_inst, std::vector<address_t> remo_callstack,
           Inst *fore_inst, std::vector<address_t> fore_callstack);
  void Update(Inst *prev_inst, std::vector<address_t> prev_callstack,
           Inst *remo_inst, std::vector<address_t> remo_callstack,
           Inst *fore_inst, std::vector<address_t> fore_callstack);
  void Save(std::ostream *out, iAccessor *curr_iAccessor);

 private:
   typedef std::tr1::unordered_map<iAccessor *, iAccessorDoubleGroup *> iAccessorTripleGroupMap;
   iAccessorTripleGroupMap iAccessor_triple_group_map_;

   DISALLOW_COPY_CONSTRUCTORS(iAccessorTripleGroup);
};

class iList {
 public:
  explicit iList(Mutex *lock) : internal_lock_(lock) {}
  ~iList() {}

  void Update(iroot_id_t id, 
              Inst *curr_inst, std::vector<address_t> curr_callstack,
              Inst *prev_inst, std::vector<address_t> prev_callstack, 
              bool locking);
  void Update(iroot_id_t id, 
              Inst *curr_inst, std::vector<address_t> curr_callstack,
              Inst *prev_inst, std::vector<address_t> prev_callstack, 
              Inst *remo_inst, std::vector<address_t> remo_callstack,
              bool locking);
  void Update(iroot_id_t id, 
              Inst *curr_inst, std::vector<address_t> curr_callstack,
              Inst *prev_inst, std::vector<address_t> prev_callstack, 
              Inst *remo_inst, std::vector<address_t> remo_callstack,
              Inst *fore_inst, std::vector<address_t> fore_callstack,
              bool locking);
  void SaveIdiom1(const std::string &db_name, StaticInfo *sinfo);
  void SaveIdiom2(const std::string &db_name, StaticInfo *sinfo);
  void SaveIdiom3(const std::string &db_name, StaticInfo *sinfo);
  void SaveIdiom4(const std::string &db_name, StaticInfo *sinfo);
  void SaveIdiom5(const std::string &db_name, StaticInfo *sinfo);

 protected:
  typedef std::tr1::unordered_map<iAccessor *, iAccessorGroup *> Idiom1Map;
  typedef std::tr1::unordered_map<iAccessor *, iAccessorDoubleGroup *> Idiom2Map;
  typedef std::tr1::unordered_map<iAccessor *, iAccessorTripleGroup *> Idiom3Map;

  Mutex *internal_lock_;
  Idiom1Map idiom1_map_;
  Idiom2Map idiom2_map_;
  Idiom3Map idiom3_map_;
  //  iListProto proto_;

 private:
  DISALLOW_COPY_CONSTRUCTORS(iList);
};

}

#endif
