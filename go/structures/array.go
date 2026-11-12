package structures

type Array struct {
	data []string
}

func NewArray() *Array {
	return &Array{}
}

func (a *Array) PushBack(value string) {
	a.data = append(a.data, value)
}

func (a *Array) Insert(index int, value string) bool {
	if index < 0 || index > len(a.data) {
		return false
	}

	a.data = append(a.data, "")
	copy(a.data[index+1:], a.data[index:])
	a.data[index] = value
	return true
}

func (a *Array) RemoveAt(index int) bool {
	if index < 0 || index >= len(a.data) {
		return false
	}

	copy(a.data[index:], a.data[index+1:])
	a.data = a.data[:len(a.data)-1]
	return true
}

func (a *Array) Set(index int, value string) bool {
	if index < 0 || index >= len(a.data) {
		return false
	}

	a.data[index] = value
	return true
}

func (a *Array) GetAt(index int) (string, bool) {
	if index < 0 || index >= len(a.data) {
		return "", false
	}

	return a.data[index], true
}

func (a *Array) Get() []string {
	return append([]string(nil), a.data...)
}

func (a *Array) Size() int {
	return len(a.data)
}

func (a *Array) Empty() bool {
	return len(a.data) == 0
}

func (a *Array) Clear() {
	a.data = nil
}
