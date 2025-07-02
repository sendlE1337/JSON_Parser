#include "../include/json/details/json_value.h"
#include "../include/json/details/json_exceptions.h"
#include <climits>
#include <ostream>
#include <sstream>
 
JsonValue::JsonValue() : data_(NullType{}) {}
JsonValue::JsonValue(std::nullptr_t) : data_(NullType{}) {}
JsonValue::JsonValue(double number) : data_(number) {}
JsonValue::JsonValue(bool value) : data_(value) {}
JsonValue::JsonValue(const std::string &str) : data_(str) {}
JsonValue::JsonValue(std::string &&str) : data_(std::move(str)) {}
JsonValue::JsonValue(const ArrayType &arr) : data_(arr) {}
JsonValue::JsonValue(ArrayType &&arr) : data_(std::move(arr)) {}
JsonValue::JsonValue(const ObjectStorage &obj) : data_(obj) {}
JsonValue::JsonValue(ObjectStorage &&obj) : data_(std::move(obj)) {}


bool JsonValue::isNumber()const{
    return std::holds_alternative<NumberType>(data_ );
}
bool JsonValue::isString()const{
    return std::holds_alternative<StringType>(data_);
}
bool JsonValue::isArray()const {
    return std::holds_alternative<ArrayType>(data_);
}
bool JsonValue::isObject()const{
    return std::holds_alternative<ObjectStorage>(data_);
}
bool JsonValue::isNull()const{
    return std::holds_alternative<NullType>(data_);
}
bool JsonValue::isBool()const{
    return std::holds_alternative<BooleanType>(data_);
}


double JsonValue::getNumber()const{
     if(!isNumber()) throw JsonTypeError("number",typeName());
     return std::get<NumberType>(data_);
}

bool JsonValue::getBool()const{
     if(!isBool())throw JsonTypeError("bool", typeName());
     return std::get<BooleanType>(data_);
}

const std::string &JsonValue::getString() const{
    if(!isString())
        throw JsonTypeError("string",typeName());
    return std::get<StringType>(data_);
}

const std::vector<JsonValue> &JsonValue::getArray()const{
     if(!isArray())throw JsonTypeError("array",typeName());
     return std::get<ArrayType>(data_);
}

const std::vector<JsonValue::ObjectMember> &JsonValue::getObject()const{
     if(!isObject())throw JsonTypeError("object",typeName());
     return std::get<ObjectStorage>(data_);
}

std::string JsonValue::typeName()const {
     if(isNull())return "null";
     if(isBool())return "bool";
     if(isNumber()) return "number";
     if(isString())return "string";
     if(isArray())return "array";
     if(isObject())return "object";
     return "unknown";
}

const std::string JsonValue::serialize(bool pretty, int indent)const{
  //Первый вариант делать билд строки из string (конкатенация). Можно ускорить код на 5-10%, но при этом это более затратно по времени.
  //Второй вариант: пойти через потоки(sstringstream). Более удобный вариант, сам распознает типы и упрощает читабельность кода. Минус - потоки используют больше памяти.
    std::stringstream ss;
    serializeValue(ss, pretty,0,indent);
    return ss.str();

};

void JsonValue::serializeValue(std::ostream &os,bool pretty, int currentIndent, int indent)const{
    switch(data_.index()){
         case 0 : os << "null"; break;
         case 1 : os << (std::get<bool>(data_) ? "true" : "false") ; break;
         case 2 : JsonValue::serializeNumber(os,pretty, currentIndent, indent);break;
         case 3 : JsonValue::serializeString(os,pretty,currentIndent,indent);break;
         case 4 : JsonValue::serializeArray(os,pretty,currentIndent,indent);break;
         case 5 : JsonValue::serializeObject(os,pretty,currentIndent,indent);break;
     }
}

void JsonValue::serializeObject(std::ostream &os, bool pretty, int currentIndent, int indent)const{
    const auto& obj = std::get<ObjectStorage>(data_);
    os << "{";

    if(pretty && !obj.empty())
        os << '\n';

    bool first = true;
    for (const auto& [key, value] : obj) {
        if (!first) {
            os << ",";
            if(pretty) os <<'\n';
        }
        if(pretty){
             os << std::string(currentIndent + indent, ' ');
        }

        os << '"' << key << '"' <<" :";
        if(pretty)os << " ";
        value.serializeValue(os,pretty,currentIndent+indent,indent); // Рекурсивный вызов
        first = false;
    }

    if(pretty && !obj.empty())
        os << '\n' <<std::string(currentIndent,' ');
    os << "}"; 
}


void JsonValue::serializeArray(std::ostream &os, bool pretty, int currentIndent, int indent) const{
    const auto &arr = std::get<ArrayType>(data_);
    os << "[";

    bool hasComplexItems = false;
    if (pretty) {
        for (const auto &item : arr) {
            if (item.isObject() || item.isArray()) {
                hasComplexItems = true;
                break;
            }
        }
    }

    if (pretty && hasComplexItems) 
        os << "\n";

    bool first = true;
    for (const auto &item : arr) {
        if (!first) {
            os << ",";
            if (pretty && hasComplexItems) os << "\n";
            else if (pretty) os << " ";
        }

        if (pretty && hasComplexItems) 
            os << std::string(currentIndent + indent, ' ');
        
        
        item.serializeValue(os, pretty, currentIndent + indent, indent);
        first = false;
    }

    if (pretty && hasComplexItems) 
        os << "\n" << std::string(currentIndent, ' ');
    
    os << "]"; 
}

void JsonValue::serializeNumber(std::ostream &os, bool pretty, int currentIndent, int indent)const{
     const auto number_memer = std::get<NumberType>(data_);

     os << number_memer;
}

void JsonValue::serializeString(std::ostream &os,bool pretty, int currentIndent, int indent)const{
    const auto string_member = std::get<StringType>(data_);

    os <<'"' <<string_member << '"';
}




