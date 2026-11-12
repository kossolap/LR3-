package structures

type FNode struct {
	Data string
	Next *FNode
}

type LinkedList struct {
	Head *FNode
	Tail *FNode
}

func NewLinkedList() *LinkedList {
	return &LinkedList{}
}

func (l *LinkedList) Empty() bool {
	return l.Head == nil
}

func (l *LinkedList) Size() int {
	count := 0
	for current := l.Head; current != nil; current = current.Next {
		count++
	}
	return count
}

func (l *LinkedList) Clear() {
	l.Head = nil
	l.Tail = nil
}

func (l *LinkedList) PushFront(value string) {
	node := &FNode{Data: value, Next: l.Head}
	l.Head = node

	if l.Tail == nil {
		l.Tail = node
	}
}

func (l *LinkedList) PushBack(value string) {
	node := &FNode{Data: value}

	if l.Tail == nil {
		l.Head = node
		l.Tail = node
		return
	}

	l.Tail.Next = node
	l.Tail = node
}

func (l *LinkedList) PopFront() (string, bool) {
	if l.Empty() {
		return "", false
	}

	value := l.Head.Data
	l.Head = l.Head.Next

	if l.Head == nil {
		l.Tail = nil
	}

	return value, true
}

func (l *LinkedList) PopBack() (string, bool) {
	if l.Empty() {
		return "", false
	}

	value := l.Tail.Data

	if l.Head == l.Tail {
		l.Clear()
		return value, true
	}

	previous := l.Head
	for previous.Next != l.Tail {
		previous = previous.Next
	}

	previous.Next = nil
	l.Tail = previous
	return value, true
}

func (l *LinkedList) Index(index int) *FNode {
	if index < 0 {
		return nil
	}

	current := l.Head
	for i := 0; current != nil && i < index; i++ {
		current = current.Next
	}
	return current
}

func (l *LinkedList) InsertBeforeValue(target, value string) bool {
	if l.Empty() {
		return false
	}

	if l.Head.Data == target {
		l.PushFront(value)
		return true
	}

	previous := l.Head
	current := l.Head.Next

	for current != nil && current.Data != target {
		previous = current
		current = current.Next
	}

	if current == nil {
		return false
	}

	previous.Next = &FNode{Data: value, Next: current}
	return true
}

func (l *LinkedList) InsertAfterValue(target, value string) bool {
	for current := l.Head; current != nil; current = current.Next {
		if current.Data == target {
			node := &FNode{Data: value, Next: current.Next}
			current.Next = node

			if node.Next == nil {
				l.Tail = node
			}
			return true
		}
	}
	return false
}

func (l *LinkedList) RemoveValue(value string) bool {
	if l.Empty() {
		return false
	}

	if l.Head.Data == value {
		_, ok := l.PopFront()
		return ok
	}

	previous := l.Head
	current := l.Head.Next

	for current != nil {
		if current.Data == value {
			if current == l.Tail {
				_, ok := l.PopBack()
				return ok
			}

			previous.Next = current.Next
			return true
		}

		previous = current
		current = current.Next
	}

	return false
}

func (l *LinkedList) RemoveBeforeValue(value string) bool {
	if l.Empty() || l.Head.Data == value {
		return false
	}

	var previousPrevious *FNode
	previous := l.Head
	current := l.Head.Next

	for current != nil && current.Data != value {
		previousPrevious = previous
		previous = current
		current = current.Next
	}

	if current == nil {
		return false
	}

	if previousPrevious == nil {
		_, ok := l.PopFront()
		return ok
	}

	previousPrevious.Next = current
	return true
}

func (l *LinkedList) RemoveAfterValue(value string) bool {
	for current := l.Head; current != nil; current = current.Next {
		if current.Data == value {
			if current.Next == nil {
				return false
			}

			if current.Next == l.Tail {
				_, ok := l.PopBack()
				return ok
			}

			current.Next = current.Next.Next
			return true
		}
	}

	return false
}

func (l *LinkedList) Find(value string) int {
	index := 0
	for current := l.Head; current != nil; current = current.Next {
		if current.Data == value {
			return index
		}
		index++
	}
	return -1
}

func (l *LinkedList) Get() []string {
	result := make([]string, 0, l.Size())
	for current := l.Head; current != nil; current = current.Next {
		result = append(result, current.Data)
	}
	return result
}

func (l *LinkedList) GetAfterValue(value string) []string {
	for current := l.Head; current != nil; current = current.Next {
		if current.Data == value {
			result := make([]string, 0)
			for node := current.Next; node != nil; node = node.Next {
				result = append(result, node.Data)
			}
			return result
		}
	}
	return []string{}
}

func (l *LinkedList) GetBeforeValue(value string) []string {
	result := make([]string, 0)

	for current := l.Head; current != nil && current.Data != value; current = current.Next {
		result = append(result, current.Data)
	}

	if l.Find(value) == -1 {
		return nil
	}
	return result
}
