package structures

type SNode struct {
	Data string
	Next *SNode
}

type Stack struct {
	Head *SNode
}

func NewStack() *Stack {
	return &Stack{}
}

func (s *Stack) Empty() bool {
	return s.Head == nil
}

func (s *Stack) Size() int {
	count := 0
	for current := s.Head; current != nil; current = current.Next {
		count++
	}
	return count
}

func (s *Stack) Push(value string) {
	s.Head = &SNode{Data: value, Next: s.Head}
}

func (s *Stack) Pop() (string, bool) {
	if s.Empty() {
		return "", false
	}

	value := s.Head.Data
	s.Head = s.Head.Next
	return value, true
}

func (s *Stack) Get() []string {
	result := make([]string, 0, s.Size())
	for current := s.Head; current != nil; current = current.Next {
		result = append(result, current.Data)
	}
	return result
}

func (s *Stack) Clear() {
	s.Head = nil
}
