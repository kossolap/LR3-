package structures

type dNode struct {
	data string
	next *dNode
	prev *dNode
}

type DoubleLinkedList struct {
	head *dNode
	tail *dNode
}

func NewDoubleLinkedList() *DoubleLinkedList {
	return &DoubleLinkedList{}
}

func (d *DoubleLinkedList) Empty() bool {
	return d.head == nil
}

func (d *DoubleLinkedList) Size() int {
	count := 0
	for current := d.head; current != nil; current = current.next {
		count++
	}
	return count
}

func (d *DoubleLinkedList) Clear() {
	d.head = nil
	d.tail = nil
}

func (d *DoubleLinkedList) PushFront(value string) {
	node := &dNode{data: value, next: d.head}

	if d.head == nil {
		d.tail = node
	} else {
		d.head.prev = node
	}

	d.head = node
}

func (d *DoubleLinkedList) PushBack(value string) {
	node := &dNode{data: value, prev: d.tail}

	if d.tail == nil {
		d.head = node
	} else {
		d.tail.next = node
	}

	d.tail = node
}

func (d *DoubleLinkedList) PopFront() (string, bool) {
	if d.Empty() {
		return "", false
	}

	value := d.head.data
	d.head = d.head.next

	if d.head == nil {
		d.tail = nil
	} else {
		d.head.prev = nil
	}

	return value, true
}

func (d *DoubleLinkedList) PopBack() (string, bool) {
	if d.Empty() {
		return "", false
	}

	value := d.tail.data
	d.tail = d.tail.prev

	if d.tail == nil {
		d.head = nil
	} else {
		d.tail.next = nil
	}

	return value, true
}

func (d *DoubleLinkedList) Index(index int) (string, bool) {
	if index < 0 {
		return "", false
	}

	current := d.head
	for i := 0; current != nil && i < index; i++ {
		current = current.next
	}

	if current == nil {
		return "", false
	}
	return current.data, true
}

func (d *DoubleLinkedList) InsertBeforeValue(target, value string) bool {
	for current := d.head; current != nil; current = current.next {
		if current.data == target {
			if current == d.head {
				d.PushFront(value)
				return true
			}

			node := &dNode{
				data: value,
				next: current,
				prev: current.prev,
			}
			current.prev.next = node
			current.prev = node
			return true
		}
	}
	return false
}

func (d *DoubleLinkedList) InsertAfterValue(target, value string) bool {
	for current := d.head; current != nil; current = current.next {
		if current.data == target {
			if current == d.tail {
				d.PushBack(value)
				return true
			}

			node := &dNode{
				data: value,
				prev: current,
				next: current.next,
			}
			current.next.prev = node
			current.next = node
			return true
		}
	}
	return false
}

func (d *DoubleLinkedList) RemoveValue(value string) bool {
	for current := d.head; current != nil; current = current.next {
		if current.data == value {
			if current == d.head {
				_, ok := d.PopFront()
				return ok
			}
			if current == d.tail {
				_, ok := d.PopBack()
				return ok
			}

			current.prev.next = current.next
			current.next.prev = current.prev
			return true
		}
	}
	return false
}

func (d *DoubleLinkedList) RemoveBeforeValue(value string) bool {
	for current := d.head; current != nil; current = current.next {
		if current.data == value {
			if current.prev == nil {
				return false
			}
			_, ok := d.removeNode(current.prev)
			return ok
		}
	}
	return false
}

func (d *DoubleLinkedList) RemoveAfterValue(value string) bool {
	for current := d.head; current != nil; current = current.next {
		if current.data == value {
			if current.next == nil {
				return false
			}
			_, ok := d.removeNode(current.next)
			return ok
		}
	}
	return false
}

func (d *DoubleLinkedList) removeNode(node *dNode) (string, bool) {
	if node == nil {
		return "", false
	}

	if node == d.head {
		return d.PopFront()
	}
	if node == d.tail {
		return d.PopBack()
	}

	node.prev.next = node.next
	node.next.prev = node.prev
	return node.data, true
}

func (d *DoubleLinkedList) Find(value string) int {
	index := 0
	for current := d.head; current != nil; current = current.next {
		if current.data == value {
			return index
		}
		index++
	}
	return -1
}

func (d *DoubleLinkedList) Get() []string {
	result := make([]string, 0, d.Size())
	for current := d.head; current != nil; current = current.next {
		result = append(result, current.data)
	}
	return result
}

func (d *DoubleLinkedList) GetReverse() []string {
	result := make([]string, 0, d.Size())
	for current := d.tail; current != nil; current = current.prev {
		result = append(result, current.data)
	}
	return result
}

func (d *DoubleLinkedList) GetAfterValue(value string) []string {
	for current := d.head; current != nil; current = current.next {
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

func (d *DoubleLinkedList) GetBeforeValue(value string) []string {
	result := make([]string, 0)

	for current := d.head; current != nil && current.data != value; current = current.next {
		result = append(result, current.data)
	}

	if d.Find(value) == -1 {
		return nil
	}
	return result
}
