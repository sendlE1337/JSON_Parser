#include "../include/json/details/tokenizer.h"
#include "../include/json/details/json_exceptions.h"
#include <iterator>

Tokenizer::Tokenizer(const std::string &input) : input_(input), pos_(0) {}

Token Tokenizer::GetNextToken()
{
  SkipWhitespace();

  char c = PeekCurrentChar();

  switch (c)
  {
  case '{':
    ConsumeCurrentChar();
    return Token(TokenType::LeftBrace, "{");
  case '}':
    ConsumeCurrentChar();
    return Token(TokenType::RightBrace, "}");
  case '[':
    ConsumeCurrentChar();
    return Token(TokenType::LeftBracket, "[");
  case ']':
    ConsumeCurrentChar();
    return Token(TokenType::RightBracket, "]");
  case ':':
    ConsumeCurrentChar();
    return Token(TokenType::Colon, ":");
  case ',':
    ConsumeCurrentChar();
    return Token(TokenType::Comma, ",");
  case '\0':
    return Token(TokenType::EndOfFile, "\0");
  default:
    if (c == '"')
      return ParseStringToken();
    if (std::isdigit(c) || c == '-')
      return ParseNumberToken();

    if (input_.substr(pos_, 4) == "true")
    {
      pos_ += 4;
      return Token(TokenType::True, "true");
    }
    if (input_.substr(pos_, 5) == "false")
    {
      pos_ += 5;
      return Token(TokenType::False, "false");
    }
    if (input_.substr(pos_, 4) == "null")
    {
      pos_ += 4;
      return Token(TokenType::Null, "null");
    }
  }

  return Token(TokenType::EndOfFile, "\0");
}
void Tokenizer::SkipWhitespace()
{
  while (pos_ < input_.size() && std::isspace(input_[pos_]))
    pos_++;
}
char Tokenizer::PeekCurrentChar() const
{
  if (pos_ < input_.size() && input_[pos_] != '\0')
    return input_[pos_];
  return input_[pos_];
}
char Tokenizer::ConsumeCurrentChar()
{
  if (pos_ < input_.size() && input_[pos_] != '\0')
    return input_[pos_++];
  return input_[pos_];
}
Token Tokenizer::ParseStringToken()
{
  pos_++;  
  std::string result;

  while (true) {
      if(pos_ >= input_.size())
          throw JsonParsingError("Unterminated string",0,pos_);
     
      char c = ConsumeCurrentChar();
      
      if(c == '"')
          break;
      if(c == '\\'){
          if(pos_ >= input_.size())
              throw JsonParsingError("Unterminated escape sequence", 0, pos_);
           
          char nextChar = ConsumeCurrentChar();
          switch(nextChar) {
              case 'n' : result += '\n'; break;
              case 't' : result += '\t'; break;
              case 'r' : result += '\r'; break;
              case 'b' : result += '\b'; break;
              case 'f' : result += '\f'; break;
              case '/' : result += '/'; break;
              case '"' : result += '"'; break;
              case '\\': result += '\\'; break;
              default:
                  throw JsonParsingError("Invalid escape sequence", 0, pos_);
          }
      }
      else 
          result += c;
      
   }

  return Token(TokenType::String, result);
}
Token Tokenizer::ParseNumberToken()
{
  std::string number_result;

  if (PeekCurrentChar() == '-')
    number_result += ConsumeCurrentChar();

  if (!std::isdigit(PeekCurrentChar()))
    throw std::runtime_error("Invalid number: expected digit");

  while (std::isdigit(PeekCurrentChar()))
    number_result += ConsumeCurrentChar();
  if (PeekCurrentChar() == '.')
  {
    number_result += ConsumeCurrentChar();
    if (!std::isdigit(PeekCurrentChar()))
      throw std::runtime_error("Invalid number: expected digit after '.'");
    while (std::isdigit(PeekCurrentChar()))
      number_result += ConsumeCurrentChar();
  }

  if (PeekCurrentChar() == 'e' || PeekCurrentChar() == 'E')
  {
    number_result += ConsumeCurrentChar();
    if (PeekCurrentChar() == '+' || PeekCurrentChar() == '-')
      number_result += ConsumeCurrentChar();
    if (!std::isdigit(PeekCurrentChar()))
      throw std::runtime_error("Invalid number: expected digit in exponent");
    while (std::isdigit(PeekCurrentChar()))
      number_result += ConsumeCurrentChar();
  }

  return Token(TokenType::Number, number_result);
}

size_t Tokenizer::getCurrentPosition()const{
     return pos_;
}
