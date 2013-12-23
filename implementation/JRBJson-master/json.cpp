//  Copyright John R. Bandela 2012
//
// Distributed under the Boost Software License, Version 1.0.
//    (See  http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include "External/rapidjson/reader.h"
#include "External/rapidjson/rapidjson.h"
#include "External/rapidjson/writer.h"
#include "External/rapidjson/prettywriter.h"
#include <stack>
#include "json.h"
#include <deque>
#include <algorithm>

namespace {

struct string_reader {
  typedef char Ch; //!< Character type. Only support char.

  string_reader(const std::string &str) : str_(&str), pos_(0) {}
  char Peek() const {
    if (pos_ >= str_->size())
      return 0;
    else
      return str_->at(pos_);
  }
  char Take() { return str_->at(pos_++); }
  size_t Tell() const { return pos_; }

  // not implemented
  void Put(char c) { ; }

  // Not implemented
  char *PutBegin() { return 0; }
  size_t PutEnd(char *) { return 0; }

  const std::string *str_;
  size_t pos_;
};

struct string_writer {
  typedef char Ch; //!< Character type. Only support char.

  string_writer() : pos_(0) {}

  void Put(char c) { str_.push_back(c); }

  // Not implemented
  char *PutBegin() { return 0; }
  size_t PutEnd(char *) { return 0; }

  std::string str_;
  size_t pos_;
};
struct parse_handler {
  typedef char Ch;
  typedef size_t SizeType;

  parse_handler() {}

  void Null() { st_.push(cppcomponents::json::Json::Null()); }
  void Bool(bool b) { st_.push(cppcomponents::json::Json::Value(b)); }
  void Int(int i) { st_.push(cppcomponents::json::Json::Value(i)); }
  void Uint(unsigned i) { st_.push(cppcomponents::json::Json::Value(i)); }
  void Int64(int64_t i) { st_.push(cppcomponents::json::Json::Value(i)); }
  void Uint64(uint64_t i) { st_.push(cppcomponents::json::Json::Value(i)); }
  void Double(double d) { st_.push(cppcomponents::json::Json::Value(d)); }
  void String(const Ch *str, SizeType length, bool copy) {
    st_.push(cppcomponents::json::Json::Value(std::string(str, str + length)));
  }
  void StartObject() {}
  void EndObject(SizeType memberCount) {
    auto obj = cppcomponents::json::Json::Object();
    for (SizeType i = 0; i < memberCount; i++) {
      check_st();
      auto val = st_.top();
      st_.pop();
      check_st();
      std::string name = st_.top().Get<std::string>();
      obj.SetAtString(name, val);
      st_.pop();
    }
    st_.push(obj);
  }
  void StartArray() {}
  void EndArray(SizeType elementCount) {
    auto v = cppcomponents::json::Json::Array();
    std::vector<jrb_json::ivalue> vec;
    for (SizeType i = 0; i < elementCount; i++) {
      check_st();
      vec.push_back(st_.top());
      st_.pop();
    }

    std::reverse(vec.begin(), vec.end());
    for (auto &e : vec) {
      v.PushBack(e);
    }
    st_.push(v);
  }

  std::stack<jrb_json::ivalue> st_;

  void check_st() {
    if (!st_.size()) {
      throw jrb_json::json_parse_error();
    }
  }
};
using namespace cppcomponents;
using namespace cppcomponents::json;
inline std::string PrintVisitorId(){ return "cppcomponents::uuid<0xec2f55cf, 0xec7d, 0x47a0, 0x8070, 0x8cb0891d050b>"; }
template <class Writer = rapidjson::Writer<string_writer> >
struct ImplementPrintVisitor:cppcomponents::implement_runtime_class<ImplementPrintVisitor<Writer>,
  cppcomponents::runtime_class<PrintVisitorId,cppcomponents::object_interfaces<cppcomponents::json::IJsonVisitor>,
  cppcomponents::factory_interface<cppcomponents::NoConstructorFactoryInterface>>>
{
  void ReleaseImplementationDestroy(){
    // Do nothing
    // This is only created on the stack

  }

  Writer* jw_;

  ImplementPrintVisitor(Writer *w) : jw_(w) {}


  void VisitNull(){ jw_->Null(); }
  void VisitBool(bool b) { jw_->Bool(b); }
  void VisitString(cr_string str){ jw_->String(str.begin()); }
  void VisitInt32(std::int32_t i){ jw_->Int(i); }
  void VisitUInt32(std::uint32_t i) { jw_->Uint(i); }
  void VisitInt64(std::int64_t i){ jw_->Uint64(i); }
  void VisitUInt64(std::uint64_t i){ jw_->Int64(i); };
  void VisitDouble(double d){ jw_->Double(d); }
  void VisitArray(use<IJsonValue> val){
    jw_->StartArray();
    for (auto iter = val.ArrayCBegin(); iter != val.ArrayCEnd(); ++iter) {
      (static_cast<jrb_json::ivalue>(*iter)).Visit(this->template QueryInterface<IJsonVisitor>());
    }
    jw_->EndArray(val.Size());

  }
  void VisitObject(use<IJsonValue> val){
    jw_->StartObject();
    for (auto iter = val.ObjectCBegin(); iter != val.ObjectCEnd(); ++iter) {
      auto p = (*iter).get();
      jw_->String(p.first.c_str());
      p.second.Visit(this->template QueryInterface<IJsonVisitor>());
    }
    jw_->EndObject(val.Size());

  }


};


}

namespace jrb_json {
ivalue parse_json(const std::string &str) {

  rapidjson::Reader r;
  parse_handler h;

  string_reader stream(str);
  if (r.Parse<0>(stream, h)) {
    h.check_st();
    return h.st_.top();
  } else {
    throw json_parse_error();
  }
}

std::string json_string(ivalue val) {
  string_writer sw;
  rapidjson::Writer<string_writer> w(sw);
  ImplementPrintVisitor<rapidjson::Writer<string_writer>> v{ &w };
  val.Visit(v.QueryInterface<IJsonVisitor>());
  return sw.str_;
}
std::string pretty_json_string(ivalue val) {
  string_writer sw;
  rapidjson::PrettyWriter<string_writer> w(sw);
  ImplementPrintVisitor<rapidjson::PrettyWriter<string_writer>> v{ &w };
  val.Visit(v.QueryInterface<IJsonVisitor>());
  return sw.str_;
}
}