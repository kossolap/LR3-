package structures

import (
	"errors"
	"math"
	"reflect"
	"strings"
	"testing"
)

func TestArray(t *testing.T) {
	a := NewArray()

	if !a.Empty() || a.Size() != 0 {
		t.Fatal("новый массив должен быть пустым")
	}
	if a.Insert(-1, "x") || a.Insert(1, "x") {
		t.Fatal("вставка по недопустимому индексу должна завершаться неудачно")
	}
	if a.RemoveAt(0) || a.Set(0, "x") {
		t.Fatal("операция над пустым массивом должна завершаться неудачно")
	}
	if _, ok := a.GetAt(0); ok {
		t.Fatal("получение элемента пустого массива должно завершаться неудачно")
	}

	a.PushBack("one")
	a.PushBack("three")

	if !a.Insert(1, "two") {
		t.Fatal("не удалось вставить элемент")
	}
	if !reflect.DeepEqual(a.Get(), []string{"one", "two", "three"}) {
		t.Fatalf("неверное содержимое: %v", a.Get())
	}

	if value, ok := a.GetAt(1); !ok || value != "two" {
		t.Fatalf("неверный GetAt: %q, %v", value, ok)
	}
	if !a.Set(1, "TWO") {
		t.Fatal("не удалось изменить элемент")
	}
	if a.Set(-1, "x") || a.Set(10, "x") {
		t.Fatal("Set принял недопустимый индекс")
	}
	if value, ok := a.GetAt(1); !ok || value != "TWO" {
		t.Fatalf("неверное значение после Set: %q, %v", value, ok)
	}

	copyOfData := a.Get()
	copyOfData[0] = "changed"
	if value, _ := a.GetAt(0); value != "one" {
		t.Fatal("Get должен возвращать копию данных")
	}

	if !a.RemoveAt(1) || a.RemoveAt(-1) || a.RemoveAt(10) {
		t.Fatal("неверная работа RemoveAt")
	}
	if !reflect.DeepEqual(a.Get(), []string{"one", "three"}) {
		t.Fatalf("неверное содержимое после удаления: %v", a.Get())
	}

	a.Clear()
	if !a.Empty() || a.Size() != 0 {
		t.Fatal("Clear не очистил массив")
	}
}

func TestLinkedList(t *testing.T) {
	l := NewLinkedList()

	if !l.Empty() || l.Size() != 0 {
		t.Fatal("новый список должен быть пустым")
	}
	if _, ok := l.PopFront(); ok {
		t.Fatal("PopFront пустого списка должен вернуть false")
	}
	if _, ok := l.PopBack(); ok {
		t.Fatal("PopBack пустого списка должен вернуть false")
	}
	if l.Index(0) != nil || l.Find("x") != -1 {
		t.Fatal("неверный результат поиска в пустом списке")
	}
	if l.InsertBeforeValue("missing", "x") ||
		l.InsertAfterValue("missing", "x") ||
		l.RemoveValue("missing") ||
		l.RemoveBeforeValue("missing") ||
		l.RemoveAfterValue("missing") {
		t.Fatal("операция с отсутствующим элементом должна завершиться неудачно")
	}

	l.PushBack("b")
	l.PushFront("a")
	l.PushBack("d")

	if !reflect.DeepEqual(l.Get(), []string{"a", "b", "d"}) {
		t.Fatalf("неверное содержимое: %v", l.Get())
	}
	if l.Size() != 3 || l.Empty() {
		t.Fatal("неверные Size или Empty")
	}
	if l.Index(-1) != nil || l.Index(3) != nil {
		t.Fatal("Index должен возвращать nil для неверного индекса")
	}
	if node := l.Index(1); node == nil || node.Data != "b" {
		t.Fatal("неверный результат Index")
	}
	if l.Find("d") != 2 || l.Find("missing") != -1 {
		t.Fatal("неверный результат Find")
	}

	if !l.InsertBeforeValue("b", "before") {
		t.Fatal("InsertBeforeValue не сработал")
	}
	if !l.InsertAfterValue("b", "after") {
		t.Fatal("InsertAfterValue не сработал")
	}
	if !reflect.DeepEqual(l.Get(), []string{"a", "before", "b", "after", "d"}) {
		t.Fatalf("неверное содержимое после вставок: %v", l.Get())
	}
	if l.InsertBeforeValue("missing", "x") || l.InsertAfterValue("missing", "x") {
		t.Fatal("вставка относительно отсутствующего элемента должна завершиться неудачно")
	}

	if !reflect.DeepEqual(l.GetAfterValue("b"), []string{"after", "d"}) {
		t.Fatalf("неверный GetAfterValue: %v", l.GetAfterValue("b"))
	}
	if !reflect.DeepEqual(l.GetBeforeValue("b"), []string{"a", "before"}) {
		t.Fatalf("неверный GetBeforeValue: %v", l.GetBeforeValue("b"))
	}

	if !l.RemoveBeforeValue("b") {
		t.Fatal("RemoveBeforeValue не сработал")
	}
	if !l.RemoveAfterValue("b") {
		t.Fatal("RemoveAfterValue не сработал")
	}
	if !l.RemoveValue("d") {
		t.Fatal("RemoveValue не сработал")
	}
	if l.RemoveValue("missing") || l.RemoveBeforeValue("a") || l.RemoveAfterValue("b") {
		t.Fatal("удаление отсутствующего или граничного элемента должно завершиться неудачно")
	}

	if value, ok := l.PopFront(); !ok || value != "a" {
		t.Fatalf("неверный PopFront: %q, %v", value, ok)
	}
	if value, ok := l.PopBack(); !ok || value != "b" {
		t.Fatalf("неверный PopBack: %q, %v", value, ok)
	}
	if !l.Empty() {
		t.Fatal("список должен стать пустым")
	}

	l.PushBack("clear")
	l.Clear()
	if !l.Empty() || l.Head != nil || l.Tail != nil {
		t.Fatal("Clear не очистил список")
	}
}

