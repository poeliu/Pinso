// File: delta/profiler.cpp - Implementation of the PSet profile controller.

#include "delta/profiler.hpp"

namespace delta {

void Profiler::HandlePreSetup() {
  ExecutionControl::HandlePreSetup();

  knob_->RegisterBool("ignore_lib", "Ignore all the accesses in the common libraries.", "0");
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

void Profiler::HandlePostSetup() {
  ExecutionControl::HandlePostSetup();

  iroot_db_ = new idiom::iRootDB(CreateMutex());
  iroot_db_->Load(knob_->ValueStr("iroot_in"), sinfo_);

  ilist_ = new iList(CreateMutex());

  if (observer_->Enabled()){
    observer_->Setup(CreateMutex(), sinfo_, callstack_info_, iroot_db_, ilist_);
    AddAnalyzer(observer_);
  }
}

/*bool Profiler::HandleIgnoreMemAccess(IMG img) {
  if (!IMG_Valid(img))
    return true;

  if (IMG_Name(img).find("libpthread") != std::string::npos)
    return true;

  if (knob_->ValueBool("ignore_lib")) {
    if ((IMG_Name(img).find("libc") != std::string::npos) ||
        (IMG_Name(img).find("libpthread") != std::string::npos) ||
        (IMG_Name(img).find("ld-") != std::string::npos) ||
        (IMG_Name(img).find("libstdc++") != std::string::npos) ||
        (IMG_Name(img).find("libgcc_s") != std::string::npos) ||
        (IMG_Name(img).find("libm") != std::string::npos) ||
        (IMG_Name(img).find("libnsl") != std::string::npos) ||
        (IMG_Name(img).find("librt") != std::string::npos) ||
        (IMG_Name(img).find("libdl") != std::string::npos) ||
        (IMG_Name(img).find("libz") != std::string::npos) ||
        (IMG_Name(img).find("libcrypt") != std::string::npos) ||
        (IMG_Name(img).find("libdb") != std::string::npos) ||
        (IMG_Name(img).find("libexpat") != std::string::npos) ||
        (IMG_Name(img).find("libbz2") != std::string::npos))
      return true;
  }

  return false;
}
*/

void Profiler::HandleProgramExit(){
  ExecutionControl::HandleProgramExit();

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

