#include "../include/json.h"
#include "../include/json/details/parser.h"
#include "../include/json/details/tokenizer.h"
#include "../include/json/details/json_exceptions.h"
#include <cstddef>
#include <iterator>
#include <memory>
#include <fstream>



struct Json::Impl{
     JsonValue value_json;
};

Json::Json() : impl_(std::make_unique<Impl>()){}


Json::Json(JsonValue value): impl_(std::make_unique<Impl>()) {
     impl_->value_json = value;
}

Json::~Json() = default;

Json Json::parse(const std::string &json_ptr){
    Json result;
    Tokenizer tokenizer(json_ptr);
    Parser parse_json(tokenizer);
    result.impl_->value_json = parse_json.parse();
    return result;
}

bool Json::isNull()const{
    return impl_->value_json.isNull(); 
}
bool Json::isNumber()const{
    return impl_->value_json.isNumber();
}
bool Json::isBoolean()const{
    return impl_->value_json.isBool();
}
bool Json::isArray()const{
    return impl_->value_json.isArray();
}
bool Json::isObject()const{
    return impl_->value_json.isObject();
}
bool Json::isString()const{
    return impl_->value_json.isString();
}

bool Json::toBool()const{
    return impl_->value_json.getBool();
}

double Json::toNumber()const{
     return impl_->value_json.getNumber();
}

const std::string& Json::toString()const{
     return impl_->value_json.getString();
}

Json Json::operator[](const std::string &key)const{
    if(!isObject()) 
        throw JsonTypeError("object", impl_->value_json.typeName());

    for(const auto& [k,v] : impl_->value_json.getObject()){
         if(k == key) return Json(v);
    }
    throw JsonKeyError(key);

}

Json Json::operator[](size_t index) const{
   if(!isArray())
      throw JsonTypeError("array", impl_->value_json.typeName());
  
   const auto &vec = impl_->value_json.getArray(); 
   if(index >= vec.size())throw std::out_of_range("Index" + std::to_string(index)+" is out of bounds");

   return Json(vec[index]);
    
}


std::string Json::serialize(bool pretty, int indent)const{
     std::string serialize_str = impl_->value_json.serialize(pretty,indent);
     return serialize_str;
}

Json Json::parseFromFile(const std::string &filepath){
    std::ifstream file(filepath);
    if(!file.is_open())
        throw std::runtime_error("Failed to open file: " + filepath);

    std::string contentJson((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return Json::parse(contentJson);
}

void Json::serializeToFile(const std::string &filepath, bool pretty, int indent)const{
     std::ofstream file(filepath);
     if(!file.is_open())
         throw std::runtime_error("Failed to create file:" + filepath);

     file << serialize(pretty, indent);
}

bool Json::isValid(const std::string &jsonStr){
     try{
          Json::parse(jsonStr);
          return true;
     }catch(...){
          return false;
     }

}
