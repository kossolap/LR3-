package structures

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestArrayTestify(t *testing.T) {
	a := NewArray()
	assert.True(t, a.Empty())

	a.PushBack("one")
	a.PushBack("three")
	assert.True(t, a.Insert(1, "two"))
	assert.Equal(t, []string{"one", "two", "three"}, a.Get())

	value, ok := a.GetAt(1)
	assert.True(t, ok)
	assert.Equal(t, "two", value)

	assert.True(t, a.Set(1, "changed"))
	assert.True(t, a.RemoveAt(0))
	assert.Equal(t, []string{"changed", "three"}, a.Get())

	assert.False(t, a.Insert(100, "bad"))
	assert.False(t, a.RemoveAt(100))

	a.Clear()
	assert.True(t, a.Empty())
}

func TestLinkedListTestify(t *testing.T) {
	list := NewLinkedList()

	list.PushBack("one")
	list.PushBack("three")
	assert.True(t, list.InsertBeforeValue("three", "two"))
	assert.True(t, list.InsertAfterValue("three", "four"))

	assert.Equal(t, []string{"one", "two", "three", "four"}, list.Get())
	assert.Equal(t, 2, list.Find("three"))
	assert.Equal(t, []string{"one", "two"}, list.GetBeforeValue("three"))
	assert.Equal(t, []string{"four"}, list.GetAfterValue("three"))

	assert.True(t, list.RemoveValue("two"))
	assert.True(t, list.RemoveBeforeValue("four"))
	assert.Equal(t, []string{"one", "four"}, list.Get())

	value, ok := list.PopFront()
	assert.True(t, ok)
	assert.Equal(t, "one", value)

	list.Clear()
	assert.True(t, list.Empty())
}

func TestDoubleLinkedListTestify(t *testing.T) {
	list := NewDoubleLinkedList()

	list.PushBack("one")
	list.PushBack("three")
	assert.True(t, list.InsertBeforeValue("three", "two"))
	assert.True(t, list.InsertAfterValue("three", "four"))

	assert.Equal(t, []string{"one", "two", "three", "four"}, list.Get())
	assert.Equal(t, []string{"four", "three", "two", "one"}, list.GetReverse())

	assert.True(t, list.RemoveValue("two"))
	assert.True(t, list.RemoveAfterValue("three"))
	assert.Equal(t, []string{"one", "three"}, list.Get())

	value, ok := list.PopBack()
	assert.True(t, ok)
	assert.Equal(t, "three", value)

	list.Clear()
	assert.True(t, list.Empty())
}

func TestQueueTestify(t *testing.T) {
	q := NewQueue()
	assert.True(t, q.Empty())

	q.Push("first")
	q.Push("second")
	assert.Equal(t, []string{"first", "second"}, q.Get())

	value, ok := q.Pop()
	assert.True(t, ok)
	assert.Equal(t, "first", value)

	assert.Equal(t, 1, q.Size())

	q.Clear()
	assert.True(t, q.Empty())

	_, ok = q.Pop()
	assert.False(t, ok)
}

func TestStackTestify(t *testing.T) {
	s := NewStack()
	assert.True(t, s.Empty())

	s.Push("first")
	s.Push("second")
	assert.Equal(t, []string{"second", "first"}, s.Get())

	value, ok := s.Pop()
	assert.True(t, ok)
	assert.Equal(t, "second", value)

	assert.Equal(t, 1, s.Size())

	s.Clear()
	assert.True(t, s.Empty())

	_, ok = s.Pop()
	assert.False(t, ok)
}

func TestHashTableTestify(t *testing.T) {
	h := NewHashTable(5)

	assert.NoError(t, h.Insert(10, "ten"))
	assert.NoError(t, h.Insert(20, "twenty"))
	assert.NoError(t, h.Insert(30, "thirty"))

	value, ok := h.Get(20)
	assert.True(t, ok)
	assert.Equal(t, "twenty", value)

	assert.True(t, h.Contains(10))
	assert.False(t, h.Contains(99))

	assert.NoError(t, h.Insert(20, "updated"))
	value, ok = h.Get(20)
	assert.True(t, ok)
	assert.Equal(t, "updated", value)

	assert.True(t, h.Remove(10))
	assert.False(t, h.Contains(10))
	assert.False(t, h.Remove(99))

	assert.Equal(t, 2, h.Size())

	h.Clear()
	assert.True(t, h.Empty())
}

func TestAVLTreeTestify(t *testing.T) {
	tree := NewAVLTree()
	assert.True(t, tree.Empty())

	for _, key := range []int{5, 3, 8, 1, 4, 7, 9} {
		tree.Insert(key)
	}

	assert.False(t, tree.Empty())
	assert.Equal(t, 7, tree.Size())
	assert.True(t, tree.Find(4))
	assert.False(t, tree.Find(100))

	assert.Equal(t, []int{1, 3, 4, 5, 7, 8, 9}, tree.Inorder())

	assert.True(t, tree.Remove(5))
	assert.False(t, tree.Find(5))
	assert.False(t, tree.Remove(100))

	tree.Clear()
	assert.True(t, tree.Empty())
	assert.Equal(t, 0, tree.Size())
}
