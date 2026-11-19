package main

import (
	"fmt"
	"os"
	"path/filepath"
	"reflect"

	"lr3/serialization"
	"lr3/structures"
)

func createDemoData() (*serialization.Data, error) {
	data := &serialization.Data{
		Array:     structures.NewArray(),
		List:      structures.NewLinkedList(),
		DList:     structures.NewDoubleLinkedList(),
		Queue:     structures.NewQueue(),
		Stack:     structures.NewStack(),
		HashTable: structures.NewHashTable(17),
		AVLTree:   structures.NewAVLTree(),
	}

	data.Array.PushBack("array-one")
	data.Array.PushBack("array-two")

	data.List.PushBack("list-one")
	data.List.PushBack("list-two")

	data.DList.PushBack("double-one")
	data.DList.PushBack("double-two")

	data.Queue.Push("queue-one")
	data.Queue.Push("queue-two")

	data.Stack.Push("stack-one")
	data.Stack.Push("stack-two")

	if err := data.HashTable.Insert(10, "ten"); err != nil {
		return nil, err
	}
	if err := data.HashTable.Insert(20, "twenty"); err != nil {
		return nil, err
	}

	data.AVLTree.Insert(30)
	data.AVLTree.Insert(10)
	data.AVLTree.Insert(20)

	return data, nil
}

func printData(title string, data *serialization.Data) {
	fmt.Println()
	fmt.Println("==========", title, "==========")

	fmt.Println("1. Array:     ", data.Array.Get())
	fmt.Println("2. List:      ", data.List.Get())
	fmt.Println("3. DList:     ", data.DList.Get())
	fmt.Println("4. Queue:     ", data.Queue.Get())
	fmt.Println("5. Stack:     ", data.Stack.Get())
	fmt.Println("6. HashTable: ", data.HashTable.Entries())
	fmt.Println("   Capacity:  ", data.HashTable.Capacity())
	fmt.Println("7. AVLTree:   ", data.AVLTree.Inorder())
}

func equalData(a, b *serialization.Data) bool {
	return reflect.DeepEqual(a.Array.Get(), b.Array.Get()) &&
		reflect.DeepEqual(a.List.Get(), b.List.Get()) &&
		reflect.DeepEqual(a.DList.Get(), b.DList.Get()) &&
		reflect.DeepEqual(a.Queue.Get(), b.Queue.Get()) &&
		reflect.DeepEqual(a.Stack.Get(), b.Stack.Get()) &&
		reflect.DeepEqual(a.HashTable.Entries(), b.HashTable.Entries()) &&
		a.HashTable.Capacity() == b.HashTable.Capacity() &&
		reflect.DeepEqual(a.AVLTree.Inorder(), b.AVLTree.Inorder())
}

func saveAndLoadJSON(data *serialization.Data, filename string) (*serialization.Data, error) {
	encoded, err := serialization.Serialize(data)
	if err != nil {
		return nil, fmt.Errorf("JSON-сериализация: %w", err)
	}

	if err := os.WriteFile(filename, encoded, 0644); err != nil {
		return nil, fmt.Errorf("запись JSON-файла: %w", err)
	}

	saved, err := os.ReadFile(filename)
	if err != nil {
		return nil, fmt.Errorf("чтение JSON-файла: %w", err)
	}

	restored, err := serialization.Deserialize(saved)
	if err != nil {
		return nil, fmt.Errorf("JSON-десериализация: %w", err)
	}

	fmt.Printf("\nJSON сохранён: %s (%d байт)\n", filename, len(saved))
	return restored, nil
}

func saveAndLoadBinary(data *serialization.Data, filename string) (*serialization.Data, error) {
	encoded, err := serialization.SerializeBinary(data)
	if err != nil {
		return nil, fmt.Errorf("бинарная сериализация: %w", err)
	}

	if err := os.WriteFile(filename, encoded, 0644); err != nil {
		return nil, fmt.Errorf("запись бинарного файла: %w", err)
	}

	saved, err := os.ReadFile(filename)
	if err != nil {
		return nil, fmt.Errorf("чтение бинарного файла: %w", err)
	}

	restored, err := serialization.DeserializeBinary(saved)
	if err != nil {
		return nil, fmt.Errorf("бинарная десериализация: %w", err)
	}

	fmt.Printf("\nБинарный файл сохранён: %s (%d байт)\n", filename, len(saved))
	return restored, nil
}

func run() error {
	fmt.Println("Лабораторная работа №3 — Go")
	fmt.Println("Демонстрация сериализации семи структур данных")

	original, err := createDemoData()
	if err != nil {
		return err
	}

	printData("ИСХОДНЫЕ ДАННЫЕ", original)

	outputDir := "demo_output"

	if err := os.MkdirAll(outputDir, 0755); err != nil {
		return fmt.Errorf("создание папки: %w", err)
	}

	jsonPath := filepath.Join(outputDir, "structures.json")
	binaryPath := filepath.Join(outputDir, "structures.bin")

	fromJSON, err := saveAndLoadJSON(original, jsonPath)
	if err != nil {
		return err
	}

	printData("ВОССТАНОВЛЕНО ИЗ JSON", fromJSON)

	fromBinary, err := saveAndLoadBinary(original, binaryPath)
	if err != nil {
		return err
	}

	printData("ВОССТАНОВЛЕНО ИЗ БИНАРНОГО ФАЙЛА", fromBinary)

	fmt.Println()
	fmt.Println("========== РЕЗУЛЬТАТ ПРОВЕРКИ ==========")

	if !equalData(original, fromJSON) {
		return fmt.Errorf("данные после JSON не совпадают с исходными")
	}
	fmt.Println("JSON:       OK — все 7 структур восстановлены")

	if !equalData(original, fromBinary) {
		return fmt.Errorf("данные после бинарного формата не совпадают с исходными")
	}
	fmt.Println("Binary:     OK — все 7 структур восстановлены")

	if !equalData(fromJSON, fromBinary) {
		return fmt.Errorf("JSON и бинарный формат восстановили разные данные")
	}
	fmt.Println("Сравнение:  OK — результаты двух форматов совпадают")

	fmt.Println()
	fmt.Println("Демонстрация успешно завершена!")

	return nil
}

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "Ошибка:", err)
		os.Exit(1)
	}
}
