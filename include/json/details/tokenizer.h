#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "token.h"
class Tokenizer
{
public:
  Tokenizer(const std::string &input);

  Token GetNextToken();
  size_t getCurrentPosition() const;
private:
  size_t pos_;
  std::string input_;

  void SkipWhitespace();
  char PeekCurrentChar() const;
  char ConsumeCurrentChar();
  Token ParseStringToken();
  Token ParseNumberToken();
};
#endif
