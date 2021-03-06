
#include "../cppcomponents_json/cppcomponents_json.hpp"
#include <cppcomponents/iterator.hpp>
#include "JRBJson-master\json.h"


using namespace cppcomponents;
using namespace json;





struct type_visitor :public boost::static_visitor<>{



  void operator()(std::int32_t)
  {
    type_ = Type::Int32;
  }
  void operator()(boost::int64_t t)
  {
    type_ = Type::Int64;
  }
  void operator()(std::uint32_t )
  {
    type_ = Type::UInt32;
  }
  void operator()(boost::uint64_t t)
  {
    type_ = Type::UInt64;
  }
  void operator()(double t)
  {
    type_ = Type::Double;
  }
  void operator()(bool b)
  {
    type_ = Type::Bool;
  }
  void operator()(const std::string& str)
  {
    type_ = Type::String;
  }
  void operator()(const jrb_json::object_type&)
  {
    type_ = Type::Object;
  }
  void operator()(const jrb_json::array_type&)
  {
    type_ = Type::Array;
  }
  void operator()(const jrb_json::null_t&)
  {
    type_ = Type::Null;
  }
  std::int32_t type_;

};

inline std::string CloneVisitorId(){ return "cppcomponents::uuid<0x17ffcbb0, 0x49b6, 0x4196, 0x9654, 0x409957b8a8dc>"; }
struct ImplementCloneVisitor :cppcomponents::implement_runtime_class<ImplementCloneVisitor,
  cppcomponents::runtime_class<CloneVisitorId, cppcomponents::object_interfaces<cppcomponents::json::IJsonVisitor>,
  cppcomponents::factory_interface<cppcomponents::NoConstructorFactoryInterface >> >
{
  void ReleaseImplementationDestroy(){
    // Do nothing
    // This is only created on the stack

  }

 use<IJsonValue> value_;

 ImplementCloneVisitor() :value_{ Json::Null() }{}

  void VisitNull(){ value_.SetNull(); }
  void VisitBool(bool b) { value_.SetBool(b); }
  void VisitString(cr_string str){ value_.SetString(str); }
  void VisitInt32(std::int32_t i){ value_.SetInt32(i); }
  void VisitUInt32(std::uint32_t i) { value_.SetUInt32(i); }
  void VisitInt64(std::int64_t i){ value_.SetInt64(i); }
  void VisitUInt64(std::uint64_t i){ value_.SetUInt64(i); };
  void VisitDouble(double d){ value_.SetDouble(d); }
  void VisitArray(use<IJsonValue> val){
    value_.SetArray();
    for (auto iter = val.ArrayCBegin(); iter != val.ArrayCEnd(); ++iter) {
      ImplementCloneVisitor v;
      (static_cast<jrb_json::ivalue>(*iter)).Visit(v.QueryInterface<IJsonVisitor>());
      value_.PushBack(v.value_); 
    }

  }
  void VisitObject(use<IJsonValue> val){
    value_.SetObject();
    for (auto iter = val.ObjectCBegin(); iter != val.ObjectCEnd(); ++iter) {
      ImplementCloneVisitor v;
      auto p = (*iter).get();
      auto key = p.first;
      p.second.Visit(v.QueryInterface<IJsonVisitor>());
      value_.SetAt(key, v.value_);
    }

  }


};


struct ImplementJson :implement_runtime_class<ImplementJson, Json_t>
{
  jrb_json::value value_;


  std::int32_t GetType(){
    type_visitor visitor;
    boost::apply_visitor(visitor, value_.jrb_get_rep());
    return visitor.type_;

  }
  bool GetBool(){
    return value_.get<bool>();
  }
  std::string GetString(){
    return value_.get<std::string>();
  }
  cr_string GetStringRef(){
    return value_.get_exact<std::string>();
  }
  double GetDouble(){
    return value_.get<double>();
  }
  std::int32_t GetInt32(){
    return value_.get<std::int32_t>();
  }
  std::uint32_t GetUInt32(){
    return value_.get<std::uint32_t>();
  }
  std::int64_t GetInt64(){
    return value_.get<std::int64_t>();
  }
  std::uint64_t GetUInt64(){
    return value_.get<std::uint64_t>();
  }

  void SetNull(){ value_ = jrb_json::null(); }
  void SetBool(bool b){ value_ = b; }
  void SetString(cr_string str){ value_ = str.to_string(); }
  void SetDouble(double d){ value_ = d; }
  void SetInt32(std::int32_t i){ value_ = i; }
  void SetUInt32(std::uint32_t i){ value_ = i; }
  void SetInt64(std::int64_t i){ value_ = i; }
  void SetUInt64(std::uint64_t i){ value_ = i; }

