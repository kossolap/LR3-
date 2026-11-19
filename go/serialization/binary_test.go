package serialization

import (
	"bytes"
	"encoding/binary"
	"math"
	"reflect"
	"strings"
	"testing"

	"lr3/structures"
)

// Проверяет совпадение содержимого всех семи структур.
func checkBinaryDataEqual(t *testing.T, got, want *Data) {
	t.Helper()

	if !reflect.DeepEqual(got.Array.Get(), want.Array.Get()) {
		t.Errorf("Array: получено %v, ожидалось %v",
			got.Array.Get(), want.Array.Get())
	}

	if !reflect.DeepEqual(got.List.Get(), want.List.Get()) {
		t.Errorf("List: получено %v, ожидалось %v",
			got.List.Get(), want.List.Get())
	}

	if !reflect.DeepEqual(got.DList.Get(), want.DList.Get()) {
		t.Errorf("DList: получено %v, ожидалось %v",
			got.DList.Get(), want.DList.Get())
	}

	if !reflect.DeepEqual(got.Queue.Get(), want.Queue.Get()) {
		t.Errorf("Queue: получено %v, ожидалось %v",
			got.Queue.Get(), want.Queue.Get())
	}

	if !reflect.DeepEqual(got.Stack.Get(), want.Stack.Get()) {
		t.Errorf("Stack: получено %v, ожидалось %v",
			got.Stack.Get(), want.Stack.Get())
	}

	if got.HashTable.Capacity() != want.HashTable.Capacity() {
		t.Errorf("HashTable capacity: получено %d, ожидалось %d",
			got.HashTable.Capacity(), want.HashTable.Capacity())
	}

	if !reflect.DeepEqual(
		got.HashTable.Entries(),
		want.HashTable.Entries(),
	) {
		t.Errorf("HashTable: получено %v, ожидалось %v",
			got.HashTable.Entries(), want.HashTable.Entries())
	}

	if !reflect.DeepEqual(
		got.AVLTree.Inorder(),
		want.AVLTree.Inorder(),
	) {
		t.Errorf("AVLTree: получено %v, ожидалось %v",
			got.AVLTree.Inorder(), want.AVLTree.Inorder())
	}
}

// Создаёт семь пустых структур.
func createEmptyBinaryData() *Data {
	return &Data{
		Array:     structures.NewArray(),
		List:      structures.NewLinkedList(),
		DList:     structures.NewDoubleLinkedList(),
		Queue:     structures.NewQueue(),
		Stack:     structures.NewStack(),
		HashTable: structures.NewHashTable(11),
		AVLTree:   structures.NewAVLTree(),
	}
}

// Создаёт корректный бинарный документ для тестов ошибок.
func validBinaryDocument(t *testing.T) []byte {
	t.Helper()

	encoded, err := SerializeBinary(createTestData())
	if err != nil {
		t.Fatalf("не удалось подготовить бинарные данные: %v", err)
	}

	return encoded
}

// Проверяет восстановление всех семи структур.
func TestBinaryRoundTripAllStructures(t *testing.T) {
	original := createTestData()

	encoded, err := SerializeBinary(original)
	if err != nil {
		t.Fatalf("ошибка бинарной сериализации: %v", err)
	}

	if !bytes.HasPrefix(encoded, []byte(binaryMagic)) {
		t.Fatal("отсутствует сигнатура бинарного формата")
	}

	restored, err := DeserializeBinary(encoded)
	if err != nil {
		t.Fatalf("ошибка бинарной десериализации: %v", err)
	}

	checkBinaryDataEqual(t, restored, original)
}

// Проверяет пустые структуры.
func TestBinaryRoundTripEmptyStructures(t *testing.T) {
	original := createEmptyBinaryData()

	encoded, err := SerializeBinary(original)
	if err != nil {
		t.Fatalf("ошибка сериализации пустых структур: %v", err)
	}

	restored, err := DeserializeBinary(encoded)
	if err != nil {
		t.Fatalf("ошибка восстановления пустых структур: %v", err)
	}

	checkBinaryDataEqual(t, restored, original)
}

// Проверяет русские буквы, кавычки, переносы строк и пустые строки.
func TestBinarySpecialCharacters(t *testing.T) {
	original := createEmptyBinaryData()

	original.Array.PushBack("Привет, мир!")
	original.Array.PushBack("")
	original.Array.PushBack("строка\nс переносом")

	original.List.PushBack("кавычки: \"текст\"")
	original.DList.PushBack("обратный слеш: \\")
	original.Queue.Push("табуляция:\tзначение")
	original.Stack.Push("специальные символы: !@#$%^&*()")

	if err := original.HashTable.Insert(-10, "отрицательный ключ"); err != nil {
		t.Fatal(err)
	}

	original.AVLTree.Insert(-100)
	original.AVLTree.Insert(0)
	original.AVLTree.Insert(100)

	encoded, err := SerializeBinary(original)
	if err != nil {
		t.Fatal(err)
	}

	restored, err := DeserializeBinary(encoded)
	if err != nil {
		t.Fatal(err)
	}

	checkBinaryDataEqual(t, restored, original)
}

