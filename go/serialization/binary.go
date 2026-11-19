package serialization

import (
	"bytes"
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"math"
	"sort"

	"lr3/structures"
)

const (
	binaryMagic     = "LR3GOB01"
	binaryVersion   = uint32(1)
	maxElements     = uint64(1_000_000)
	maxStringLength = uint64(100_000_000)
	maxHashCapacity = uint64(1_000_000)
)

func writeUint32(w io.Writer, value uint32) error {
	return binary.Write(w, binary.LittleEndian, value)
}

func writeUint64(w io.Writer, value uint64) error {
	return binary.Write(w, binary.LittleEndian, value)
}

func readUint32(r io.Reader) (uint32, error) {
	var value uint32
	err := binary.Read(r, binary.LittleEndian, &value)
	return value, err
}

func readUint64(r io.Reader) (uint64, error) {
	var value uint64
	err := binary.Read(r, binary.LittleEndian, &value)
	return value, err
}

func writeString(w io.Writer, value string) error {
	if uint64(len(value)) > maxStringLength {
		return errors.New("слишком длинная строка")
	}

	if err := writeUint64(w, uint64(len(value))); err != nil {
		return err
	}

	_, err := io.WriteString(w, value)
	return err
}

func readString(r io.Reader) (string, error) {
	length, err := readUint64(r)
	if err != nil {
		return "", err
	}

	if length > maxStringLength {
		return "", errors.New("некорректная длина строки")
	}

	buffer := make([]byte, int(length))
	if _, err := io.ReadFull(r, buffer); err != nil {
		return "", err
	}

	return string(buffer), nil
}

func writeStrings(w io.Writer, values []string) error {
	if uint64(len(values)) > maxElements {
		return errors.New("слишком много элементов")
	}

	if err := writeUint64(w, uint64(len(values))); err != nil {
		return err
	}

	for _, value := range values {
		if err := writeString(w, value); err != nil {
			return err
		}
	}

	return nil
}

func readStrings(r io.Reader) ([]string, error) {
	count, err := readUint64(r)
	if err != nil {
		return nil, err
	}

	if count > maxElements {
		return nil, errors.New("некорректное количество элементов")
	}

	values := make([]string, 0, int(count))

	for i := uint64(0); i < count; i++ {
		value, err := readString(r)
		if err != nil {
			return nil, err
		}

		values = append(values, value)
	}

	return values, nil
}

// SerializeBinary сохраняет семь структур данных
// в бинарный формат.
func SerializeBinary(data *Data) ([]byte, error) {
	if data == nil {
		return nil, errors.New("данные для сериализации не заданы")
	}

	if data.Array == nil ||
		data.List == nil ||
		data.DList == nil ||
		data.Queue == nil ||
		data.Stack == nil ||
		data.HashTable == nil ||
		data.AVLTree == nil {
		return nil, errors.New("одна или несколько структур не инициализированы")
	}

	hashCapacity := data.HashTable.Capacity()

	if hashCapacity <= 0 ||
		uint64(hashCapacity) > maxHashCapacity {
		return nil, errors.New("некорректная вместимость хеш-таблицы")
	}

	entries := data.HashTable.Entries()

	if uint64(len(entries)) > maxElements ||
		len(entries) > hashCapacity {
		return nil, errors.New("некорректное количество записей хеш-таблицы")
	}

	keys := make([]int, 0, len(entries))

	for key := range entries {
		keys = append(keys, key)
	}

	sort.Ints(keys)

	var buffer bytes.Buffer

	if _, err := buffer.WriteString(binaryMagic); err != nil {
		return nil, err
	}

	if err := writeUint32(&buffer, binaryVersion); err != nil {
		return nil, err
	}

	groups := [][]string{
		data.Array.Get(),
		data.List.Get(),
		data.DList.Get(),
		data.Queue.Get(),
		data.Stack.Get(),
	}

	for _, group := range groups {
		if err := writeStrings(&buffer, group); err != nil {
			return nil, err
		}
	}

	if err := writeUint64(&buffer, uint64(hashCapacity)); err != nil {
		return nil, err
	}

	if err := writeUint64(&buffer, uint64(len(keys))); err != nil {
		return nil, err
	}

	for _, key := range keys {
		if int64(key) < math.MinInt32 ||
			int64(key) > math.MaxInt32 {
			return nil, fmt.Errorf(
				"ключ хеш-таблицы выходит за диапазон int32: %d",
				key,
			)
		}

		if err := writeUint32(
			&buffer,
			uint32(int32(key)),
		); err != nil {
			return nil, err
		}

		if err := writeString(&buffer, entries[key]); err != nil {
			return nil, err
		}
	}

	avlKeys := data.AVLTree.Inorder()

	if uint64(len(avlKeys)) > maxElements {
		return nil, errors.New("слишком много ключей AVL-дерева")
	}

	if err := writeUint64(
		&buffer,
		uint64(len(avlKeys)),
	); err != nil {
		return nil, err
	}

	for _, key := range avlKeys {
		if int64(key) < math.MinInt32 ||
			int64(key) > math.MaxInt32 {
			return nil, fmt.Errorf(
				"ключ AVL-дерева выходит за диапазон int32: %d",
				key,
			)
		}

		if err := writeUint32(
			&buffer,
			uint32(int32(key)),
		); err != nil {
			return nil, err
		}
	}

	return buffer.Bytes(), nil
}

