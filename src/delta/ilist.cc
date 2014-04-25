// File: delta/ilist.h - Implementation of ilist and ilist database.

#include "delta/ilist.h"

#include <fstream>

namespace delta {

void iAccessorGroup::Add(Inst *inst, std::vector<address_t> callstack) {
  iAccessor *iAccessor_ = new iAccessor(inst, callstack);
  iAccessor_group_map_.insert(iAccessorGroupMap::value_type(iAccessor_, NULL));
} 

void iAccessorGroup::Update(Inst *inst, std::vector<address_t> callstack) {
  bool flag = true;
  for (iAccessorGroupMap::iterator it = iAccessor_group_map_.begin();
       it != iAccessor_group_map_.end(); ++it) {
    iAccessor *iAccessor_ = it->first;
    if (iAccessor_->inst_ == inst) {
       if (iAccessor_->call_stack_ == callstack) {
         flag = false;
         break;
       }
    }
  }
  if (flag) {
    iAccessor *iAccessor_ = new iAccessor(inst, callstack);
    iAccessor_group_map_.insert(iAccessorGroupMap::value_type(iAccessor_, NULL));
  } 
}

void iAccessorGroup::Save(std::ostream *out, iAccessor *curr_iAccessor) {
  *out << std::hex; 
  uint32 map_size_ = iAccessor_group_map_.size();
  uint32 counter_ = 0;
  for(iAccessorGroupMap::iterator it = iAccessor_group_map_.begin();
      it != iAccessor_group_map_.end(); ++it, ++counter_) {
    iAccessor *iAccessor_ = it->first;
    for (std::vector<address_t>::iterator iit = iAccessor_->call_stack_.begin();
         iit != iAccessor_->call_stack_.end(); ++iit) {
      *out << (*iit) << " "; 
    }
    *out << "0x" << iAccessor_->inst_->id();
    if (counter_ != map_size_ -1) {
      *out << std::endl;
      for (std::vector<address_t>::iterator iiit = curr_iAccessor->call_stack_.begin(); iiit != curr_iAccessor->call_stack_.end(); ++iiit) {
        *out << (*iiit) << " ";
      }
      *out << "0x" << curr_iAccessor->inst_->id() << " "; 
    }
  }
}

void iAccessorGroup::Save(std::ostream *out, iAccessor *curr_iAccessor,
                          iAccessor *prev_iAccessor) {
  *out << std::hex;
  uint32 map_size_ = iAccessor_group_map_.size();
  uint32 counter_ = 0;
  for(iAccessorGroupMap::iterator it = iAccessor_group_map_.begin();
      it != iAccessor_group_map_.end(); ++it, ++counter_) {
    iAccessor *iAccessor_ = it->first;
    for (std::vector<address_t>::iterator vit = iAccessor_->call_stack_.begin();
         vit != iAccessor_->call_stack_.end(); ++vit) {
      *out << (*vit) << " "; 
    }
    *out << "0x" << iAccessor_->inst_->id();
    if (counter_ != map_size_ -1) {
      *out << std::endl;
      for (std::vector<address_t>::iterator curr_it = curr_iAccessor->call_stack_.begin(); curr_it != curr_iAccessor->call_stack_.end(); ++curr_it) {
        *out << (*curr_it) << " ";
      }
      *out << "0x" << curr_iAccessor->inst_->id() << " "; 
      for (std::vector<address_t>::iterator prev_it = prev_iAccessor->call_stack_.begin(); prev_it != prev_iAccessor->call_stack_.end(); ++prev_it) {
        *out << (*prev_it) << " ";
      }
      *out << "0x" << prev_iAccessor->inst_->id() << " "; 
    }
  } 
}

void iAccessorGroup::Save(std::ostream *out, iAccessor *curr_iAccessor,
                                             iAccessor *prev_iAccessor,
                                             iAccessor *remo_iAccessor) {
  *out << std::hex;
  uint32 map_size_ = iAccessor_group_map_.size();
  uint32 counter_ = 0;
  for(iAccessorGroupMap::iterator it = iAccessor_group_map_.begin();
      it != iAccessor_group_map_.end(); ++it, ++counter_) {
    iAccessor *iAccessor_ = it->first;
    for (std::vector<address_t>::iterator vit = iAccessor_->call_stack_.begin();
         vit != iAccessor_->call_stack_.end(); ++vit) {
      *out << (*vit) << " "; 
    }
    *out << "0x" << iAccessor_->inst_->id();
    if (counter_ != map_size_ -1) {
      *out << std::endl;
      for (std::vector<address_t>::iterator curr_it = curr_iAccessor->call_stack_.begin(); curr_it != curr_iAccessor->call_stack_.end(); ++curr_it) {
        *out << (*curr_it) << " ";
      }
      *out << "0x" << curr_iAccessor->inst_->id() << " "; 
      for (std::vector<address_t>::iterator prev_it = prev_iAccessor->call_stack_.begin(); prev_it != prev_iAccessor->call_stack_.end(); ++prev_it) {
        *out << (*prev_it) << " ";
      }
      *out << "0x" << prev_iAccessor->inst_->id() << " "; 
      for (std::vector<address_t>::iterator remo_it = remo_iAccessor->call_stack_.begin(); remo_it != remo_iAccessor->call_stack_.end(); ++remo_it) {
        *out << (*remo_it) << " ";
      }
      *out << "0x" << remo_iAccessor->inst_->id() << " "; 
    }
  } 
}

void iAccessorDoubleGroup::Add(Inst *prev_inst, std::vector<address_t> prev_callstack, Inst *remo_inst, std::vector<address_t> remo_callstack) {
  iAccessor *iAccessor_ = new iAccessor(prev_inst, prev_callstack);
  iAccessorGroup *iAccessor_group_ = new iAccessorGroup;
  iAccessor_group_->Add(remo_inst, remo_callstack);
  iAccessor_double_group_map_.insert(iAccessorDoubleGroupMap::value_type(iAccessor_, iAccessor_group_)); 
}

void iAccessorDoubleGroup::Update(Inst *prev_inst, std::vector<address_t> prev_callstack, Inst *remo_inst, std::vector<address_t> remo_callstack) {
  bool flag = true;
  for (iAccessorDoubleGroupMap::iterator it = iAccessor_double_group_map_.begin();
       it != iAccessor_double_group_map_.end(); ++it) {
    iAccessor *iaccessor_ = it->first;
    if(iaccessor_->inst_ == prev_inst) {
      if(iaccessor_->call_stack_ == prev_callstack) {
        iAccessorGroup *iAccessor_group_ = it->second;
        iAccessor_group_->Update(remo_inst, remo_callstack); 
        flag = false;
        break;
      }
    }
  }  
  if(flag){
    iAccessor *iAccessor_ = new iAccessor(prev_inst, prev_callstack);
    iAccessorGroup *iAccessor_group_ = new iAccessorGroup;
    iAccessor_group_->Add(remo_inst, remo_callstack);
    iAccessor_double_group_map_.insert(iAccessorDoubleGroupMap::value_type(iAccessor_, iAccessor_group_));
  }
}

void iAccessorDoubleGroup::Save(std::ostream *out, iAccessor *curr_iAccessor) {
  *out << std::hex; 
  uint32 map_size_ = iAccessor_double_group_map_.size();
  uint32 counter_ = 0;
  for(iAccessorDoubleGroupMap::iterator it = iAccessor_double_group_map_.begin();     it != iAccessor_double_group_map_.end(); ++it, ++counter_) {
    iAccessor *iAccessor_ = it->first;
    for (std::vector<address_t>::iterator vit = iAccessor_->call_stack_.begin();
         vit != iAccessor_->call_stack_.end(); ++vit) {
      *out << (*vit) << " "; 
    }
    *out << "0x" << iAccessor_->inst_->id() << " ";
    iAccessorGroup *iAccessor_group_ = it->second;
    iAccessor_group_->Save(out, curr_iAccessor, iAccessor_);
    if (counter_ != map_size_ -1) {
      *out << std::endl;
      for (std::vector<address_t>::iterator curr_it = curr_iAccessor->call_stack_.begin(); curr_it != curr_iAccessor->call_stack_.end(); ++curr_it) {
        *out << (*curr_it) << " ";
      }
      *out << "0x" << curr_iAccessor->inst_->id() << " "; 
    }
  }
}

void iAccessorDoubleGroup::Save(std::ostream *out, iAccessor *curr_iAccessor,
                                                   iAccessor *prev_iAccessor) {
  *out << std::hex; 
  uint32 map_size_ = iAccessor_double_group_map_.size();
  uint32 counter_ = 0;
  for(iAccessorDoubleGroupMap::iterator it = iAccessor_double_group_map_.begin();     it != iAccessor_double_group_map_.end(); ++it, ++counter_) {
    iAccessor *iAccessor_ = it->first;
    for (std::vector<address_t>::iterator vit = iAccessor_->call_stack_.begin();
         vit != iAccessor_->call_stack_.end(); ++vit) {
      *out << (*vit) << " "; 
    }
    *out << "0x" << iAccessor_->inst_->id() << " ";
    iAccessorGroup *iAccessor_group_ = it->second;
    iAccessor_group_->Save(out, curr_iAccessor, prev_iAccessor, iAccessor_);
    if (counter_ != map_size_ -1) {
      *out << std::endl;
      for (std::vector<address_t>::iterator curr_it = curr_iAccessor->call_stack_.begin(); curr_it != curr_iAccessor->call_stack_.end(); ++curr_it) {
        *out << (*curr_it) << " ";
      }
      *out << "0x" << curr_iAccessor->inst_->id() << " "; 
      for (std::vector<address_t>::iterator prev_it = prev_iAccessor->call_stack_.begin(); prev_it != prev_iAccessor->call_stack_.end(); ++prev_it) {
        *out << (*prev_it) << " ";
      }
      *out << "0x" << prev_iAccessor->inst_->id() << " "; 
    }
  }

}
void iAccessorTripleGroup::Add(Inst *prev_inst,std::vector<address_t> prev_callstack,
                               Inst *remo_inst,std::vector<address_t> remo_callstack,
                               Inst *fore_inst,std::vector<address_t> fore_callstack) {
  iAccessor *iAccessor_ = new iAccessor(prev_inst, prev_callstack);
  iAccessorDoubleGroup *iAccessor_double_group_ = new iAccessorDoubleGroup;
  iAccessor_double_group_->Add(remo_inst, remo_callstack,
                               fore_inst, fore_callstack);
  iAccessor_triple_group_map_.insert(iAccessorTripleGroupMap::value_type(iAccessor_, iAccessor_double_group_)); 
}

void iAccessorTripleGroup::Update(Inst *prev_inst,std::vector<address_t> prev_callstack,
                                Inst *remo_inst, std::vector<address_t> remo_callstack,
                                Inst *fore_inst, std::vector<address_t> fore_callstack) {
  bool flag = true;
  for (iAccessorTripleGroupMap::iterator it = iAccessor_triple_group_map_.begin();
       it != iAccessor_triple_group_map_.end(); ++it) {
    iAccessor *iaccessor_ = it->first;
    if(iaccessor_->inst_ == prev_inst) {
      if(iaccessor_->call_stack_ == prev_callstack) {
        iAccessorDoubleGroup *iAccessor_double_group_ = it->second;
        iAccessor_double_group_->Update(remo_inst, remo_callstack,
                                        fore_inst, fore_callstack); 
        flag = false;
        break;
      }
    }
  }  

  if(flag) {
    iAccessor *iAccessor_ = new iAccessor(prev_inst, prev_callstack);
    iAccessorDoubleGroup *iAccessor_double_group_ = new iAccessorDoubleGroup;
    iAccessor_double_group_->Add(remo_inst, remo_callstack,
                                 fore_inst, fore_callstack);
    iAccessor_triple_group_map_.insert(iAccessorTripleGroupMap::value_type(iAccessor_, iAccessor_double_group_)); 
  }
}

void iAccessorTripleGroup::Save(std::ostream *out, iAccessor *curr_iAccessor) {
  *out << std::hex; 
  uint32 map_size_ = iAccessor_triple_group_map_.size();
  uint32 counter_ = 0;
  for(iAccessorTripleGroupMap::iterator it = iAccessor_triple_group_map_.begin();     it != iAccessor_triple_group_map_.end(); ++it, ++counter_) {
    iAccessor *iAccessor_ = it->first;
    for (std::vector<address_t>::iterator vit = iAccessor_->call_stack_.begin();
         vit != iAccessor_->call_stack_.end(); ++vit) {
      *out << (*vit) << " "; 
    }
    *out << "0x" << iAccessor_->inst_->id() << " ";
    iAccessorDoubleGroup *iAccessor_double_group_ = it->second;
    iAccessor_double_group_->Save(out, curr_iAccessor, iAccessor_);
    if (counter_ != map_size_ -1) {
      *out << std::endl;
      for (std::vector<address_t>::iterator curr_it = curr_iAccessor->call_stack_.begin(); curr_it != curr_iAccessor->call_stack_.end(); ++curr_it) {
        *out << (*curr_it) << " ";
      }
      *out << "0x" << curr_iAccessor->inst_->id() << " "; 
    }
  }
}

// idiom-1
void iList::Update(iroot_id_t iroot_id, 
                   Inst *curr_inst, std::vector<address_t> curr_callstack,
                   Inst *prev_inst, std::vector<address_t> prev_callstack, 
                   bool locking) {
  ScopedLock locker(internal_lock_, locking);

  bool flag = true;
  for (Idiom1Map::iterator it = idiom1_map_.begin();
       it != idiom1_map_.end(); ++it) {
    iAccessor *iaccessor = it->first;
    if (iaccessor->inst_ == curr_inst) {
      if (iaccessor->call_stack_ == curr_callstack) {
        iAccessorGroup *iAccessor_group_ = it->second;
        iAccessor_group_->Update(prev_inst, prev_callstack);
        flag = false;
        break;
      }
    }
  }

  if (flag) {
    iAccessor *iAccessor_ = new iAccessor(curr_inst, curr_callstack); 
    iAccessorGroup *iAccessor_group_ = new iAccessorGroup;
    iAccessor_group_->Add(prev_inst, prev_callstack);
    idiom1_map_.insert(Idiom1Map::value_type(iAccessor_, iAccessor_group_));
  }
}

// idiom-2
void iList::Update(iroot_id_t iroot_id, 
                   Inst *curr_inst, std::vector<address_t> curr_callstack,
                   Inst *prev_inst, std::vector<address_t> prev_callstack, 
                   Inst *remo_inst, std::vector<address_t> remo_callstack, 
                   bool locking) {
  ScopedLock locker(internal_lock_, locking);

  bool flag = true;
  for(Idiom2Map::iterator it = idiom2_map_.begin();
      it != idiom2_map_.end(); ++it) {
    iAccessor *iaccessor_ = it->first;
    if(iaccessor_->inst_ == curr_inst) {
      if(iaccessor_->call_stack_ == curr_callstack) {
        iAccessorDoubleGroup *iAccessor_double_group_ = it->second;
        iAccessor_double_group_->Update(prev_inst, prev_callstack,
                                        remo_inst, remo_callstack);
        flag = false;
        break;
      }
    }
  }
  if(flag) {
    iAccessor *iAccessor_ = new iAccessor(curr_inst, curr_callstack);
    iAccessorDoubleGroup *iAccessor_double_group_ = new iAccessorDoubleGroup;
    iAccessor_double_group_->Add(prev_inst, prev_callstack,
                                 remo_inst, remo_callstack);
    idiom2_map_.insert(Idiom2Map::value_type(iAccessor_, iAccessor_double_group_));
  }
}

// idiom-3
void iList::Update(iroot_id_t iroot_id, 
                   Inst *curr_inst, std::vector<address_t> curr_callstack,
                   Inst *prev_inst, std::vector<address_t> prev_callstack, 
                   Inst *remo_inst, std::vector<address_t> remo_callstack, 
                   Inst *fore_inst, std::vector<address_t> fore_callstack,
                   bool locking) {
  ScopedLock locker(internal_lock_, locking);

  bool flag = true;
  for(Idiom3Map::iterator it = idiom3_map_.begin();
      it != idiom3_map_.end(); ++it) {
    iAccessor *iaccessor_ = it->first;
    if(iaccessor_->inst_ == curr_inst) {
      if(iaccessor_->call_stack_ == curr_callstack) {
        iAccessorTripleGroup *iAccessor_triple_group_ = it->second;
        iAccessor_triple_group_->Update(prev_inst, prev_callstack,
                                        remo_inst, remo_callstack,
                                        fore_inst, fore_callstack);
        flag = false;
        break;
      }
    }
  }
  
  if(flag) {
    iAccessor *iAccessor_ = new iAccessor(curr_inst, curr_callstack);
    iAccessorTripleGroup *iAccessor_triple_group_ = new iAccessorTripleGroup;
    iAccessor_triple_group_->Add(prev_inst, prev_callstack,
                                 remo_inst, remo_callstack,
                                 fore_inst, fore_callstack);
    idiom3_map_.insert(Idiom3Map::value_type(iAccessor_, iAccessor_triple_group_));
  }

}

void iList::SaveIdiom1(const std::string & db_name, StaticInfo * sinfo) {
  std::ofstream out(db_name.c_str());
  out << std::hex;
  for (Idiom1Map::iterator it = idiom1_map_.begin();
       it != idiom1_map_.end(); ++it) {
    iAccessor *iAccessor_ = it->first;
    iAccessorGroup *iAccessor_group_ = it->second; 
    for (std::vector<address_t>::iterator iit = iAccessor_->call_stack_.begin();
         iit != iAccessor_->call_stack_.end(); ++iit) {
      out << (*iit) << " "; 
    }
    out << "0x" << iAccessor_->inst_->id() << " ";
    iAccessor_group_->Save(&out, iAccessor_);
    out << std::endl;
  } 
  out.close();
}

void iList::SaveIdiom2(const std::string & db_name, StaticInfo * sinfo) {
  std::ofstream out(db_name.c_str());
  out << std::hex;
  for (Idiom2Map::iterator it = idiom2_map_.begin();
       it != idiom2_map_.end(); ++it) {
    iAccessor *iAccessor_ = it->first;
    iAccessorDoubleGroup *iAccessor_double_group_ = it->second;
    for (std::vector<address_t>::iterator vit = iAccessor_->call_stack_.begin();
         vit != iAccessor_->call_stack_.end(); ++vit) {
      out << (*vit) << " "; 
    }
    out << "0x" << iAccessor_->inst_->id() << " ";
    iAccessor_double_group_->Save(&out, iAccessor_);
    out << std::endl;
  }  
  out.close();
}

void iList::SaveIdiom3(const std::string & db_name, StaticInfo * sinfo) {
  std::ofstream out(db_name.c_str());
  out << std::hex;
  for (Idiom3Map::iterator it = idiom3_map_.begin();
       it != idiom3_map_.end(); ++it) {
    iAccessor *iAccessor_ = it->first;
    iAccessorTripleGroup *iAccessor_triple_group_ = it->second; 
    for (std::vector<address_t>::iterator iit = iAccessor_->call_stack_.begin();
         iit != iAccessor_->call_stack_.end(); ++iit) {
      out << (*iit) << " "; 
    }
    out << "0x" << iAccessor_->inst_->id() << " ";
    iAccessor_triple_group_->Save(&out, iAccessor_);
    out << std::endl;
  } 
  out.close();
}

void iList::SaveIdiom4(const std::string & db_name, StaticInfo * sinfo) {
  std::ofstream out(db_name.c_str());
  out << std::hex;
  for (Idiom3Map::iterator it = idiom3_map_.begin();
       it != idiom3_map_.end(); ++it) {
    iAccessor *iAccessor_ = it->first;
    iAccessorTripleGroup *iAccessor_triple_group_ = it->second; 
    for (std::vector<address_t>::iterator iit = iAccessor_->call_stack_.begin();
         iit != iAccessor_->call_stack_.end(); ++iit) {
      out << (*iit) << " "; 
    }
    out << "0x" << iAccessor_->inst_->id() << " ";
    iAccessor_triple_group_->Save(&out, iAccessor_);
    out << std::endl;
  } 
  out.close();
}

void iList::SaveIdiom5(const std::string & db_name, StaticInfo * sinfo) {
  std::ofstream out(db_name.c_str());
  out << std::hex;
  for (Idiom3Map::iterator it = idiom3_map_.begin();
       it != idiom3_map_.end(); ++it) {
    iAccessor *iAccessor_ = it->first;
    iAccessorTripleGroup *iAccessor_triple_group_ = it->second; 
    for (std::vector<address_t>::iterator iit = iAccessor_->call_stack_.begin();
         iit != iAccessor_->call_stack_.end(); ++iit) {
      out << (*iit) << " "; 
    }
    out << "0x" << iAccessor_->inst_->id() << " ";
    iAccessor_triple_group_->Save(&out, iAccessor_);
    out << std::endl;
  } 
  out.close();
}

}  // namespace delta