// Проверяет крайние значения 32-битных ключей.
func TestBinaryInt32BoundaryKeys(t *testing.T) {
	original := createEmptyBinaryData()

	if err := original.HashTable.Insert(
		math.MinInt32, "minimum",
	); err != nil {
		t.Fatal(err)
	}

	if err := original.HashTable.Insert(
		math.MaxInt32, "maximum",
	); err != nil {
		t.Fatal(err)
	}

	original.AVLTree.Insert(math.MinInt32)
	original.AVLTree.Insert(math.MaxInt32)

	encoded, err := SerializeBinary(original)
	if err != nil {
		t.Fatal(err)
	}

	restored, err := DeserializeBinary(encoded)
	if err != nil {
		t.Fatal(err)
	}

	checkBinaryDataEqual(t, restored, original)
}

// Проверяет, что повторная сериализация даёт те же байты.
func TestBinaryDeterministicEncoding(t *testing.T) {
	data := createTestData()

	first, err := SerializeBinary(data)
	if err != nil {
		t.Fatal(err)
	}

	second, err := SerializeBinary(data)
	if err != nil {
		t.Fatal(err)
	}

	if !bytes.Equal(first, second) {
		t.Fatal("одинаковые данные дали разные бинарные документы")
	}
}

// Проверяет совместимость содержимого JSON и бинарного формата.
func TestBinaryAndJSONSameData(t *testing.T) {
	original := createTestData()

	binaryEncoded, err := SerializeBinary(original)
	if err != nil {
		t.Fatal(err)
	}

	jsonEncoded, err := Serialize(original)
	if err != nil {
		t.Fatal(err)
	}

	fromBinary, err := DeserializeBinary(binaryEncoded)
	if err != nil {
		t.Fatal(err)
	}

	fromJSON, err := Deserialize(jsonEncoded)
	if err != nil {
		t.Fatal(err)
	}

	checkBinaryDataEqual(t, fromBinary, fromJSON)
}

// Проверяет nil.
func TestBinarySerializeNil(t *testing.T) {
	if _, err := SerializeBinary(nil); err == nil {
		t.Fatal("ожидалась ошибка для nil")
	}
}

// Проверяет неинициализированные структуры.
func TestBinarySerializeIncomplete(t *testing.T) {
	data := &Data{
		Array: structures.NewArray(),
	}

	if _, err := SerializeBinary(data); err == nil {
		t.Fatal("ожидалась ошибка для неполных данных")
	}
}

// Проверяет отсутствие данных.
func TestBinaryDeserializeEmpty(t *testing.T) {
	if _, err := DeserializeBinary(nil); err == nil {
		t.Fatal("ожидалась ошибка для пустого документа")
	}
}

// Проверяет неправильную сигнатуру.
func TestBinaryInvalidMagic(t *testing.T) {
	encoded := validBinaryDocument(t)
	encoded[0] = 'X'

	if _, err := DeserializeBinary(encoded); err == nil {
		t.Fatal("ожидалась ошибка неправильной сигнатуры")
	}
}

// Проверяет неподдерживаемую версию.
func TestBinaryUnsupportedVersion(t *testing.T) {
	encoded := validBinaryDocument(t)

	binary.LittleEndian.PutUint32(
		encoded[len(binaryMagic):],
		99,
	)

	if _, err := DeserializeBinary(encoded); err == nil {
		t.Fatal("ожидалась ошибка неподдерживаемой версии")
	}
}

// Проверяет обрезанные документы на разных позициях.
func TestBinaryTruncatedDocument(t *testing.T) {
	encoded := validBinaryDocument(t)

	for i := 0; i < len(encoded); i++ {
		_, err := DeserializeBinary(encoded[:i])

		if err == nil {
			t.Fatalf(
				"ожидалась ошибка при обрезке до %d байт",
				i,
			)
		}
	}
}

// Проверяет лишние данные после документа.
func TestBinaryTrailingData(t *testing.T) {
	encoded := validBinaryDocument(t)
	encoded = append(encoded, 0xFF)

	if _, err := DeserializeBinary(encoded); err == nil {
		t.Fatal("ожидалась ошибка лишних данных")
	}
}

// Проверяет недопустимое количество элементов массива.
func TestBinaryInvalidElementCount(t *testing.T) {
	encoded := validBinaryDocument(t)

	offset := len(binaryMagic) + 4

	binary.LittleEndian.PutUint64(
		encoded[offset:],
		maxElements+1,
	)

	if _, err := DeserializeBinary(encoded); err == nil {
		t.Fatal("ожидалась ошибка количества элементов")
	}
}

