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
  void SetAtInteger(std::uint32_t i,use<IJsonValue>);
  void SetAtString(cr_string str,use<IJsonValue>);
  void RemoveAtInteger(std::uint32_t i);
  void RemoveAtString(cr_string str);

  std::uint32_t Size();

  use<InterfaceUnknown> ArrayCBegin();
  use<InterfaceUnknown> ArrayCEnd();
  use<InterfaceUnknown> ObjectCBegin();
  use<InterfaceUnknown> ObjectCEnd();

  std::string ToJsonString();
  std::string ToPrettyJsonString();

  CPPCOMPONENTS_CONSTRUCT(IJsonValue, GetType, GetBool, GetString, GetStringRef,
                          GetDouble, GetInt32, GetUInt32, GetInt64, GetUInt64,
                          SetNull, SetBool, SetString, SetDouble, SetInt32,
                          SetUInt32, SetInt64, SetUInt64, ToArray, ToObject,
                          PushBack, Reserve,
			  GetAtInteger,GetAtString,SetAtInteger,SetAtString,RemoveAtInteger,RemoveAtString,
                          ToJsonString, ToPrettyJsonString)

  CPPCOMPONENTS_INTERFACE_EXTRAS(IJsonValue) {
    void operator()(cr_string idx, use<IJsonValue> v) {
      this->get_interface().SetAtString(idx, v);
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

template<class T>
use<IJsonValue> to_json(T&& t);

struct IJsonValueStatics
    : define_interface<cppcomponents::uuid<0x9d61663e, 0x8855, 0x4012, 0x9746,
                                           0x24fe74ee1486> > {
  use<IJsonValue> Null();
  use<IJsonValue> FromString(cr_string string);

  CPPCOMPONENTS_CONSTRUCT(IJsonValueStatics, Null, FromString)

  CPPCOMPONENTS_STATIC_INTERFACE_EXTRAS(IJsonValueStatics) {
    template <class... T> static use<IJsonValue> Array(T &&... t) { auto value = Class::Null(); value.ToArray(); ArrayHelper(value, std::forward<T>(t0)...); return value; }
    static use<IJsonValue>  Object(){ auto value = Class::Null(); value.ToObject(); return value; }
    template<class T>
    static use<IJsonValue> Value(T&& t){ auto value = Class::Null(); value.Set(std::forward<T>(t)); return value; }
private :
    template <class T0, class... T>
    static void ArrayHelper(use<IJsonValue> &value, T0 &&t0, T &&...t) { value.PushBack(to_json(std::forward<T0>(t0)); ArrayHelper(value, std::forward<T>(t)...)); }
    static void ArrayHelper(use<IJsonValue> &value) { }
  };
};

inline std::string JsonId(){ return "cppcomponents_json_dll!Json"; }
typedef runtime_class<JsonId, object_interfaces<IJsonValue>, static_interfaces<IJsonValueStatics>, factory_interface<NoConstructorFactoryInterface>> Json_t;
typedef use_runtime_class<Json_t> Json;

template<class T>
use<IJsonValue> to_json(T&& t){
  auto value = Json::Value(std::forward<T>(t));
  return value;
}
use<IJsonValue> to_json(use<IJsonValue> value){
  return value;
}


}
}

#endif