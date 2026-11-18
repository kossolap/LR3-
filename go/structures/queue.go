package structures

type qNode struct {
	data string
	next *qNode
}

type Queue struct {
	head *qNode
	tail *qNode
}

func NewQueue() *Queue {
	return &Queue{}
}

func (q *Queue) Empty() bool {
	return q.head == nil
}

func (q *Queue) Size() int {
	count := 0
	for current := q.head; current != nil; current = current.next {
		count++
	}
	return count
}

func (q *Queue) Push(value string) {
	node := &qNode{data: value}

	if q.tail == nil {
		q.head = node
		q.tail = node
		return
	}

	q.tail.next = node
	q.tail = node
}

func (q *Queue) Pop() (string, bool) {
	if q.Empty() {
		return "", false
	}

	value := q.head.data
	q.head = q.head.next

	if q.head == nil {
		q.tail = nil
	}

	return value, true
}

func (q *Queue) Get() []string {
	result := make([]string, 0, q.Size())
	for current := q.head; current != nil; current = current.next {
		result = append(result, current.data)
	}
	return result
}

func (q *Queue) Clear() {
	q.head = nil
	q.tail = nil
}