// Проверяет недопустимую длину строки.
func TestBinaryInvalidStringLength(t *testing.T) {
	encoded := validBinaryDocument(t)

	// Сигнатура + версия + количество элементов массива.
	offset := len(binaryMagic) + 4 + 8

	binary.LittleEndian.PutUint64(
		encoded[offset:],
		maxStringLength+1,
	)

	if _, err := DeserializeBinary(encoded); err == nil {
		t.Fatal("ожидалась ошибка длины строки")
	}
}

// Проверяет, что некорректная вместимость хеш-таблицы
// обнаруживается при чтении.
func TestBinaryInvalidHashCapacity(t *testing.T) {
	original := createEmptyBinaryData()

	encoded, err := SerializeBinary(original)
	if err != nil {
		t.Fatal(err)
	}

	// Сигнатура + версия + пять пустых групп строк.
	offset := len(binaryMagic) + 4 + 5*8

	binary.LittleEndian.PutUint64(encoded[offset:], 0)

	if _, err := DeserializeBinary(encoded); err == nil {
		t.Fatal("ожидалась ошибка вместимости хеш-таблицы")
	}
}

// Проверяет слишком большое количество записей хеш-таблицы.
func TestBinaryInvalidHashCount(t *testing.T) {
	original := createEmptyBinaryData()

	encoded, err := SerializeBinary(original)
	if err != nil {
		t.Fatal(err)
	}

	// Сигнатура + версия + пять пустых групп + вместимость.
	offset := len(binaryMagic) + 4 + 5*8 + 8

	binary.LittleEndian.PutUint64(
		encoded[offset:],
		maxElements+1,
	)

	if _, err := DeserializeBinary(encoded); err == nil {
		t.Fatal("ожидалась ошибка количества записей")
	}
}

// Проверяет повторяющиеся ключи хеш-таблицы.
func TestBinaryDuplicateHashKeys(t *testing.T) {
	original := createEmptyBinaryData()

	if err := original.HashTable.Insert(1, "one"); err != nil {
		t.Fatal(err)
	}

	if err := original.HashTable.Insert(2, "two"); err != nil {
		t.Fatal(err)
	}

	encoded, err := SerializeBinary(original)
	if err != nil {
		t.Fatal(err)
	}

	// Сигнатура + версия + пять пустых групп +
	// вместимость + количество записей.
	offset := len(binaryMagic) + 4 + 5*8 + 8 + 8

	// Первая запись: ключ (4) + длина строки (8) + "one" (3).
	secondKeyOffset := offset + 4 + 8 + len("one")

	binary.LittleEndian.PutUint32(
		encoded[secondKeyOffset:],
		uint32(1),
	)

	_, err = DeserializeBinary(encoded)

	if err == nil ||
		!strings.Contains(err.Error(), "повторяющийся ключ") {
		t.Fatalf(
			"ожидалась ошибка повторяющегося ключа, получено: %v",
			err,
		)
	}
}

// Проверяет повторяющиеся ключи AVL-дерева.
func TestBinaryDuplicateAVLKeys(t *testing.T) {
	original := createEmptyBinaryData()

	original.AVLTree.Insert(10)
	original.AVLTree.Insert(20)

	encoded, err := SerializeBinary(original)
	if err != nil {
		t.Fatal(err)
	}

	// Сигнатура + версия + пять пустых групп +
	// вместимость + число записей хеш-таблицы +
	// количество ключей AVL.
	offset := len(binaryMagic) + 4 + 5*8 + 8 + 8 + 8

	// Второй ключ AVL следует через 4 байта после первого.
	binary.LittleEndian.PutUint32(
		encoded[offset+4:],
		uint32(10),
	)

	_, err = DeserializeBinary(encoded)

	if err == nil ||
		!strings.Contains(err.Error(), "повторяющийся ключ") {
		t.Fatalf(
			"ожидалась ошибка повторяющегося ключа AVL, получено: %v",
			err,
		)
	}
}

// Проверяет некорректное количество ключей AVL.
func TestBinaryInvalidAVLCount(t *testing.T) {
	original := createEmptyBinaryData()

	encoded, err := SerializeBinary(original)
	if err != nil {
		t.Fatal(err)
	}

	// Сигнатура + версия + пять пустых групп +
	// вместимость + число записей хеш-таблицы.
	offset := len(binaryMagic) + 4 + 5*8 + 8 + 8

	binary.LittleEndian.PutUint64(
		encoded[offset:],
		maxElements+1,
	)

	if _, err := DeserializeBinary(encoded); err == nil {
		t.Fatal("ожидалась ошибка количества ключей AVL")
	}
}
