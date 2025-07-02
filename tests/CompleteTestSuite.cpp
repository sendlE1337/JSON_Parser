#include "../include/json.h"
#include "../external/catch.hpp"
#include "../include/json/details/json_exceptions.h"
#include <fstream>

TEST_CASE("Basic JSON parsing", "[parser]") {
    SECTION("Primitive types") {
        auto jsonNull = Json::parse("null");
        REQUIRE(jsonNull.isNull());

        auto jsonBool = Json::parse("true");
        REQUIRE(jsonBool.isBoolean());
        REQUIRE(jsonBool.toBool() == true);

        auto jsonNumber = Json::parse("42.5");
        REQUIRE(jsonNumber.isNumber());
        REQUIRE(jsonNumber.toNumber() == 42.5);

        auto jsonString = Json::parse("\"hello\"");
        REQUIRE(jsonString.isString());
        REQUIRE(jsonString.toString() == "hello");
    }

    SECTION("Arrays") {
        auto jsonArray = Json::parse("[1, \"two\", true, null]");
        REQUIRE(jsonArray.isArray());
        REQUIRE(jsonArray[0].toNumber() == 1);
        REQUIRE(jsonArray[1].toString() == "two");
        REQUIRE(jsonArray[2].toBool() == true);
        REQUIRE(jsonArray[3].isNull());
    }

    SECTION("Objects") {
        auto jsonObj = Json::parse(R"({"name": "Alice", "age": 25, "active": true})");
        REQUIRE(jsonObj.isObject());
        REQUIRE(jsonObj["name"].toString() == "Alice");
        REQUIRE(jsonObj["age"].toNumber() == 25);
        REQUIRE(jsonObj["active"].toBool() == true);
    }
}

TEST_CASE("Nested structures", "[nested]") {
    SECTION("Complex object") {
        auto json = Json::parse(R"({
            "user": {
                "name": "Bob",
                "skills": ["C++", "Python"],
                "meta": {
                    "created": 1234567890
                }
            }
        })");

        REQUIRE(json["user"]["name"].toString() == "Bob");
        REQUIRE(json["user"]["skills"][0].toString() == "C++");
        REQUIRE(json["user"]["meta"]["created"].toNumber() == 1234567890);
    }

    SECTION("Array of objects") {
        auto json = Json::parse(R"([
            {"id": 1, "value": "first"},
            {"id": 2, "value": "second"}
        ])");

        REQUIRE(json[0]["id"].toNumber() == 1);
        REQUIRE(json[1]["value"].toString() == "second");
    }
}

TEST_CASE("Error handling", "[errors]") {
    SECTION("Invalid JSON") {
        REQUIRE_THROWS_AS(Json::parse("{invalid}"), JsonParsingError);
        REQUIRE_THROWS_AS(Json::parse("[1, 2, "), JsonParsingError);
    }

    SECTION("Type errors") {
        auto json = Json::parse(R"({"num": 42})");
        REQUIRE_THROWS_AS(json["num"].toString(), JsonTypeError);
        REQUIRE_THROWS_AS(json["missing"], JsonKeyError);
    }

    SECTION("Array bounds") {
        auto json = Json::parse("[1, 2, 3]");
        REQUIRE_THROWS_AS(json[3], std::out_of_range);
    }
}

TEST_CASE("Serialization", "[serialize]") {
    SECTION("Compact mode") {
        auto json = Json::parse(R"({"a":1,"b":[2,3]})");
        REQUIRE(json.serialize(false) == R"({"a" :1,"b" :[2,3]})");
    }

    SECTION("Pretty mode") {
        auto json = Json::parse(R"({"a":1,"b":[2,3]})");
        std::string expected = R"({
  "a" : 1,
  "b" : [2, 3]
})";
        REQUIRE(json.serialize(true) == expected);
    }

    SECTION("Empty structures") {
        auto emptyObj = Json::parse("{}");
        REQUIRE(emptyObj.serialize(true) == "{}");

        REQUIRE_THROWS_AS(Json::parse("[]"), JsonParsingError);
    }
}

TEST_CASE("Edge cases", "[edge]") {
    SECTION("Escaped characters") {
        auto json = Json::parse(R"({"text": "line\nbreak"})");
        REQUIRE(json["text"].toString() == "line\nbreak");
        std::string str = R"({"text" :"line)" "\n" R"(break"})";
        REQUIRE(json.serialize(false) == str);
    }

    SECTION("Large numbers") {
        auto json = Json::parse("12345678901234567890");
        REQUIRE(json.isNumber());
    }

    SECTION("Deep nesting") {
        std::string deepJson(1000, '[');
        deepJson += std::string(1000, ']');
        REQUIRE_THROWS_AS(Json::parse(deepJson), JsonParsingError);
    }
}

TEST_CASE("File loading", "[file]") {
    std::ofstream("test.json") << R"({"key": "value"})";

    auto json = Json::parseFromFile("test.json");
    REQUIRE(json["key"].toString() == "value");

    REQUIRE_THROWS_AS(Json::parseFromFile("nonexistent.json"), std::runtime_error);
}

TEST_CASE("File saving", "[file]") {
    auto json = Json::parse(R"({"test": [1, 2, 3]})");

    json.serializeToFile("output.json", true);  
    auto loadedJson = Json::parseFromFile("output.json");
    REQUIRE(loadedJson["test"][1].toNumber() == 2);
}


