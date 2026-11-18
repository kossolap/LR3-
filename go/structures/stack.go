package structures

type sNode struct {
	data string
	next *sNode
}

type Stack struct {
	head *sNode
}

func NewStack() *Stack {
	return &Stack{}
}

func (s *Stack) Empty() bool {
	return s.head == nil
}

func (s *Stack) Size() int {
	count := 0
	for current := s.head; current != nil; current = current.next {
		count++
	}
	return count
}

func (s *Stack) Push(value string) {
	s.head = &sNode{data: value, next: s.head}
}

func (s *Stack) Pop() (string, bool) {
	if s.Empty() {
		return "", false
	}

	value := s.head.data
	s.head = s.head.next
	return value, true
}

func (s *Stack) Get() []string {
	result := make([]string, 0, s.Size())
	for current := s.head; current != nil; current = current.next {
		result = append(result, current.data)
	}
	return result
}

func (s *Stack) Clear() {
	s.head = nil
}
