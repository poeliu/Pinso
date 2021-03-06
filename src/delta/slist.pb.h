// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: delta/slist.proto

#ifndef PROTOBUF_delta_2fslist_2eproto__INCLUDED
#define PROTOBUF_delta_2fslist_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
// @@protoc_insertion_point(includes)

namespace delta {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_delta_2fslist_2eproto();
void protobuf_AssignDesc_delta_2fslist_2eproto();
void protobuf_ShutdownFile_delta_2fslist_2eproto();

class sListEntryProto;
class sListProto;

// ===================================================================

class sListEntryProto : public ::google::protobuf::Message {
 public:
  sListEntryProto();
  virtual ~sListEntryProto();
  
  sListEntryProto(const sListEntryProto& from);
  
  inline sListEntryProto& operator=(const sListEntryProto& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const sListEntryProto& default_instance();
  
  void Swap(sListEntryProto* other);
  
  // implements Message ----------------------------------------------
  
  sListEntryProto* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const sListEntryProto& from);
  void MergeFrom(const sListEntryProto& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint32 iroot_id = 1;
  inline bool has_iroot_id() const;
  inline void clear_iroot_id();
  static const int kIrootIdFieldNumber = 1;
  inline ::google::protobuf::uint32 iroot_id() const;
  inline void set_iroot_id(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:delta.sListEntryProto)
 private:
  inline void set_has_iroot_id();
  inline void clear_has_iroot_id();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::uint32 iroot_id_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_delta_2fslist_2eproto();
  friend void protobuf_AssignDesc_delta_2fslist_2eproto();
  friend void protobuf_ShutdownFile_delta_2fslist_2eproto();
  
  void InitAsDefaultInstance();
  static sListEntryProto* default_instance_;
};
// -------------------------------------------------------------------

class sListProto : public ::google::protobuf::Message {
 public:
  sListProto();
  virtual ~sListProto();
  
  sListProto(const sListProto& from);
  
  inline sListProto& operator=(const sListProto& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const sListProto& default_instance();
  
  void Swap(sListProto* other);
  
  // implements Message ----------------------------------------------
  
  sListProto* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const sListProto& from);
  void MergeFrom(const sListProto& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // repeated .delta.sListEntryProto entry = 1;
  inline int entry_size() const;
  inline void clear_entry();
  static const int kEntryFieldNumber = 1;
  inline const ::delta::sListEntryProto& entry(int index) const;
  inline ::delta::sListEntryProto* mutable_entry(int index);
  inline ::delta::sListEntryProto* add_entry();
  inline const ::google::protobuf::RepeatedPtrField< ::delta::sListEntryProto >&
      entry() const;
  inline ::google::protobuf::RepeatedPtrField< ::delta::sListEntryProto >*
      mutable_entry();
  
  // @@protoc_insertion_point(class_scope:delta.sListProto)
 private:
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::google::protobuf::RepeatedPtrField< ::delta::sListEntryProto > entry_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_delta_2fslist_2eproto();
  friend void protobuf_AssignDesc_delta_2fslist_2eproto();
  friend void protobuf_ShutdownFile_delta_2fslist_2eproto();
  
  void InitAsDefaultInstance();
  static sListProto* default_instance_;
};
// ===================================================================


// ===================================================================

// sListEntryProto

// required uint32 iroot_id = 1;
inline bool sListEntryProto::has_iroot_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void sListEntryProto::set_has_iroot_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void sListEntryProto::clear_has_iroot_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void sListEntryProto::clear_iroot_id() {
  iroot_id_ = 0u;
  clear_has_iroot_id();
}
inline ::google::protobuf::uint32 sListEntryProto::iroot_id() const {
  return iroot_id_;
}
inline void sListEntryProto::set_iroot_id(::google::protobuf::uint32 value) {
  set_has_iroot_id();
  iroot_id_ = value;
}

// -------------------------------------------------------------------

// sListProto

// repeated .delta.sListEntryProto entry = 1;
inline int sListProto::entry_size() const {
  return entry_.size();
}
inline void sListProto::clear_entry() {
  entry_.Clear();
}
inline const ::delta::sListEntryProto& sListProto::entry(int index) const {
  return entry_.Get(index);
}
inline ::delta::sListEntryProto* sListProto::mutable_entry(int index) {
  return entry_.Mutable(index);
}
inline ::delta::sListEntryProto* sListProto::add_entry() {
  return entry_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::delta::sListEntryProto >&
sListProto::entry() const {
  return entry_;
}
inline ::google::protobuf::RepeatedPtrField< ::delta::sListEntryProto >*
sListProto::mutable_entry() {
  return &entry_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace delta

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_delta_2fslist_2eproto__INCLUDED
