package structures

type AVLNode struct {
	Key    int
	Left   *AVLNode
	Right  *AVLNode
	Height int
}

type AVLTree struct {
	Root *AVLNode
	size int
}

func NewAVLTree() *AVLTree {
	return &AVLTree{}
}

func nodeHeight(node *AVLNode) int {
	if node == nil {
		return 0
	}
	return node.Height
}

func updateHeight(node *AVLNode) {
	left := nodeHeight(node.Left)
	right := nodeHeight(node.Right)

	if left > right {
		node.Height = left + 1
	} else {
		node.Height = right + 1
	}
}

func balanceFactor(node *AVLNode) int {
	if node == nil {
		return 0
	}
	return nodeHeight(node.Left) - nodeHeight(node.Right)
}

func rotateRight(node *AVLNode) *AVLNode {
	pivot := node.Left
	node.Left = pivot.Right
	pivot.Right = node

	updateHeight(node)
	updateHeight(pivot)
	return pivot
}

func rotateLeft(node *AVLNode) *AVLNode {
	pivot := node.Right
	node.Right = pivot.Left
	pivot.Left = node

	updateHeight(node)
	updateHeight(pivot)
	return pivot
}

func balanceNode(node *AVLNode) *AVLNode {
	if node == nil {
		return nil
	}

	updateHeight(node)
	balance := balanceFactor(node)

	if balance > 1 {
		if balanceFactor(node.Left) < 0 {
			node.Left = rotateLeft(node.Left)
		}
		return rotateRight(node)
	}

	if balance < -1 {
		if balanceFactor(node.Right) > 0 {
			node.Right = rotateRight(node.Right)
		}
		return rotateLeft(node)
	}

	return node
}

func insertNode(node *AVLNode, key int) (*AVLNode, bool) {
	if node == nil {
		return &AVLNode{Key: key, Height: 1}, true
	}

	var inserted bool

	switch {
	case key < node.Key:
		node.Left, inserted = insertNode(node.Left, key)
	case key > node.Key:
		node.Right, inserted = insertNode(node.Right, key)
	default:
		return node, false
	}

	return balanceNode(node), inserted
}

func minNode(node *AVLNode) *AVLNode {
	for node != nil && node.Left != nil {
		node = node.Left
	}
	return node
}

func removeNode(node *AVLNode, key int) (*AVLNode, bool) {
	if node == nil {
		return nil, false
	}

	var removed bool

	switch {
	case key < node.Key:
		node.Left, removed = removeNode(node.Left, key)
	case key > node.Key:
		node.Right, removed = removeNode(node.Right, key)
	default:
		removed = true

		if node.Left == nil {
			return node.Right, true
		}
		if node.Right == nil {
			return node.Left, true
		}

		successor := minNode(node.Right)
		node.Key = successor.Key
		node.Right, _ = removeNode(node.Right, successor.Key)
	}

	if !removed {
		return node, false
	}

	return balanceNode(node), true
}

func (t *AVLTree) Insert(key int) {
	var inserted bool
	t.Root, inserted = insertNode(t.Root, key)
	if inserted {
		t.size++
	}
}

func (t *AVLTree) Remove(key int) bool {
	var removed bool
	t.Root, removed = removeNode(t.Root, key)
	if removed {
		t.size--
	}
	return removed
}

func (t *AVLTree) Find(key int) bool {
	current := t.Root

	for current != nil {
		switch {
		case key < current.Key:
			current = current.Left
		case key > current.Key:
			current = current.Right
		default:
			return true
		}
	}

	return false
}

func (t *AVLTree) Size() int {
	return t.size
}

func (t *AVLTree) Empty() bool {
	return t.Root == nil
}

func (t *AVLTree) Clear() {
	t.Root = nil
	t.size = 0
}

func inorder(node *AVLNode, result *[]int) {
	if node == nil {
		return
	}

	inorder(node.Left, result)
	*result = append(*result, node.Key)
	inorder(node.Right, result)
}

func preorder(node *AVLNode, result *[]int) {
	if node == nil {
		return
	}

	*result = append(*result, node.Key)
	preorder(node.Left, result)
	preorder(node.Right, result)
}

func postorder(node *AVLNode, result *[]int) {
	if node == nil {
		return
	}

	postorder(node.Left, result)
	postorder(node.Right, result)
	*result = append(*result, node.Key)
}

func (t *AVLTree) Inorder() []int {
	result := make([]int, 0, t.size)
	inorder(t.Root, &result)
	return result
}

func (t *AVLTree) Preorder() []int {
	result := make([]int, 0, t.size)
	preorder(t.Root, &result)
	return result
}

func (t *AVLTree) Postorder() []int {
	result := make([]int, 0, t.size)
	postorder(t.Root, &result)
	return result
}
