#ifndef TOKEN_H
#define TOKEN_H
#include <map>
#include <string>
#include <ostream>

enum class TokenType
{
  LeftBrace,    // {
  RightBrace,   // }
  LeftBracket,  // [
  RightBracket, // ]
  Comma,        // ,
  Colon,        // :
  String,       // "text"
  Number,       // 123
  True,         // true
  False,        // false
  Null,         // null
  EndOfFile     // \0
};

struct Token
{
  TokenType type;
  std::string string_value;

  Token() = default;
  Token(TokenType type_, const std::string &str) : type(type_), string_value(str) {}
};
#endif
