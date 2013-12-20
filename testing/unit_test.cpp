#include "../cppcomponents_json/cppcomponents_json.hpp"
#include <iostream>

int main(){

  std::string json = R"({"test":1, "test1":[1,"string",1.5,{"t":1}]} )";

  auto value = cppcomponents::json::Json::FromString(json);

  assert(value.GetType() == cppcomponents::json::Type::Object);

  auto i = value.GetAtString("test").Get<std::int32_t>();

  assert(i == 1);

  std::cout << cppcomponents::json::Json::ToFormattedJsonString(value);
}