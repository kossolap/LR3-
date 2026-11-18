package structures

type fNode struct {
	data string
	next *fNode
}

type LinkedList struct {
	head *fNode
	tail *fNode
}

func NewLinkedList() *LinkedList {
	return &LinkedList{}
}

func (l *LinkedList) Empty() bool {
	return l.head == nil
}

func (l *LinkedList) Size() int {
	count := 0
	for current := l.head; current != nil; current = current.next {
		count++
	}
	return count
}

func (l *LinkedList) Clear() {
	l.head = nil
	l.tail = nil
}

func (l *LinkedList) PushFront(value string) {
	node := &fNode{data: value, next: l.head}
	l.head = node

	if l.tail == nil {
		l.tail = node
	}
}

func (l *LinkedList) PushBack(value string) {
	node := &fNode{data: value}

	if l.tail == nil {
		l.head = node
		l.tail = node
		return
	}

	l.tail.next = node
	l.tail = node
}

func (l *LinkedList) PopFront() (string, bool) {
	if l.Empty() {
		return "", false
	}

	value := l.head.data
	l.head = l.head.next

	if l.head == nil {
		l.tail = nil
	}

	return value, true
}

func (l *LinkedList) PopBack() (string, bool) {
	if l.Empty() {
		return "", false
	}

	value := l.tail.data

	if l.head == l.tail {
		l.Clear()
		return value, true
	}

	previous := l.head
	for previous.next != l.tail {
		previous = previous.next
	}

	previous.next = nil
	l.tail = previous
	return value, true
}

func (l *LinkedList) Index(index int) (string, bool) {
	if index < 0 {
		return "", false
	}

	current := l.head
	for i := 0; current != nil && i < index; i++ {
		current = current.next
	}

	if current == nil {
		return "", false
	}
	return current.data, true
}

func (l *LinkedList) InsertBeforeValue(target, value string) bool {
	if l.Empty() {
		return false
	}

	if l.head.data == target {
		l.PushFront(value)
		return true
	}

	previous := l.head
	current := l.head.next

	for current != nil && current.data != target {
		previous = current
		current = current.next
	}

	if current == nil {
		return false
	}

	previous.next = &fNode{data: value, next: current}
	return true
}

func (l *LinkedList) InsertAfterValue(target, value string) bool {
	for current := l.head; current != nil; current = current.next {
		if current.data == target {
			node := &fNode{data: value, next: current.next}
			current.next = node

			if node.next == nil {
				l.tail = node
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

	if l.head.data == value {
		_, ok := l.PopFront()
		return ok
	}

	previous := l.head
	current := l.head.next

	for current != nil {
		if current.data == value {
			if current == l.tail {
				_, ok := l.PopBack()
				return ok
			}

			previous.next = current.next
			return true
		}

		previous = current
		current = current.next
	}

	return false
}

func (l *LinkedList) RemoveBeforeValue(value string) bool {
	if l.Empty() || l.head.data == value {
		return false
	}

	var previousPrevious *fNode
	previous := l.head
	current := l.head.next

	for current != nil && current.data != value {
		previousPrevious = previous
		previous = current
		current = current.next
	}

	if current == nil {
		return false
	}

	if previousPrevious == nil {
		_, ok := l.PopFront()
		return ok
	}

	previousPrevious.next = current
	return true
}

func (l *LinkedList) RemoveAfterValue(value string) bool {
	for current := l.head; current != nil; current = current.next {
		if current.data == value {
			if current.next == nil {
				return false
			}

			if current.next == l.tail {
				_, ok := l.PopBack()
				return ok
			}

			current.next = current.next.next
			return true
		}
	}

	return false
}

func (l *LinkedList) Find(value string) int {
	index := 0
	for current := l.head; current != nil; current = current.next {
		if current.data == value {
			return index
		}
		index++
	}
	return -1
}

func (l *LinkedList) Get() []string {
	result := make([]string, 0, l.Size())
	for current := l.head; current != nil; current = current.next {
		result = append(result, current.data)
	}
	return result
}

func (l *LinkedList) GetAfterValue(value string) []string {
	for current := l.head; current != nil; current = current.next {
		if current.data == value {
			result := make([]string, 0)
			for node := current.next; node != nil; node = node.next {
				result = append(result, node.data)
			}
			return result
		}
	}
	return []string{}
}

func (l *LinkedList) GetBeforeValue(value string) []string {
	result := make([]string, 0)

	for current := l.head; current != nil && current.data != value; current = current.next {
		result = append(result, current.data)
	}

	if l.Find(value) == -1 {
		return nil
	}
	return result
}