  void SetArray(){ value_ = jrb_json::array(); }
  void SetObject(){
    value_ = jrb_json::object();
  }

  void PushBack(use<IJsonValue> v){
    value_.push_back(v);
  }
  void Reserve(std::uint32_t size){
    value_.get_exact<jrb_json::array_type>().reserve(size);
  }

  use<IJsonValue> GetAtInteger(std::uint32_t i){
    return value_[i];
  }
  use<IJsonValue> FindAtString(cr_string str){
    auto& m = value_.get_exact<jrb_json::object_type>();
    auto iter = m.find(str.to_string());
    if (iter == m.end()){
      return nullptr;
    }
    else{
      return (*iter).second;
    }

  }

  use<IJsonValue> GetAtString(cr_string str){
    auto ret =  value_[str.to_string()];
    if (!ret){
      ret = ImplementJson::create().QueryInterface<IJsonValue>();
      ret.SetNull();
      value_[str.to_string()] = ret;
      return ret;
    }
    else{
      return ret;
    }
  }
  void SetAtInteger(std::uint32_t i, use<IJsonValue> v){
    value_[i] = v;
  }
  void SetAtString(cr_string str, use<IJsonValue> v){
    value_[str.to_string()] = v;
  }

  void RemoveAtInteger(std::uint32_t i){
    auto& v = value_.get_exact<jrb_json::array_type>();
    if (i >= v.size()) throw error_invalid_arg();
    v.erase(v.begin() + i);
  }
  void RemoveAtString(cr_string str){
    auto& m = value_.get_exact<jrb_json::object_type>();
    auto iter = m.find(str.to_string());
    if (iter == m.end()){
      throw error_invalid_arg();
    }
    else{
      m.erase(iter);
    }
  }

  std::uint32_t Size(){
    return value_.size();
  }
  typedef cppcomponents::uuid<0x31138995, 0x9caf, 0x4ffb, 0xb3a1, 0xe02867163cc5> array_iter_uuid;
  use<InterfaceUnknown> ArrayCBeginRaw(){
    
    return iterator::make_iterator<array_iter_uuid>(value_.array_begin());
  }
  use<InterfaceUnknown> ArrayCEndRaw(){
    return iterator::make_iterator<array_iter_uuid>(value_.array_end());

  }

  typedef cppcomponents::uuid<0x235e14e5, 0x65b4, 0x426b, 0x8d4e, 0x93e07f63580e> object_iter_uuid;
  use<InterfaceUnknown> ObjectCBeginRaw(){
    return iterator::make_iterator<object_iter_uuid>(value_.object_begin());
  }
  use<InterfaceUnknown> ObjectCEndRaw(){
    return iterator::make_iterator<object_iter_uuid>(value_.object_end());
  }

  void Visit(use<IJsonVisitor> visitor){
    auto type = GetType();
    switch (type){
    case Type::Array:
      visitor.VisitArray(this->QueryInterface<IJsonValue>());
      break;

    case Type::Bool:
      visitor.VisitBool(GetBool());
      break;

    case Type::Double:
      visitor.VisitDouble(GetDouble());
      break;

    case Type::Int32:
      visitor.VisitInt32(GetInt32());
      break;

    case Type::Int64:
      visitor.VisitInt64(GetInt64());
      break;

    case Type::Null:
      visitor.VisitNull();
      break;

    case Type::Object:
      visitor.VisitObject(this->QueryInterface<IJsonValue>());
      break;

    case Type::String:
      visitor.VisitString(GetStringRef());
      break;

    case Type::UInt32:
      visitor.VisitUInt32(GetUInt32());
      break;

    case Type::UInt64:
      visitor.VisitUInt64(GetUInt64());
      break;

    default:
      assert(0);
      throw cppcomponents::error_fail();




    }




  }

  static std::string ToJsonString(use<IJsonValue> val){
    return jrb_json::json_string(val);

  }
  static std::string ToFormattedJsonString(use<IJsonValue> val){
    return jrb_json::pretty_json_string(val);

  }


  ImplementJson(){}

  static use<IJsonValue> Null(){
    auto v = ImplementJson::create().QueryInterface<IJsonValue>();
    v.SetNull();
    return v;
  }
  static use<IJsonValue> FromString(cr_string str){
    return jrb_json::parse_json(str.to_string());

  }
  use<InterfaceUnknown> IClonable_Clone(){
    ImplementCloneVisitor v;
    Visit(v.QueryInterface<IJsonVisitor>());
    return v.value_;
  }
};


CPPCOMPONENTS_REGISTER(ImplementJson)

CPPCOMPONENTS_DEFINE_FACTORY()