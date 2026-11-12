package serialization

import (
	"bytes"
	"strings"
	"testing"

	"lr3/structures"
)

func createTestData() *Data {
	data := &Data{
		Array:     structures.NewArray(),
		List:      structures.NewLinkedList(),
		DList:     structures.NewDoubleLinkedList(),
		Queue:     structures.NewQueue(),
		Stack:     structures.NewStack(),
		HashTable: structures.NewHashTable(17),
		AVLTree:   structures.NewAVLTree(),
	}

	data.Array.PushBack("array-1")
	data.Array.PushBack("array-2")

	data.List.PushBack("list-1")
	data.List.PushBack("list-2")

	data.DList.PushBack("dlist-1")
	data.DList.PushBack("dlist-2")

	data.Queue.Push("queue-1")
	data.Queue.Push("queue-2")

	data.Stack.Push("stack-1")
	data.Stack.Push("stack-2")

	if err := data.HashTable.Insert(10, "ten"); err != nil {
		panic(err)
	}
	if err := data.HashTable.Insert(20, "twenty"); err != nil {
		panic(err)
	}

	data.AVLTree.Insert(30)
	data.AVLTree.Insert(10)
	data.AVLTree.Insert(20)
	data.AVLTree.Insert(40)

	return data
}

func TestSerializeDeserialize(t *testing.T) {
	original := createTestData()

	encoded, err := Serialize(original)
	if err != nil {
		t.Fatalf("ошибка сериализации: %v", err)
	}

	if !bytes.Contains(encoded, []byte(`"version": 1`)) {
		t.Fatal("в JSON отсутствует версия формата")
	}

	restored, err := Deserialize(encoded)
	if err != nil {
		t.Fatalf("ошибка десериализации: %v", err)
	}

	if got, want := restored.Array.Get(), original.Array.Get(); !equalStrings(got, want) {
		t.Errorf("Array: получено %v, ожидалось %v", got, want)
	}

	if got, want := restored.List.Get(), original.List.Get(); !equalStrings(got, want) {
		t.Errorf("List: получено %v, ожидалось %v", got, want)
	}

	if got, want := restored.DList.Get(), original.DList.Get(); !equalStrings(got, want) {
		t.Errorf("DList: получено %v, ожидалось %v", got, want)
	}

	if got, want := restored.Queue.Get(), original.Queue.Get(); !equalStrings(got, want) {
		t.Errorf("Queue: получено %v, ожидалось %v", got, want)
	}

	if got, want := restored.Stack.Get(), original.Stack.Get(); !equalStrings(got, want) {
		t.Errorf("Stack: получено %v, ожидалось %v", got, want)
	}

	if restored.HashTable.Capacity() != original.HashTable.Capacity() {
		t.Errorf(
			"HashTable capacity: получено %d, ожидалось %d",
			restored.HashTable.Capacity(),
			original.HashTable.Capacity(),
		)
	}

	for key, want := range original.HashTable.Entries() {
		got, ok := restored.HashTable.Get(key)
		if !ok || got != want {
			t.Errorf("HashTable key %d: получено %q, ожидалось %q", key, got, want)
		}
	}

	if got, want := restored.AVLTree.Inorder(), original.AVLTree.Inorder(); !equalInts(got, want) {
		t.Errorf("AVLTree: получено %v, ожидалось %v", got, want)
	}
}

func TestSerializeNilData(t *testing.T) {
	if _, err := Serialize(nil); err == nil {
		t.Fatal("ожидалась ошибка при сериализации nil")
	}
}

func TestSerializeIncompleteData(t *testing.T) {
	data := &Data{
		Array: structures.NewArray(),
	}

	if _, err := Serialize(data); err == nil {
		t.Fatal("ожидалась ошибка для неинициализированных структур")
	}
}

func TestDeserializeInvalidJSON(t *testing.T) {
	_, err := Deserialize([]byte(`{"version":`))
	if err == nil {
		t.Fatal("ожидалась ошибка для некорректного JSON")
	}
}

func TestDeserializeUnsupportedVersion(t *testing.T) {
	input := []byte(`{"version":99,"hash_capacity":11}`)

	_, err := Deserialize(input)
	if err == nil {
		t.Fatal("ожидалась ошибка для неподдерживаемой версии")
	}
}

func TestDeserializeInvalidCapacity(t *testing.T) {
	input := []byte(`{"version":1,"hash_capacity":0}`)

	_, err := Deserialize(input)
	if err == nil {
		t.Fatal("ожидалась ошибка для некорректной вместимости")
	}
}

func TestDeserializeUnknownField(t *testing.T) {
	input := []byte(`{"version":1,"hash_capacity":11,"unknown":true}`)

	_, err := Deserialize(input)
	if err == nil {
		t.Fatal("ожидалась ошибка для неизвестного поля")
	}
}

func TestDeserializeMultipleDocuments(t *testing.T) {
	input := []byte(`{"version":1,"hash_capacity":11} {"version":1}`)

	_, err := Deserialize(input)
	if err == nil {
		t.Fatal("ожидалась ошибка для нескольких JSON-документов")
	}
}

func TestDeserializeDuplicateHashKeys(t *testing.T) {
	input := []byte(`{
		"version": 1,
		"hash_capacity": 11,
		"hash_entries": [
			{"key": 5, "value": "first"},
			{"key": 5, "value": "second"}
		]
	}`)

	_, err := Deserialize(input)
	if err == nil || !strings.Contains(err.Error(), "повторяющийся ключ") {
		t.Fatalf("ожидалась ошибка повторяющегося ключа, получено: %v", err)
	}
}

func equalStrings(a, b []string) bool {
	if len(a) != len(b) {
		return false
	}

	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}

	return true
}

func equalInts(a, b []int) bool {
	if len(a) != len(b) {
		return false
	}

	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}

	return true
}