// DeserializeBinary восстанавливает семь структур
// из бинарного формата.
func DeserializeBinary(encoded []byte) (*Data, error) {
	reader := bytes.NewReader(encoded)

	magic := make([]byte, len(binaryMagic))

	if _, err := io.ReadFull(reader, magic); err != nil {
		return nil, fmt.Errorf(
			"не удалось прочитать сигнатуру: %w",
			err,
		)
	}

	if string(magic) != binaryMagic {
		return nil, errors.New("неверная сигнатура бинарного файла")
	}

	version, err := readUint32(reader)
	if err != nil {
		return nil, fmt.Errorf(
			"не удалось прочитать версию: %w",
			err,
		)
	}

	if version != binaryVersion {
		return nil, fmt.Errorf(
			"неподдерживаемая версия бинарного формата: %d",
			version,
		)
	}

	arrayValues, err := readStrings(reader)
	if err != nil {
		return nil, fmt.Errorf("ошибка чтения массива: %w", err)
	}

	listValues, err := readStrings(reader)
	if err != nil {
		return nil, fmt.Errorf("ошибка чтения списка: %w", err)
	}

	dlistValues, err := readStrings(reader)
	if err != nil {
		return nil, fmt.Errorf("ошибка чтения двойного списка: %w", err)
	}

	queueValues, err := readStrings(reader)
	if err != nil {
		return nil, fmt.Errorf("ошибка чтения очереди: %w", err)
	}

	stackValues, err := readStrings(reader)
	if err != nil {
		return nil, fmt.Errorf("ошибка чтения стека: %w", err)
	}

	hashCapacity, err := readUint64(reader)
	if err != nil {
		return nil, fmt.Errorf(
			"ошибка чтения вместимости хеш-таблицы: %w",
			err,
		)
	}

	if hashCapacity == 0 ||
		hashCapacity > maxHashCapacity {
		return nil, errors.New(
			"некорректная вместимость хеш-таблицы",
		)
	}

	hashCount, err := readUint64(reader)
	if err != nil {
		return nil, fmt.Errorf(
			"ошибка чтения количества записей хеш-таблицы: %w",
			err,
		)
	}

	if hashCount > maxElements ||
		hashCount > hashCapacity {
		return nil, errors.New(
			"некорректное количество записей хеш-таблицы",
		)
	}

	data := &Data{
		Array:     structures.NewArray(),
		List:      structures.NewLinkedList(),
		DList:     structures.NewDoubleLinkedList(),
		Queue:     structures.NewQueue(),
		Stack:     structures.NewStack(),
		HashTable: structures.NewHashTable(int(hashCapacity)),
		AVLTree:   structures.NewAVLTree(),
	}

	for _, value := range arrayValues {
		data.Array.PushBack(value)
	}

	for _, value := range listValues {
		data.List.PushBack(value)
	}

	for _, value := range dlistValues {
		data.DList.PushBack(value)
	}

	for _, value := range queueValues {
		data.Queue.Push(value)
	}

	// Get() возвращает стек от вершины к основанию.
	for i := len(stackValues) - 1; i >= 0; i-- {
		data.Stack.Push(stackValues[i])
	}

	seenHashKeys := make(map[int]bool)

	for i := uint64(0); i < hashCount; i++ {
		rawKey, err := readUint32(reader)
		if err != nil {
			return nil, fmt.Errorf(
				"ошибка чтения ключа хеш-таблицы: %w",
				err,
			)
		}

		key := int(int32(rawKey))

		value, err := readString(reader)
		if err != nil {
			return nil, fmt.Errorf(
				"ошибка чтения значения хеш-таблицы: %w",
				err,
			)
		}

		if seenHashKeys[key] {
			return nil, fmt.Errorf(
				"повторяющийся ключ хеш-таблицы: %d",
				key,
			)
		}

		seenHashKeys[key] = true

		if err := data.HashTable.Insert(key, value); err != nil {
			return nil, fmt.Errorf(
				"ошибка восстановления хеш-таблицы: %w",
				err,
			)
		}
	}

	avlCount, err := readUint64(reader)
	if err != nil {
		return nil, fmt.Errorf(
			"ошибка чтения количества ключей AVL-дерева: %w",
			err,
		)
	}

	if avlCount > maxElements {
		return nil, errors.New(
			"некорректное количество ключей AVL-дерева",
		)
	}

	seenAVLKeys := make(map[int]bool)

	for i := uint64(0); i < avlCount; i++ {
		rawKey, err := readUint32(reader)
		if err != nil {
			return nil, fmt.Errorf(
				"ошибка чтения ключа AVL-дерева: %w",
				err,
			)
		}

		key := int(int32(rawKey))

		if seenAVLKeys[key] {
			return nil, fmt.Errorf(
				"повторяющийся ключ AVL-дерева: %d",
				key,
			)
		}

		seenAVLKeys[key] = true
		data.AVLTree.Insert(key)
	}

	if reader.Len() != 0 {
		return nil, errors.New(
			"лишние данные после бинарного документа",
		)
	}

	return data, nil
}
