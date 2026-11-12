package structures

import (
	"errors"
	"math"
	"strconv"
)

var ErrTableFull = errors.New("hash table is full")

type hashEntry struct {
	key   int
	value string
	state uint8
}

const (
	entryEmpty uint8 = iota
	entryOccupied
	entryDeleted
)

type HashTable struct {
	table []hashEntry
	size  int
}

func isPrime(number int) bool {
	if number < 2 {
		return false
	}

	if number == 2 {
		return true
	}

	if number%2 == 0 {
		return false
	}

	for divisor := 3; divisor <= number/divisor; divisor += 2 {
		if number%divisor == 0 {
			return false
		}
	}

	return true
}

func nextPrime(number int) int {
	if number < 2 {
		number = 2
	}

	for !isPrime(number) {
		if number == math.MaxInt {
			panic("hash table capacity overflow")
		}
		number++
	}

	return number
}

func NewHashTable(capacity int) *HashTable {
	if capacity < 2 {
		capacity = 2
	}

	capacity = nextPrime(capacity)
	return &HashTable{
		table: make([]hashEntry, capacity),
	}
}

// foldingHash groups the decimal digits of the absolute key into groups of three.
func foldingHash(key int) uint64 {
	var value uint64

	if key < 0 {
		value = uint64(-(key + 1)) + 1
	} else {
		value = uint64(key)
	}

	var sum uint64
	for value > 0 {
		sum += value % 1000
		value /= 1000
	}

	return sum
}

func (h *HashTable) primaryHash(key int) int {
	return int(foldingHash(key) % uint64(len(h.table)))
}

func (h *HashTable) secondaryHash(key int) int {
	return 1 + int(foldingHash(key)%uint64(len(h.table)-1))
}

func (h *HashTable) findIndex(key int) int {
	start := h.primaryHash(key)
	step := h.secondaryHash(key)
	index := start

	for range len(h.table) {
		entry := h.table[index]

		if entry.state == entryEmpty {
			return -1
		}

		if entry.state == entryOccupied && entry.key == key {
			return index
		}

		index = (index + step) % len(h.table)
	}

	return -1
}

func (h *HashTable) Insert(key int, value string) error {
	if index := h.findIndex(key); index >= 0 {
		h.table[index].value = value
		return nil
	}

	start := h.primaryHash(key)
	step := h.secondaryHash(key)
	index := start
	firstDeleted := -1

	for range len(h.table) {
		entry := h.table[index]

		if entry.state == entryDeleted && firstDeleted == -1 {
			firstDeleted = index
		}

		if entry.state == entryEmpty {
			if firstDeleted >= 0 {
				index = firstDeleted
			}

			h.table[index] = hashEntry{
				key:   key,
				value: value,
				state: entryOccupied,
			}
			h.size++
			return nil
		}

		index = (index + step) % len(h.table)
	}

	if firstDeleted >= 0 {
		h.table[firstDeleted] = hashEntry{
			key:   key,
			value: value,
			state: entryOccupied,
		}
		h.size++
		return nil
	}

	return ErrTableFull
}

func (h *HashTable) Get(key int) (string, bool) {
	index := h.findIndex(key)
	if index < 0 {
		return "", false
	}

	return h.table[index].value, true
}

func (h *HashTable) Contains(key int) bool {
	return h.findIndex(key) >= 0
}

func (h *HashTable) Remove(key int) bool {
	index := h.findIndex(key)
	if index < 0 {
		return false
	}

	h.table[index].value = ""
	h.table[index].state = entryDeleted
	h.size--
	return true
}

func (h *HashTable) Size() int {
	return h.size
}

func (h *HashTable) Capacity() int {
	return len(h.table)
}

func (h *HashTable) Empty() bool {
	return h.size == 0
}

func (h *HashTable) Clear() {
	h.table = make([]hashEntry, len(h.table))
	h.size = 0
}

func (h *HashTable) Entries() map[int]string {
	result := make(map[int]string, h.size)

	for _, entry := range h.table {
		if entry.state == entryOccupied {
			result[entry.key] = entry.value
		}
	}

	return result
}

func (h *HashTable) String() string {
	result := ""
	for key, value := range h.Entries() {
		result += strconv.Itoa(key) + ":" + value + " "
	}
	return result
}
