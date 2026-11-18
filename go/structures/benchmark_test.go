package structures

import (
	"fmt"
	"testing"
)

var (
	benchmarkSink       any
	benchmarkStringSink string
)

func BenchmarkArrayPushBack(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		a := NewArray()
		for j := 0; j < 100; j++ {
			a.PushBack("value")
		}
		benchmarkSink = a
	}
}

func BenchmarkLinkedListPushBack(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		list := NewLinkedList()
		for j := 0; j < 100; j++ {
			list.PushBack("value")
		}
		benchmarkSink = list
	}
}

func BenchmarkDoubleLinkedListPushBack(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		list := NewDoubleLinkedList()
		for j := 0; j < 100; j++ {
			list.PushBack("value")
		}
		benchmarkSink = list
	}
}

func BenchmarkQueuePush(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		queue := NewQueue()
		for j := 0; j < 100; j++ {
			queue.Push("value")
		}
		benchmarkSink = queue
	}
}

func BenchmarkStackPush(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		stack := NewStack()
		for j := 0; j < 100; j++ {
			stack.Push("value")
		}
		benchmarkSink = stack
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
		benchmarkSink = table
	}
}

func BenchmarkAVLTreeInsert(b *testing.B) {
	b.ReportAllocs()
	for i := 0; i < b.N; i++ {
		tree := NewAVLTree()
		for j := 0; j < 100; j++ {
			tree.Insert(j)
		}
		benchmarkSink = tree
	}
}

func BenchmarkArrayGet(b *testing.B) {
	a := NewArray()
	for i := 0; i < 1000; i++ {
		a.PushBack(fmt.Sprintf("value-%d", i))
	}

	b.ReportAllocs()
	b.ResetTimer()

	for i := 0; i < b.N; i++ {
		value, ok := a.GetAt(i % 1000)
		if !ok {
			b.Fatal("GetAt returned false for a valid index")
		}
		benchmarkStringSink = value
	}
}
