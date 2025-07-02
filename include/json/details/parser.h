#ifndef PARSER_H
#define PARSER_H
#include "tokenizer.h"
#include "json_value.h"
class Parser
{
public:
  Parser(Tokenizer tokenizer);

  JsonValue parse();
  void print_parse();

private:
  Tokenizer tokenizer_;
  Token current_token_;

  JsonValue ParseJsonValue();
  JsonValue ParseStringLiteral();
  JsonValue ParseObject();
  JsonValue ParseArray();
  JsonValue ParseNumericLiteral();
  JsonValue ParseLiteral();
};
#endif
