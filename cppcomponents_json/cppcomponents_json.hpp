#pragma once
#ifndef INCLUDE_GUARD_CPPCOMPONENTS_JSON_CPPCOMPONENTS_JSON_HPP_12_19_2013_
#define INCLUDE_GUARD_CPPCOMPONENTS_JSON_CPPCOMPONENTS_JSON_HPP_12_19_2013_
#include <cppcomponents/cppcomponents.hpp>
#include <cppcomponents/iterator.hpp>

namespace cppcomponents {
namespace json {

namespace Type {
enum {
  Null,
  Bool,
  String,
  Int32,
  UInt32,
  Int64,
  UInt64,
  Double,
  Array,
  Object
};
}

struct IJsonValue;



struct IJsonVisitor :define_interface<cppcomponents::uuid<0xae7d80fa, 0x1f96, 0x4bc1, 0x9053, 0xf4e7d461519c>>
{
  void VisitNull();
  void VisitBool(bool b);
  void VisitString(cr_string str);
  void VisitInt32(std::int32_t i);
  void VisitUInt32(std::uint32_t i);
  void VisitInt64(std::int64_t i);
  void VisitUInt64(std::uint64_t i);
  void VisitDouble(double d);
  void VisitArray(use<IJsonValue> ar);
  void VisitObject(use<IJsonValue> obj);