func TestDoubleLinkedList(t *testing.T) {
	d := NewDoubleLinkedList()

	if !d.Empty() || d.Size() != 0 {
		t.Fatal("новый двусвязный список должен быть пустым")
	}
	if _, ok := d.PopFront(); ok {
		t.Fatal("PopFront пустого списка должен вернуть false")
	}
	if _, ok := d.PopBack(); ok {
		t.Fatal("PopBack пустого списка должен вернуть false")
	}
	if d.Index(0) != nil || d.Find("x") != -1 {
		t.Fatal("неверный поиск в пустом списке")
	}
	if d.InsertBeforeValue("missing", "x") ||
		d.InsertAfterValue("missing", "x") ||
		d.RemoveValue("missing") ||
		d.RemoveBeforeValue("missing") ||
		d.RemoveAfterValue("missing") {
		t.Fatal("операция с отсутствующим элементом должна завершиться неудачно")
	}

	d.PushBack("b")
	d.PushFront("a")
	d.PushBack("d")

	if !reflect.DeepEqual(d.Get(), []string{"a", "b", "d"}) {
		t.Fatalf("неверный прямой порядок: %v", d.Get())
	}
	if !reflect.DeepEqual(d.GetReverse(), []string{"d", "b", "a"}) {
		t.Fatalf("неверный обратный порядок: %v", d.GetReverse())
	}
	if d.Size() != 3 || d.Empty() {
		t.Fatal("неверные Size или Empty")
	}
	if d.Index(-1) != nil || d.Index(3) != nil {
		t.Fatal("Index должен возвращать nil для неверного индекса")
	}
	if node := d.Index(1); node == nil || node.Data != "b" {
		t.Fatal("неверный результат Index")
	}
	if d.Find("d") != 2 || d.Find("missing") != -1 {
		t.Fatal("неверный результат Find")
	}

	if !d.InsertBeforeValue("b", "before") ||
		!d.InsertAfterValue("b", "after") {
		t.Fatal("вставка относительно элемента не сработала")
	}
	if !reflect.DeepEqual(d.Get(), []string{"a", "before", "b", "after", "d"}) {
		t.Fatalf("неверное содержимое после вставок: %v", d.Get())
	}
	if !reflect.DeepEqual(d.GetAfterValue("b"), []string{"after", "d"}) {
		t.Fatalf("неверный GetAfterValue: %v", d.GetAfterValue("b"))
	}
	if !reflect.DeepEqual(d.GetBeforeValue("b"), []string{"a", "before"}) {
		t.Fatalf("неверный GetBeforeValue: %v", d.GetBeforeValue("b"))
	}

	if d.InsertBeforeValue("missing", "x") || d.InsertAfterValue("missing", "x") {
		t.Fatal("вставка относительно отсутствующего элемента должна завершиться неудачно")
	}
	if !d.RemoveBeforeValue("b") || !d.RemoveAfterValue("b") {
		t.Fatal("удаление соседнего элемента не сработало")
	}
	if !d.RemoveValue("d") || d.RemoveValue("missing") {
		t.Fatal("неверная работа RemoveValue")
	}
	if d.RemoveBeforeValue("a") || d.RemoveAfterValue("b") {
		t.Fatal("удаление за границей списка должно завершиться неудачно")
	}

	if value, ok := d.PopFront(); !ok || value != "a" {
		t.Fatalf("неверный PopFront: %q, %v", value, ok)
	}
	if value, ok := d.PopBack(); !ok || value != "b" {
		t.Fatalf("неверный PopBack: %q, %v", value, ok)
	}
	if !d.Empty() {
		t.Fatal("список должен быть пустым")
	}

	d.PushBack("clear")
	d.Clear()
	if !d.Empty() || d.Head != nil || d.Tail != nil {
		t.Fatal("Clear не очистил двусвязный список")
	}
}

