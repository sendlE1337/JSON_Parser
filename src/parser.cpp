#include "../include/json/details/parser.h"
#include "../include/json/details/json_exceptions.h"

Parser::Parser(Tokenizer tokenizer) : tokenizer_(tokenizer) {}

JsonValue Parser::parse() {return ParseJsonValue();}

JsonValue Parser::ParseJsonValue()
{
  current_token_ = tokenizer_.GetNextToken();

  switch (current_token_.type)
  {
  case TokenType::LeftBrace:
      return ParseObject();
  case TokenType::LeftBracket:
    return ParseArray();
  case TokenType::String:
    return ParseStringLiteral();
  case TokenType::Number:
    return ParseNumericLiteral();
  case TokenType::True:
  case TokenType::False:
  case TokenType::Null:
    return ParseLiteral();

  default:
    throw JsonParsingError("Unknown token");
  };
}

JsonValue Parser::ParseObject()
{
  std::vector<std::pair<std::string, JsonValue>> object;
        
  current_token_ = tokenizer_.GetNextToken();
 
  if(current_token_.type == TokenType::RightBrace)
      return JsonValue(object);

  while (true)
  {
    if (current_token_.type != TokenType::String)
        throw JsonParsingError("Expected string key in object",0,tokenizer_.getCurrentPosition());
    
    std::string key_token = current_token_.string_value;

    current_token_ = tokenizer_.GetNextToken();
    if (current_token_.type != TokenType::Colon)
      throw JsonParsingError("Expected ':' after key", 0, tokenizer_.getCurrentPosition());

    JsonValue value = ParseJsonValue();

    object.emplace_back(std::move(key_token),std::move(value));

    current_token_ = tokenizer_.GetNextToken();
    if (current_token_.type == TokenType::Comma){
      current_token_ = tokenizer_.GetNextToken();
      continue;
    }
    else if (current_token_.type == TokenType::RightBrace)
        break;
    else
     throw JsonParsingError("Expected ',' or '}' in object"); 
  }

  return JsonValue(std::move(object));
}

JsonValue Parser::ParseStringLiteral()
{
  if (current_token_.type != TokenType::String) 
      throw JsonTypeError("string", current_token_.string_value);

  return JsonValue(current_token_.string_value);
}

JsonValue Parser::ParseArray()
{
  std::vector<JsonValue> vector;

  while (true)
  {
    JsonValue value_token = ParseJsonValue();
    vector.push_back(value_token);

    current_token_ = tokenizer_.GetNextToken();
    if (current_token_.type == TokenType::RightBracket)
      break;
    if (current_token_.type != TokenType::Comma)
      throw JsonParsingError("Expected ',' or '}' after object member", 0, tokenizer_.getCurrentPosition());
  }
  return JsonValue(std::move(vector));
}

JsonValue Parser::ParseLiteral()
{
  if (current_token_.type == TokenType::True)
    return JsonValue(true);
  if (current_token_.type == TokenType::False)
    return JsonValue(false);
  if (current_token_.type == TokenType::Null)
    return JsonValue(nullptr);
  throw JsonTypeError("true/false/null", "unknown literal");
}

JsonValue Parser::ParseNumericLiteral()
{
  double number = std::stod(current_token_.string_value);
  return JsonValue(number);
}