  CPPCOMPONENTS_CONSTRUCT(IJsonVisitor, VisitNull, VisitBool, VisitString, VisitInt32,
    VisitUInt32, VisitInt64, VisitUInt64, VisitDouble, VisitArray, VisitObject);


};

struct IJsonValue : define_interface<cppcomponents::uuid<
                        0xce866f96, 0x2d50, 0x48a7, 0xb2ca, 0xadcb82351047> > {

  std::int32_t GetType();
  bool GetBool();
  std::string GetString();
  cr_string GetStringRef();
  double GetDouble();
  std::int32_t GetInt32();
  std::uint32_t GetUInt32();
  std::int64_t GetInt64();
  std::uint64_t GetUInt64();

  void SetNull();
  void SetBool(bool b);
  void SetString(cr_string str);
  void SetDouble(double d);
  void SetInt32(std::int32_t i);
  void SetUInt32(std::uint32_t i);
  void SetInt64(std::int64_t i);
  void SetUInt64(std::uint64_t i);

  void ToArray();
  void ToObject();

  void PushBack(use<IJsonValue>);
  void Reserve(std::uint32_t size);

  use<IJsonValue> GetAtInteger(std::uint32_t i);
  use<IJsonValue> GetAtString(cr_string str);
  use<IJsonValue> FindAtString(cr_string str);
  void SetAtInteger(std::uint32_t i,use<IJsonValue>);
  void SetAtString(cr_string str,use<IJsonValue>);
  void RemoveAtInteger(std::uint32_t i);
  void RemoveAtString(cr_string str);

  std::uint32_t Size();

  use<InterfaceUnknown> ArrayCBeginRaw();
  use<InterfaceUnknown> ArrayCEndRaw();
  use<InterfaceUnknown> ObjectCBeginRaw();
  use<InterfaceUnknown> ObjectCEndRaw();

  void Visit(use<IJsonVisitor>);


  CPPCOMPONENTS_CONSTRUCT(IJsonValue, GetType, GetBool, GetString, GetStringRef,
                          GetDouble, GetInt32, GetUInt32, GetInt64, GetUInt64,
                          SetNull, SetBool, SetString, SetDouble, SetInt32,
                          SetUInt32, SetInt64, SetUInt64, ToArray, ToObject,
                          PushBack, Reserve,
			  GetAtInteger,GetAtString,FindAtString,SetAtInteger,SetAtString,RemoveAtInteger,RemoveAtString,
			  Size,ArrayCBeginRaw,ArrayCEndRaw,ObjectCBeginRaw,ObjectCEndRaw,Visit)

  CPPCOMPONENTS_INTERFACE_EXTRAS(IJsonValue) {
    void operator()(cr_string idx, use<IJsonValue> v) {
      this->get_interface().SetAtString(idx, v);
    }
    template<class T>
    void operator()(cr_string idx, T&& v) {
      auto val = this->get_interface().GetAtString(idx);
      val.Set(v);
    }
    void Set(bool v) { this->get_interface().SetBool(v); }
    void Set(cr_string v) { this->get_interface().SetString(v); }
    void Set(std::string v) { this->get_interface().SetString(v); }
    void Set(const char *v) { this->get_interface().SetString(v); }
    void Set(double v) { this->get_interface().SetDouble(v); }
    void Set(std::int32_t v) { this->get_interface().SetInt32(v); }
    void Set(std::uint32_t v) { this->get_interface().SetUInt32(v); }
    void Set(std::int64_t v) { this->get_interface().SetInt64(v); }
    void Set(std::uint64_t v) { this->get_interface().SetUInt64(v); }

    template <class T> T Get() {
      T v;
      GetHelper(v);
      return v;
    }

    bool IsNull(){
      return (this->get_interface().GetType() == Type::Null);
    }


    cppcomponents::iterator::random_access_iterator_wrapper<use<IJsonValue>> ArrayCBegin(){
      return cppcomponents::iterator::random_access_iterator_wrapper<use<IJsonValue>>{this->get_interface().ArrayCBeginRaw() };
    }
    cppcomponents::iterator::random_access_iterator_wrapper<use<IJsonValue>> ArrayCEnd(){
      return cppcomponents::iterator::random_access_iterator_wrapper<use<IJsonValue>>{this->get_interface().ArrayCEndRaw() };
    }
    cppcomponents::iterator::bidirectional_iterator_wrapper<std::pair<const std::string,  use<IJsonValue>>> ObjectCBegin(){
      return cppcomponents::iterator::bidirectional_iterator_wrapper<std::pair<const std::string,  use<IJsonValue>>>{this->get_interface().ObjectCBeginRaw() };
    }
    cppcomponents::iterator::bidirectional_iterator_wrapper<std::pair<const std::string,  use<IJsonValue>>> ObjectCEnd(){
      return cppcomponents::iterator::bidirectional_iterator_wrapper<std::pair<const std::string,  use<IJsonValue>>>{this->get_interface().ObjectCEndRaw() };
    }

  private:
    void GetHelper(bool &v) { v = this->get_interface().GetBool(); }
    void GetHelper(std::string &v) { v = this->get_interface().GetString(); }
    void GetHelper(cr_string &v) { v = this->get_interface().GetStringRef(); }
    void GetHelper(double &v) { v = this->get_interface().GetDouble(); }
    void GetHelper(std::int32_t &v) { v = this->get_interface().GetInt32(); }
    void GetHelper(std::uint32_t &v) { v = this->get_interface().GetUInt32(); }
    void GetHelper(std::int64_t &v) { v = this->get_interface().GetInt64(); }
    void GetHelper(std::uint64_t &v) { v = this->get_interface().GetUInt64(); }
  };
};


struct IJsonValueStatics
    : define_interface<cppcomponents::uuid<0x9d61663e, 0x8855, 0x4012, 0x9746,
                                           0x24fe74ee1486> > {
  use<IJsonValue> Null();
  use<IJsonValue> FromString(cr_string string);
  std::string ToJsonString(use<IJsonValue>);
  std::string ToFormattedJsonString(use<IJsonValue>);

  CPPCOMPONENTS_CONSTRUCT(IJsonValueStatics, Null, FromString,ToJsonString,ToFormattedJsonString)

  CPPCOMPONENTS_STATIC_INTERFACE_EXTRAS(IJsonValueStatics) {
    template <class... T> static use<IJsonValue> Array(T &&... t) { auto value = Class::Null(); value.ToArray(); ArrayHelper(value, std::forward<T>(t0)...); return value; }
    static use<IJsonValue>  Object(){ auto value = Class::Null(); value.ToObject(); return value; }
    template<class T>
    static use<IJsonValue> Value(T&& t){ auto value = Class::Null(); value.Set(std::forward<T>(t)); return value; }
private :
    template <class T0, class... T>
    static void ArrayHelper(use<IJsonValue> &value, T0 &&t0, T &&...t) { value.PushBack(Value(std::forward<T0>(t0))); ArrayHelper(value, std::forward<T>(t)...); }
    static void ArrayHelper(use<IJsonValue> &value) { }
    template<class... T>
    static void ArrayHelper(use<IJsonValue> &value, use<IJsonValue> t0, T &&...t) { value.PushBack(t0); ArrayHelper(value, std::forward<T>(t)...); }
  };
};

inline std::string JsonId(){ return "cppcomponents_json_dll!Json"; }
typedef runtime_class<JsonId, object_interfaces<IJsonValue>, static_interfaces<IJsonValueStatics>, factory_interface<NoConstructorFactoryInterface>> Json_t;
typedef use_runtime_class<Json_t> Json;


}
}

#endif