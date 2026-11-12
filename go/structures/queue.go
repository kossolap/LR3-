package structures

type QNode struct {
	Data string
	Next *QNode
}

type Queue struct {
	Head *QNode
	Tail *QNode
}

func NewQueue() *Queue {
	return &Queue{}
}

func (q *Queue) Empty() bool {
	return q.Head == nil
}

func (q *Queue) Size() int {
	count := 0
	for current := q.Head; current != nil; current = current.Next {
		count++
	}
	return count
}

func (q *Queue) Push(value string) {
	node := &QNode{Data: value}

	if q.Tail == nil {
		q.Head = node
		q.Tail = node
		return
	}

	q.Tail.Next = node
	q.Tail = node
}

func (q *Queue) Pop() (string, bool) {
	if q.Empty() {
		return "", false
	}

	value := q.Head.Data
	q.Head = q.Head.Next

	if q.Head == nil {
		q.Tail = nil
	}

	return value, true
}

func (q *Queue) Get() []string {
	result := make([]string, 0, q.Size())
	for current := q.Head; current != nil; current = current.Next {
		result = append(result, current.Data)
	}
	return result
}

func (q *Queue) Clear() {
	q.Head = nil
	q.Tail = nil
}
