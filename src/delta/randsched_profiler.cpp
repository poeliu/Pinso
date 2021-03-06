// File: delta/randsched_profiler.cpp - Implementation of the PSet profile controller.

#include "delta/randsched_profiler.hpp"

namespace delta {

void RandSchedProfiler::HandlePreSetup() {
  randsched::Scheduler::HandlePreSetup();

  knob_->RegisterStr("iroot_in", "The input memorization database.", "iroot.db");
  knob_->RegisterStr("iroot_out", "The output memorization database.", "iroot.db");
  knob_->RegisterStr("ilist_idiom1_out", "The output memorization database.", "ilist_idiom1.db");  
  knob_->RegisterStr("ilist_idiom2_out", "The output memorization database.", "ilist_idiom2.db");  
  knob_->RegisterStr("ilist_idiom3_out", "The output memorization database.", "ilist_idiom3.db");  
  knob_->RegisterStr("ilist_idiom4_out", "The output memorization database.", "ilist_idiom4.db");  
  knob_->RegisterStr("ilist_idiom5_out", "The output memorization database.", "ilist_idiom5.db");  

  observer_ = new Observer(knob_);
  observer_->Register();
}

void RandSchedProfiler::HandlePostSetup() {
  randsched::Scheduler::HandlePostSetup();

  iroot_db_ = new idiom::iRootDB(CreateMutex());
  iroot_db_->Load(knob_->ValueStr("iroot_in"), sinfo_);

  ilist_ = new iList(CreateMutex());

  if (observer_->Enabled()){
    observer_->Setup(CreateMutex(), sinfo_, callstack_info_, iroot_db_, ilist_);
    AddAnalyzer(observer_);
  }
}

void RandSchedProfiler::HandleProgramExit(){
  randsched::Scheduler::HandleProgramExit();

  // output iroot.db
  iroot_db_->Save(knob_->ValueStr("iroot_out"), sinfo_);
  // output ilist.db
  if (knob_->ValueBool("type1"))
    ilist_->SaveIdiom1(knob_->ValueStr("ilist_idiom1_out"), sinfo_);
  if (knob_->ValueBool("type2"))
    ilist_->SaveIdiom2(knob_->ValueStr("ilist_idiom2_out"), sinfo_);
  if (knob_->ValueBool("type3"))
    ilist_->SaveIdiom3(knob_->ValueStr("ilist_idiom3_out"), sinfo_);
  if (knob_->ValueBool("type4"))
    ilist_->SaveIdiom4(knob_->ValueStr("ilist_idiom4_out"), sinfo_);
  if (knob_->ValueBool("type5"))
    ilist_->SaveIdiom5(knob_->ValueStr("ilist_idiom5_out"), sinfo_);
} 

} // namespace delta