func TestQueue(t *testing.T) {
	q := NewQueue()

	if !q.Empty() || q.Size() != 0 {
		t.Fatal("новая очередь должна быть пустой")
	}
	if _, ok := q.Pop(); ok {
		t.Fatal("Pop пустой очереди должен вернуть false")
	}

	q.Push("first")
	q.Push("second")
	q.Push("third")

	if !reflect.DeepEqual(q.Get(), []string{"first", "second", "third"}) {
		t.Fatalf("неверное содержимое очереди: %v", q.Get())
	}
	if q.Size() != 3 || q.Empty() {
		t.Fatal("неверные Size или Empty")
	}

	if value, ok := q.Pop(); !ok || value != "first" {
		t.Fatalf("неверный Pop: %q, %v", value, ok)
	}
	if value, ok := q.Pop(); !ok || value != "second" {
		t.Fatalf("неверный Pop: %q, %v", value, ok)
	}
	if value, ok := q.Pop(); !ok || value != "third" {
		t.Fatalf("неверный Pop: %q, %v", value, ok)
	}
	if !q.Empty() {
		t.Fatal("очередь должна стать пустой")
	}

	q.Push("again")
	q.Clear()
	if !q.Empty() || q.Head != nil || q.Tail != nil {
		t.Fatal("Clear не очистил очередь")
	}
}

func TestStack(t *testing.T) {
	s := NewStack()

	if !s.Empty() || s.Size() != 0 {
		t.Fatal("новый стек должен быть пустым")
	}
	if _, ok := s.Pop(); ok {
		t.Fatal("Pop пустого стека должен вернуть false")
	}

	s.Push("first")
	s.Push("second")
	s.Push("third")

	if !reflect.DeepEqual(s.Get(), []string{"third", "second", "first"}) {
		t.Fatalf("неверное содержимое стека: %v", s.Get())
	}
	if s.Size() != 3 || s.Empty() {
		t.Fatal("неверные Size или Empty")
	}

	if value, ok := s.Pop(); !ok || value != "third" {
		t.Fatalf("неверный Pop: %q, %v", value, ok)
	}
	if value, ok := s.Pop(); !ok || value != "second" {
		t.Fatalf("неверный Pop: %q, %v", value, ok)
	}
	if value, ok := s.Pop(); !ok || value != "first" {
		t.Fatalf("неверный Pop: %q, %v", value, ok)
	}
	if !s.Empty() {
		t.Fatal("стек должен стать пустым")
	}

	s.Push("again")
	s.Clear()
	if !s.Empty() || s.Head != nil {
		t.Fatal("Clear не очистил стек")
	}
}

