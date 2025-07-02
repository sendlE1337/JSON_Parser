#ifndef JSONVALUE_H
#define JSONVALUE_H
#include <ostream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <variant>

class JsonValue
{
public:
  using NullType = std::monostate;
  using BooleanType = bool;
  using NumberType = double;
  using StringType = std::string;
  using ArrayType = std::vector<JsonValue>;
  using ObjectMember = std::pair<std::string, JsonValue>;
  using ObjectStorage = std::vector<ObjectMember>;

  using Value = std::variant<NullType, BooleanType, NumberType, StringType, ArrayType, ObjectStorage>;

public:
  JsonValue(); 
  explicit JsonValue(std::nullptr_t);
  explicit JsonValue(double number) ;
  explicit JsonValue(bool value) ;
  explicit JsonValue(const std::string &str) ;
  explicit JsonValue(std::string &&str) ;
  explicit JsonValue(const ArrayType &arr);
  explicit JsonValue(ArrayType &&arr);
  explicit JsonValue(const ObjectStorage &obj); 
  explicit JsonValue(ObjectStorage &&obj) ;

  void print_json() const
  {
    std::visit(JsonPrinter{}, data_);
  }

  bool isNull()const;
  bool isBool()const;
  bool isString()const;
  bool isNumber()const;
  bool isArray()const;
  bool isObject()const;

  double getNumber()const;
  bool getBool()const;
  const std::string &getString()const;

  const std::vector<JsonValue>& getArray()const;

  const std::vector<ObjectMember> &getObject()const;
  std::string typeName() const;

  const std::string serialize(bool pretty = false, int indent = 2)const;
  void serializeValue(std::ostream &os, bool pretty = false, int currentIndent = 0, int indent = 2 )const;
  void serializeNumber(std::ostream &os, bool pretty, int currentIndent, int indent)const;
  void serializeString(std::ostream &os, bool pretty, int currentIndent,int indent)const;
  void serializeArray(std::ostream &os, bool pretty, int currentIndent, int indent)const;
  void serializeObject(std::ostream &os, bool pretty, int currentIndent, int indent)const;
private:
  Value data_;

  struct JsonPrinter
  {
    void operator()(NullType) const
    {
      std::cout << "null";
    }
    void operator()(bool value) const
    {
      std::cout << (value ? "true" : "false");
    }
    void operator()(double number) const
    {
      std::cout << number;
    }
    void operator()(const StringType &s) const
    {
      std::cout << s;
    }
    void operator()(const ArrayType &a) const
    {
      std::cout << "[";
      bool first = true;
      for (const auto &element : a)
      {
        if (!first)
          std::cout << ", ";
        first = false;
        element.print_json();
      }
      std::cout << "]";
    }

    void operator()(const ObjectStorage &o) const
    {
      std::cout << "{";
      bool first = true;
      for (const auto &[key, value] : o)
      {
        if (!first)
          std::cout << ", ";
        first = false;
        std::cout << '"' << key << "\":";
        value.print_json();
      }
      std::cout << "}";
    };
  };
};
#endif
