#include "../cppcomponents_json/cppcomponents_json.hpp"
#include <iostream>

int main(){

  std::string json = R"({"test":1, "test1":[1,"string",1.5,{"t":1}]} )";

  auto value = cppcomponents::json::Json::FromString(json);

  assert(value.GetType() == cppcomponents::json::Type::Object);

  auto i = value.GetAt("test").Get<std::int32_t>();

  assert(i == 1);

  

  std::cout << cppcomponents::json::Json::ToFormattedJsonString(value) << "\n";

  using cppcomponents::json::Json;
  value = Json::Object()
    ("test", 1)
    ("Array", Json::Array(1, 2, 3, 4,Json::Object(),Json::Array()));

  std::cout << cppcomponents::json::Json::ToFormattedJsonString(value) << "\n";


}