func TestAVLTree(t *testing.T) {
	tree := NewAVLTree()

	if !tree.Empty() || tree.Size() != 0 || tree.Find(10) {
		t.Fatal("новое дерево должно быть пустым")
	}
	if tree.Remove(10) {
		t.Fatal("удаление из пустого дерева должно вернуть false")
	}

	// Последовательность проверяет балансировку и вращения AVL.
	for _, key := range []int{30, 20, 10, 25, 28, 27, 40, 50, 45, 5, 4} {
		tree.Insert(key)
	}
	tree.Insert(30) // повторный ключ не должен увеличивать размер.

	if tree.Size() != 11 {
		t.Fatalf("неверный размер дерева: %d", tree.Size())
	}
	if !tree.Find(30) || !tree.Find(4) || tree.Find(999) {
		t.Fatal("неверная работа Find")
	}

	inorder := tree.Inorder()
	for i := 1; i < len(inorder); i++ {
		if inorder[i-1] >= inorder[i] {
			t.Fatalf("Inorder должен быть строго возрастающим: %v", inorder)
		}
	}
	if len(tree.Preorder()) != tree.Size() || len(tree.Postorder()) != tree.Size() {
		t.Fatal("обходы дерева должны содержать все узлы")
	}

	// Проверка высот и баланса каждого узла.
	var checkBalance func(*AVLNode) int
	checkBalance = func(node *AVLNode) int {
		if node == nil {
			return 0
		}
		leftHeight := checkBalance(node.Left)
		rightHeight := checkBalance(node.Right)
		if difference := leftHeight - rightHeight; difference < -1 || difference > 1 {
			t.Errorf("нарушен AVL-баланс у ключа %d: разница высот %d", node.Key, difference)
		}
		expectedHeight := leftHeight + 1
		if rightHeight > leftHeight {
			expectedHeight = rightHeight + 1
		}
		if node.Height != expectedHeight {
			t.Errorf("неверная высота узла %d: %d вместо %d", node.Key, node.Height, expectedHeight)
		}
		return expectedHeight
	}
	checkBalance(tree.Root)

	if tree.Remove(999) {
		t.Fatal("удаление отсутствующего ключа должно вернуть false")
	}
	for _, key := range []int{4, 5, 20, 30, 40} {
		if !tree.Remove(key) {
			t.Errorf("не удалось удалить ключ %d", key)
		}
		if tree.Find(key) {
			t.Errorf("ключ %d остался в дереве после удаления", key)
		}
		checkBalance(tree.Root)
	}
	if tree.Size() != 6 {
		t.Fatalf("неверный размер после удаления: %d", tree.Size())
	}

	tree.Clear()
	if !tree.Empty() || tree.Size() != 0 || len(tree.Inorder()) != 0 {
		t.Fatal("Clear не очистил дерево")
	}
}

func TestAVLRotations(t *testing.T) {
	tests := []struct {
		name string
		keys []int
		root int
	}{
		{"left-left", []int{30, 20, 10}, 20},
		{"right-right", []int{10, 20, 30}, 20},
		{"left-right", []int{30, 10, 20}, 20},
		{"right-left", []int{10, 30, 20}, 20},
	}

	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			tree := NewAVLTree()
			for _, key := range test.keys {
				tree.Insert(key)
			}
			if tree.Root == nil || tree.Root.Key != test.root {
				t.Fatalf("ожидался корень %d, получено %v", test.root, tree.Root)
			}
		})
	}
}

func TestHashTable(t *testing.T) {
	table := NewHashTable(5)

	if table.Empty() == false || table.Size() != 0 {
		t.Fatal("новая хеш-таблица должна быть пустой")
	}
	if table.Capacity() < 5 || !isPrime(table.Capacity()) {
		t.Fatalf("ёмкость должна быть простым числом не меньше 5: %d", table.Capacity())
	}

	if err := table.Insert(10, "ten"); err != nil {
		t.Fatalf("Insert завершился ошибкой: %v", err)
	}
	if err := table.Insert(20, "twenty"); err != nil {
		t.Fatalf("Insert завершился ошибкой: %v", err)
	}
	if err := table.Insert(10, "TEN"); err != nil {
		t.Fatalf("обновление ключа завершилось ошибкой: %v", err)
	}
	if table.Size() != 2 {
		t.Fatalf("обновление существующего ключа изменило размер: %d", table.Size())
	}
	if value, ok := table.Get(10); !ok || value != "TEN" {
		t.Fatalf("неверный Get: %q, %v", value, ok)
	}
	if !table.Contains(20) || table.Contains(999) {
		t.Fatal("неверная работа Contains")
	}
	if _, ok := table.Get(999); ok {
		t.Fatal("Get вернул значение для отсутствующего ключа")
	}

	entries := table.Entries()
	if len(entries) != 2 || entries[10] != "TEN" || entries[20] != "twenty" {
		t.Fatalf("неверный Entries: %v", entries)
	}
	if !strings.Contains(table.String(), "10:TEN") {
		t.Fatalf("String не содержит ожидаемую запись: %s", table.String())
	}

	if !table.Remove(10) || table.Remove(10) {
		t.Fatal("неверная работа Remove")
	}
	if table.Contains(10) {
		t.Fatal("удалённый ключ всё ещё содержится в таблице")
	}
	if err := table.Insert(30, "thirty"); err != nil {
		t.Fatalf("вставка после удаления завершилась ошибкой: %v", err)
	}
	if value, ok := table.Get(20); !ok || value != "twenty" {
		t.Fatal("удаление элемента нарушило поиск другого ключа")
	}

	table.Clear()
	if !table.Empty() || table.Size() != 0 || len(table.Entries()) != 0 {
		t.Fatal("Clear не очистил хеш-таблицу")
	}
}

