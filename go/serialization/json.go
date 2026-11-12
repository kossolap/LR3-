package serialization

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"sort"

	"lr3/structures"
)

const formatVersion = 1

// Data объединяет все структуры, которые сериализуются одним документом.
type Data struct {
	Array     *structures.Array
	List      *structures.LinkedList
	DList     *structures.DoubleLinkedList
	Queue     *structures.Queue
	Stack     *structures.Stack
	HashTable *structures.HashTable
	AVLTree   *structures.AVLTree
}

type hashEntry struct {
	Key   int    `json:"key"`
	Value string `json:"value"`
}

type document struct {
	Version      int         `json:"version"`
	Array        []string    `json:"array"`
	List         []string    `json:"list"`
	DoubleList   []string    `json:"double_list"`
	Queue        []string    `json:"queue"`
	Stack        []string    `json:"stack"`
	HashCapacity int         `json:"hash_capacity"`
	HashEntries  []hashEntry `json:"hash_entries"`
	AVLKeys      []int       `json:"avl_keys"`
}

// Serialize преобразует все структуры в текстовый JSON-документ.
func Serialize(data *Data) ([]byte, error) {
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

	entries := data.HashTable.Entries()
	keys := make([]int, 0, len(entries))

	for key := range entries {
		keys = append(keys, key)
	}
	sort.Ints(keys)

	hashEntries := make([]hashEntry, 0, len(keys))
	for _, key := range keys {
		hashEntries = append(hashEntries, hashEntry{
			Key:   key,
			Value: entries[key],
		})
	}

	doc := document{
		Version:      formatVersion,
		Array:        data.Array.Get(),
		List:         data.List.Get(),
		DoubleList:   data.DList.Get(),
		Queue:        data.Queue.Get(),
		Stack:        data.Stack.Get(),
		HashCapacity: data.HashTable.Capacity(),
		HashEntries:  hashEntries,
		AVLKeys:      data.AVLTree.Inorder(),
	}

	return json.MarshalIndent(doc, "", "  ")
}

// Deserialize восстанавливает все структуры из JSON-документа.
func Deserialize(encoded []byte) (*Data, error) {
	decoder := json.NewDecoder(bytes.NewReader(encoded))
	decoder.DisallowUnknownFields()

	var doc document
	if err := decoder.Decode(&doc); err != nil {
		return nil, fmt.Errorf("ошибка чтения JSON: %w", err)
	}

	var extra interface{}
	if err := decoder.Decode(&extra); err != io.EOF {
		if err == nil {
			return nil, errors.New("JSON содержит несколько документов")
		}
		return nil, fmt.Errorf("лишние данные после JSON-документа: %w", err)
	}

	if doc.Version != formatVersion {
		return nil, fmt.Errorf(
			"неподдерживаемая версия формата: %d",
			doc.Version,
		)
	}

	if doc.HashCapacity <= 0 {
		return nil, errors.New("некорректная вместимость хеш-таблицы")
	}

	data := &Data{
		Array:     structures.NewArray(),
		List:      structures.NewLinkedList(),
		DList:     structures.NewDoubleLinkedList(),
		Queue:     structures.NewQueue(),
		Stack:     structures.NewStack(),
		HashTable: structures.NewHashTable(doc.HashCapacity),
		AVLTree:   structures.NewAVLTree(),
	}

	for _, value := range doc.Array {
		data.Array.PushBack(value)
	}

	for _, value := range doc.List {
		data.List.PushBack(value)
	}

	for _, value := range doc.DoubleList {
		data.DList.PushBack(value)
	}

	for _, value := range doc.Queue {
		data.Queue.Push(value)
	}

	// Stack.Get возвращает элементы от вершины к основанию.
	// Поэтому восстанавливаем стек в обратном порядке.
	for i := len(doc.Stack) - 1; i >= 0; i-- {
		data.Stack.Push(doc.Stack[i])
	}

	seenKeys := make(map[int]bool, len(doc.HashEntries))

	for _, entry := range doc.HashEntries {
		if seenKeys[entry.Key] {
			return nil, fmt.Errorf(
				"повторяющийся ключ хеш-таблицы: %d",
				entry.Key,
			)
		}
		seenKeys[entry.Key] = true

		if err := data.HashTable.Insert(entry.Key, entry.Value); err != nil {
			return nil, fmt.Errorf(
				"ошибка восстановления хеш-таблицы: %w",
				err,
			)
		}
	}

	for _, key := range doc.AVLKeys {
		data.AVLTree.Insert(key)
	}

	return data, nil
}
