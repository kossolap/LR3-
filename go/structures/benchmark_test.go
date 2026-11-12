package structures

import (
	"fmt"
	"testing"
)

func BenchmarkArrayPushBack(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		a := NewArray()
		for j := 0; j < 100; j++ {
			a.PushBack("value")
		}
	}
}

func BenchmarkLinkedListPushBack(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		list := NewLinkedList()
		for j := 0; j < 100; j++ {
			list.PushBack("value")
		}
	}
}

func BenchmarkDoubleLinkedListPushBack(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		list := NewDoubleLinkedList()
		for j := 0; j < 100; j++ {
			list.PushBack("value")
		}
	}
}

func BenchmarkQueuePush(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		queue := NewQueue()
		for j := 0; j < 100; j++ {
			queue.Push("value")
		}
	}
}

func BenchmarkStackPush(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		stack := NewStack()
		for j := 0; j < 100; j++ {
			stack.Push("value")
		}
	}
}

func BenchmarkHashTableInsert(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		table := NewHashTable(101)
		for j := 0; j < 100; j++ {
			if err := table.Insert(j, "value"); err != nil {
				b.Fatal(err)
			}
		}
	}
}

func BenchmarkAVLTreeInsert(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		tree := NewAVLTree()
		for j := 0; j < 100; j++ {
			tree.Insert(j)
		}
	}
}

func BenchmarkArrayGet(b *testing.B) {
	a := NewArray()
	for i := 0; i < 1000; i++ {
		a.PushBack(fmt.Sprintf("value-%d", i))
	}
	b.ResetTimer()
	b.ReportAllocs()

	for i := 0; i < b.N; i++ {
		_, _ = a.GetAt(i % 1000)
	}
}
