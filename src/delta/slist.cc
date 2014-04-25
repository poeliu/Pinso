// File: delta/slist.h - Define suspect list database.

#include <iostream>
#include <sstream>

#include "core/logging.h"
#include "slist.h"

namespace delta {

bool SuspectList::Empty() {
  return _suspect_list_vector.empty();
}

void SuspectList::Load(const  std::string & file_name, StaticInfo *sinfo) {
    std::ifstream in(file_name.c_str());
    if (!in.good() || in.eof()) {
      in.close();
      return;
    }

  std::string line;
  while (!in.eof()) {
    std::getline(in, line);
    if (line.empty())
      break;
    std::stringstream ss(line);
    AddSuspectListVector(ss);
    AddSuspcetListObjectMap(ss);
  }
  in.close();
}

std::vector<inst_id_type> SuspectList::GetSuspect(address_t suspect_iterator) {

  if(suspect_iterator >= _suspect_list_vector.size()){
    DEBUG_FMT_PRINT("Exit: suspect_iterator is out of list size!\n");
	exit(1);
  }
  PatternVector pattern_vector = _suspect_list_vector[suspect_iterator];
  return pattern_vector;
}

bool SuspectList::IsExistInObjectMap(inst_id_type inst_id) {
  return _suspect_list_object_map.count(inst_id);
}

idiom::iRoot *SuspectList::ChooseForTest(idiom::iroot_id_t iroot_id) {
  idiom::iRoot *iroot = iroot_db_->FindiRoot(iroot_id, false);
  DEBUG_ASSERT(iroot);
  return iroot;
}

void SuspectList::AddSuspectListVector( std::istream & in) {
  inst_id_type inst_id;
  PatternVector pattern_vector_;
  in >> std::hex;
  for (int i =0; i < 4; ++i) {
    in >> inst_id;
   pattern_vector_.push_back(inst_id);
  }
  _suspect_list_vector.push_back(pattern_vector_);
}

void SuspectList::AddSuspcetListObjectMap(std::istream & in) {
  inst_id_type curr_inst, prec_inst;
  in >> std::hex;
  in >> curr_inst;
  in >> prec_inst;
  _suspect_list_object_map.insert(SuspectListObjectMap::value_type(curr_inst, NULL));
  _suspect_list_object_map.insert(SuspectListObjectMap::value_type(prec_inst, NULL));
}

}    //namespace
