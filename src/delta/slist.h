// File: delta/slist.h - Define suspect list database.

#ifndef DELTA_SLIST_H_
#define DELTA_SLIST_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

#include "core/basictypes.h"
#include "core/static_info.h"
#include "core/sync.h"
#include "idiom/iroot.h"
#include "delta/slist.pb.h"

class Inst;
class Image;
class StaticInfo;

namespace delta {

class SuspectList {
 public:
  SuspectList(Mutex *lock, idiom::iRootDB *iroot_db) 
  	         : internal_lock_(lock),
  	           iroot_db_(iroot_db){}
  ~SuspectList() {delete internal_lock_;}

  bool Empty();
  void Load(const std::string &file_name, StaticInfo *sinfo);
  std::vector<inst_id_type> GetSuspect(address_t suspect_iterator);
  bool IsExistInObjectMap(inst_id_type Inst_id);
  idiom::iRoot *ChooseForTest(idiom::iroot_id_t iroot_id);

 protected:
  Mutex *internal_lock_; 	
  idiom::iRootDB *iroot_db_;

 private:
  typedef std::vector< inst_id_type > PatternVector;
  typedef std::vector<PatternVector> SuspectListVector;
  typedef std::map<inst_id_type, void *> SuspectListObjectMap;

  SuspectListVector _suspect_list_vector;
  SuspectListObjectMap _suspect_list_object_map;

  void AddSuspectListVector(std::istream &in);
  void AddSuspcetListObjectMap(std::istream &in);

  DISALLOW_COPY_CONSTRUCTORS(SuspectList);
};

} // namespace

#endif
