package structures

type avlNode struct {
	key    int
	left   *avlNode
	right  *avlNode
	height int
}

type AVLTree struct {
	root *avlNode
	size int
}

func NewAVLTree() *AVLTree {
	return &AVLTree{}
}

func nodeHeight(node *avlNode) int {
	if node == nil {
		return 0
	}
	return node.height
}

func updateHeight(node *avlNode) {
	left := nodeHeight(node.left)
	right := nodeHeight(node.right)

	if left > right {
		node.height = left + 1
	} else {
		node.height = right + 1
	}
}

func balanceFactor(node *avlNode) int {
	if node == nil {
		return 0
	}
	return nodeHeight(node.left) - nodeHeight(node.right)
}

func rotateRight(node *avlNode) *avlNode {
	pivot := node.left
	node.left = pivot.right
	pivot.right = node

	updateHeight(node)
	updateHeight(pivot)
	return pivot
}

func rotateLeft(node *avlNode) *avlNode {
	pivot := node.right
	node.right = pivot.left
	pivot.left = node

	updateHeight(node)
	updateHeight(pivot)
	return pivot
}

func balanceNode(node *avlNode) *avlNode {
	if node == nil {
		return nil
	}

	updateHeight(node)
	balance := balanceFactor(node)

	if balance > 1 {
		if balanceFactor(node.left) < 0 {
			node.left = rotateLeft(node.left)
		}
		return rotateRight(node)
	}

	if balance < -1 {
		if balanceFactor(node.right) > 0 {
			node.right = rotateRight(node.right)
		}
		return rotateLeft(node)
	}

	return node
}

func insertNode(node *avlNode, key int) (*avlNode, bool) {
	if node == nil {
		return &avlNode{key: key, height: 1}, true
	}

	var inserted bool

	switch {
	case key < node.key:
		node.left, inserted = insertNode(node.left, key)
	case key > node.key:
		node.right, inserted = insertNode(node.right, key)
	default:
		return node, false
	}

	return balanceNode(node), inserted
}

func minNode(node *avlNode) *avlNode {
	for node != nil && node.left != nil {
		node = node.left
	}
	return node
}

func removeNode(node *avlNode, key int) (*avlNode, bool) {
	if node == nil {
		return nil, false
	}

	var removed bool

	switch {
	case key < node.key:
		node.left, removed = removeNode(node.left, key)
	case key > node.key:
		node.right, removed = removeNode(node.right, key)
	default:
		removed = true

		if node.left == nil {
			return node.right, true
		}
		if node.right == nil {
			return node.left, true
		}

		successor := minNode(node.right)
		node.key = successor.key
		node.right, _ = removeNode(node.right, successor.key)
	}

	if !removed {
		return node, false
	}

	return balanceNode(node), true
}

func (t *AVLTree) Insert(key int) {
	var inserted bool
	t.root, inserted = insertNode(t.root, key)
	if inserted {
		t.size++
	}
}

func (t *AVLTree) Remove(key int) bool {
	var removed bool
	t.root, removed = removeNode(t.root, key)
	if removed {
		t.size--
	}
	return removed
}

func (t *AVLTree) Find(key int) bool {
	current := t.root

	for current != nil {
		switch {
		case key < current.key:
			current = current.left
		case key > current.key:
			current = current.right
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
	return t.root == nil
}

func (t *AVLTree) Clear() {
	t.root = nil
	t.size = 0
}

func inorder(node *avlNode, result *[]int) {
	if node == nil {
		return
	}

	inorder(node.left, result)
	*result = append(*result, node.key)
	inorder(node.right, result)
}

func preorder(node *avlNode, result *[]int) {
	if node == nil {
		return
	}

	*result = append(*result, node.key)
	preorder(node.left, result)
	preorder(node.right, result)
}

func postorder(node *avlNode, result *[]int) {
	if node == nil {
		return
	}

	postorder(node.left, result)
	postorder(node.right, result)
	*result = append(*result, node.key)
}

func (t *AVLTree) Inorder() []int {
	result := make([]int, 0, t.size)
	inorder(t.root, &result)
	return result
}

func (t *AVLTree) Preorder() []int {
	result := make([]int, 0, t.size)
	preorder(t.root, &result)
	return result
}

func (t *AVLTree) Postorder() []int {
	result := make([]int, 0, t.size)
	postorder(t.root, &result)
	return result
}


func (t *AVLTree) Validate() bool {
	count := 0

	var validateNode func(node *avlNode, min, max *int) (int, bool)
	validateNode = func(node *avlNode, min, max *int) (int, bool) {
		if node == nil {
			return 0, true
		}

		if min != nil && node.key <= *min {
			return 0, false
		}
		if max != nil && node.key >= *max {
			return 0, false
		}

		leftHeight, leftOK := validateNode(node.left, min, &node.key)
		if !leftOK {
			return 0, false
		}

		rightHeight, rightOK := validateNode(node.right, &node.key, max)
		if !rightOK {
			return 0, false
		}

		difference := leftHeight - rightHeight
		if difference < -1 || difference > 1 {
			return 0, false
		}

		expectedHeight := leftHeight + 1
		if rightHeight > leftHeight {
			expectedHeight = rightHeight + 1
		}
		if node.height != expectedHeight {
			return 0, false
		}

		count++
		return expectedHeight, true
	}

	_, valid := validateNode(t.root, nil, nil)
	return valid && count == t.size
}
