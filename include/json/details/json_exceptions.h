#ifndef JSON_EXCEPTIONS
#define JSON_EXCEPTIONS
#include <stdexcept>
#include <string>

class JsonParsingError : public std::runtime_error{
public:
    explicit JsonParsingError(const std::string & msg, size_t line = 0, size_t pos = 0) : std::runtime_error(msg + " at " + std::to_string(line) + ":" + std::to_string(pos)){}
};

class JsonTypeError : public std::runtime_error{
public:
    explicit JsonTypeError(const std::string &expected, const std::string &actual) : std::runtime_error("Expected " + expected + ", got " + actual ){}
};

class JsonKeyError : public std::runtime_error{
public:
    explicit JsonKeyError(const std::string &key_miss) : std::runtime_error("Key no found: " + key_miss){}
};
#endif
