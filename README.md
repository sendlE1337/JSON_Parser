# JSON Парсер на C++17

Высокопроизводительный парсер JSON с полной поддержкой стандарта RFC 8259.

## Основные возможности

- ⚡ Высокая скорость парсинга и сериализации
- ✅ Полная поддержка стандарта JSON:
  - Все типы данных: `null`, `boolean`, `number`, `string`, `array`, `object`
  - Экранирование строк (`\n`, `\t`, `\"` и др.)
  - Научная нотация чисел (напр., `1.23e-5`)
- 📁 Работа с файлами:
  ```cpp
  Json data = Json::parseFromFile("input.json");
  data.serializeToFile("output.json", true);  // Красивое форматирование
  ```
- 🛠 Удобный API:
  - Проверка типов ( `isNull()`, `isArray()` и др.)
  - Преобразование типов (`toBool()`, `toNumber()`, `toString()`)
  - Доступ к элементам (`operator[]` для объектов и массивов)

- 💥 Гибкая обработка ошибок:
  - Кастомные исключения (`JsonParsingError`, `JsonTypeError`, `JsonKeyError`)

## Установка и сборка
### Требования
  - CMake ≥ 3.10
  - Компилятор с поддержкой C++17

### Инструкция
```cpp
# Клонирование репозитория
git clone https://github.com/sendlE1337/JSON_Parser.git
cd json-parser

# Сборка
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON  # Опционально: -DBUILD_TESTS=OFF для отключения тестов
make

# Запуск тестов
ctest -v
```
## Примеры использования
- Базовый парсинг
  ```cpp
  Json json = Json::parse(R"(
  {
    "name": "Alice",
    "age": 30,
    "skills": ["C++", "Python"],
    "active": true
  }
  )");

  std::string name = json["name"].toString();  // "Alice"
  int age = json["age"].toNumber();            // 30
  bool active = json["active"].toBool();       // true
  ```
- Работа с файлами
  ```cpp
  Json config = Json::parseFromFile("config.json");

  config["settings"]["volume"] = 80;

  config.serializeToFile("config_updated.json", true);
  ```
## Тестирование
 ### Проект включает полный набор тестов (50+ проверок), покрывающих:
   - Парсинг всех типов данных
   - Обработку ошибок
   - Сериализацию (компактную и с форматированием)
   - Работу с файлами
   - Граничные случаи
   
  ### Запуск тестов:
  ```cpp
  ./build/json_tests
  ```














    



    
