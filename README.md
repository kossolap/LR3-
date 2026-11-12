# Лабораторная работа №3

Реализация структур данных на C++17 и Go.

## Реализованные структуры

- Динамический массив
- Односвязный список
- Двусвязный список
- Очередь
- Стек
- Хеш-таблица
- AVL-дерево

## Структура проекта

- `cpp/` — реализация на C++17
- `go/` — реализация на Go
- `docs/uml/` — UML-диаграммы

## C++

Сборка: `cd ~/lr3/cpp && cmake -S . -B build && cmake --build build -j`

Тесты: `cd ~/lr3/cpp && ctest --test-dir build --output-on-failure`

Покрытие: `cd ~/lr3/cpp && gcovr --root . --filter 'src/' --html-details coverage.html --print-summary`

Benchmark: `cd ~/lr3/cpp && ./build/lr3_benchmark`

## Go

Тесты: `cd ~/lr3/go && go test ./...`

Покрытие: `cd ~/lr3/go && go test -coverprofile=coverage.out ./... && go tool cover -html=coverage.out -o coverage.html`

Benchmark: `cd ~/lr3/go && go test -bench=. -benchmem ./...`

Проверка: `cd ~/lr3/go && gofmt -l structures serialization && golangci-lint run ./... && go vet ./...`

## Сериализация

- C++ — бинарная сериализация.
- Go — текстовая сериализация JSON.

## UML-диаграммы

- [Диаграмма C++](docs/uml/cpp.png)
- [Диаграмма Go](docs/uml/go.png)

## Используемые технологии

C++17, CMake, Catch2, Go, testing, golangci-lint, clang-format, gcovr, PlantUML.