func TestHashTableCollisionsAndFull(t *testing.T) {
	table := NewHashTable(3)
	capacity := table.Capacity()

	// Подбираем ключи с одинаковым начальным индексом.
	keys := make([]int, 0, capacity)
	targetIndex := table.primaryHash(0)
	for key := 0; len(keys) < capacity && key < 100000; key++ {
		if table.primaryHash(key) == targetIndex {
			keys = append(keys, key)
		}
	}
	if len(keys) != capacity {
		t.Fatal("не удалось подобрать набор коллидирующих ключей")
	}

	for _, key := range keys {
		if err := table.Insert(key, "value"); err != nil {
			t.Fatalf("ошибка вставки ключа %d: %v", key, err)
		}
	}
	if table.Size() != capacity {
		t.Fatalf("таблица должна быть заполнена: размер %d, ёмкость %d", table.Size(), capacity)
	}
	if err := table.Insert(1000001, "overflow"); !errors.Is(err, ErrTableFull) {
		t.Fatalf("ожидалась ErrTableFull, получено: %v", err)
	}

	// Удаление оставляет tombstone; поиск и повторная вставка должны работать.
	if !table.Remove(keys[0]) {
		t.Fatal("не удалось удалить ключ из заполненной таблицы")
	}
	if value, ok := table.Get(keys[len(keys)-1]); !ok || value != "value" {
		t.Fatal("поиск после удаления коллидирующего ключа нарушен")
	}
	if err := table.Insert(1000001, "reused"); err != nil {
		t.Fatalf("не удалось использовать удалённую ячейку: %v", err)
	}
	if value, ok := table.Get(1000001); !ok || value != "reused" {
		t.Fatal("неверное значение после повторной вставки")
	}
}

func TestHashTableNegativeAndExtremeKeys(t *testing.T) {
	table := NewHashTable(7)
	keys := []int{-1, -1234567, math.MinInt, math.MaxInt, 0}

	for _, key := range keys {
		if err := table.Insert(key, "value"); err != nil {
			t.Fatalf("ошибка вставки ключа %d: %v", key, err)
		}
	}
	for _, key := range keys {
		if value, ok := table.Get(key); !ok || value != "value" {
			t.Errorf("не найден ключ %d", key)
		}
	}
	for _, key := range keys {
		if !table.Remove(key) {
			t.Errorf("не удалось удалить ключ %d", key)
		}
	}
	if !table.Empty() {
		t.Fatal("таблица должна быть пустой после удаления всех ключей")
	}
}

func TestHashHelpers(t *testing.T) {
	if !isPrime(2) || !isPrime(3) || !isPrime(97) {
		t.Fatal("isPrime не распознал простые числа")
	}
	if isPrime(-1) || isPrime(0) || isPrime(1) || isPrime(9) || isPrime(100) {
		t.Fatal("isPrime ошибочно распознал составное число")
	}
	if nextPrime(2) != 2 || nextPrime(3) != 3 || !isPrime(nextPrime(10)) {
		t.Fatal("неверная работа nextPrime")
	}

	if foldingHash(0) != 0 {
		t.Fatal("неверный foldingHash для нуля")
	}
	if foldingHash(123456) != 579 {
		t.Fatal("foldingHash должен быть детерминированным")
	}

	table := NewHashTable(11)
	if table.primaryHash(-123) < 0 || table.primaryHash(-123) >= table.Capacity() {
		t.Fatal("primaryHash вернул индекс вне таблицы")
	}
	step := table.secondaryHash(-123)
	if step <= 0 || step >= table.Capacity() {
		t.Fatalf("неверный шаг вторичного хеширования: %d", step)
	}
}
