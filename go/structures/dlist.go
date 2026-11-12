package structures

type DNode struct {
	Data string
	Next *DNode
	Prev *DNode
}

type DoubleLinkedList struct {
	Head *DNode
	Tail *DNode
}

func NewDoubleLinkedList() *DoubleLinkedList {
	return &DoubleLinkedList{}
}

func (d *DoubleLinkedList) Empty() bool {
	return d.Head == nil
}

func (d *DoubleLinkedList) Size() int {
	count := 0
	for current := d.Head; current != nil; current = current.Next {
		count++
	}
	return count
}

func (d *DoubleLinkedList) Clear() {
	d.Head = nil
	d.Tail = nil
}

func (d *DoubleLinkedList) PushFront(value string) {
	node := &DNode{Data: value, Next: d.Head}

	if d.Head == nil {
		d.Tail = node
	} else {
		d.Head.Prev = node
	}

	d.Head = node
}

func (d *DoubleLinkedList) PushBack(value string) {
	node := &DNode{Data: value, Prev: d.Tail}

	if d.Tail == nil {
		d.Head = node
	} else {
		d.Tail.Next = node
	}

	d.Tail = node
}

func (d *DoubleLinkedList) PopFront() (string, bool) {
	if d.Empty() {
		return "", false
	}

	value := d.Head.Data
	d.Head = d.Head.Next

	if d.Head == nil {
		d.Tail = nil
	} else {
		d.Head.Prev = nil
	}

	return value, true
}

func (d *DoubleLinkedList) PopBack() (string, bool) {
	if d.Empty() {
		return "", false
	}

	value := d.Tail.Data
	d.Tail = d.Tail.Prev

	if d.Tail == nil {
		d.Head = nil
	} else {
		d.Tail.Next = nil
	}

	return value, true
}

func (d *DoubleLinkedList) Index(index int) *DNode {
	if index < 0 {
		return nil
	}

	current := d.Head
	for i := 0; current != nil && i < index; i++ {
		current = current.Next
	}
	return current
}

func (d *DoubleLinkedList) InsertBeforeValue(target, value string) bool {
	for current := d.Head; current != nil; current = current.Next {
		if current.Data == target {
			if current == d.Head {
				d.PushFront(value)
				return true
			}

			node := &DNode{
				Data: value,
				Next: current,
				Prev: current.Prev,
			}
			current.Prev.Next = node
			current.Prev = node
			return true
		}
	}
	return false
}

func (d *DoubleLinkedList) InsertAfterValue(target, value string) bool {
	for current := d.Head; current != nil; current = current.Next {
		if current.Data == target {
			if current == d.Tail {
				d.PushBack(value)
				return true
			}

			node := &DNode{
				Data: value,
				Prev: current,
				Next: current.Next,
			}
			current.Next.Prev = node
			current.Next = node
			return true
		}
	}
	return false
}

func (d *DoubleLinkedList) RemoveValue(value string) bool {
	for current := d.Head; current != nil; current = current.Next {
		if current.Data == value {
			if current == d.Head {
				_, ok := d.PopFront()
				return ok
			}
			if current == d.Tail {
				_, ok := d.PopBack()
				return ok
			}

			current.Prev.Next = current.Next
			current.Next.Prev = current.Prev
			return true
		}
	}
	return false
}

func (d *DoubleLinkedList) RemoveBeforeValue(value string) bool {
	for current := d.Head; current != nil; current = current.Next {
		if current.Data == value {
			if current.Prev == nil {
				return false
			}
			_, ok := d.removeNode(current.Prev)
			return ok
		}
	}
	return false
}

func (d *DoubleLinkedList) RemoveAfterValue(value string) bool {
	for current := d.Head; current != nil; current = current.Next {
		if current.Data == value {
			if current.Next == nil {
				return false
			}
			_, ok := d.removeNode(current.Next)
			return ok
		}
	}
	return false
}

func (d *DoubleLinkedList) removeNode(node *DNode) (string, bool) {
	if node == nil {
		return "", false
	}

	if node == d.Head {
		return d.PopFront()
	}
	if node == d.Tail {
		return d.PopBack()
	}

	node.Prev.Next = node.Next
	node.Next.Prev = node.Prev
	return node.Data, true
}

func (d *DoubleLinkedList) Find(value string) int {
	index := 0
	for current := d.Head; current != nil; current = current.Next {
		if current.Data == value {
			return index
		}
		index++
	}
	return -1
}

func (d *DoubleLinkedList) Get() []string {
	result := make([]string, 0, d.Size())
	for current := d.Head; current != nil; current = current.Next {
		result = append(result, current.Data)
	}
	return result
}

func (d *DoubleLinkedList) GetReverse() []string {
	result := make([]string, 0, d.Size())
	for current := d.Tail; current != nil; current = current.Prev {
		result = append(result, current.Data)
	}
	return result
}

func (d *DoubleLinkedList) GetAfterValue(value string) []string {
	for current := d.Head; current != nil; current = current.Next {
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

func (d *DoubleLinkedList) GetBeforeValue(value string) []string {
	result := make([]string, 0)

	for current := d.Head; current != nil && current.Data != value; current = current.Next {
		result = append(result, current.Data)
	}

	if d.Find(value) == -1 {
		return nil
	}
	return result
}
