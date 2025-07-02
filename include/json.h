#ifndef JSON_H
#define JSON_H

#include <memory>
#include <string>
#include "../include/json/details/json_value.h"

class Json
{ 
public:
    Json();

    Json(const Json&)=delete;
    Json& operator=(const Json&) = delete;

    Json(Json&&) = default;
    Json& operator=(Json&&) = default;

    ~Json();

    static Json parse(const std::string &json_ptr);

    bool isNull()const;
    bool isNumber()const;
    bool isString()const;
    bool isArray()const;
    bool isObject()const;
    bool isBoolean()const;

    bool toBool()const;
    double toNumber()const;
    const std::string& toString()const;

    Json operator[](const std::string &key)const;
    Json operator[](size_t index)const;

    std::string serialize(bool pretty = false, int indent = 2)const;

    static Json parseFromFile(const std::string &filepath);
    void serializeToFile(const std::string &filepath, bool pretty = false, int indent = 2)const;
    static bool isValid(const std::string &jsonStr);
private:
   struct Impl;
   std::unique_ptr<Impl> impl_;
   Json(JsonValue value);
};


#endif
