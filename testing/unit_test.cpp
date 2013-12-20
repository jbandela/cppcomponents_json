#include "../cppcomponents_json/cppcomponents_json.hpp"
#include <iostream>

int main(){

  std::string json = R"({"test":1} )";

  auto value = cppcomponents::json::Json::FromString(json);

  assert(value.GetType() == cppcomponents::json::Type::Object);

  auto i = value.GetAtString("test").Get<std::int32_t>();

  assert(i == 1);

  std::cout << value.ToPrettyJsonString();
}