// NOTE: Per assignment rules, only this single include is allowed here.
#include "VectorStore.h"

// =====================================
// Helper functions
// =====================================

// Helper function to print n spaces for tree visualization
void printNSpace(int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << " ";
    }
}

// =====================================
// AVLTree<K, T> implementation
// =====================================

template <class K, class T>
void AVLTree<K, T>::printTreeStructure() const
{
    int height = this->getHeight();
    if (this->root == NULL)
    {
        cout << "NULL\n";
        return;
    }
    queue<AVLNode *> q;
    q.push(root);
    AVLNode *temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;
    int space = pow(2, height);
    printNSpace(space / 2);
    while (!q.empty())
    {
        temp = q.front();
        q.pop();
        if (temp == NULL)
        {
            cout << " ";
            q.push(NULL);
            q.push(NULL);
        }
        else
        {
            cout << temp->data;
            q.push(temp->pLeft);
            q.push(temp->pRight);
        }
        printNSpace(space);
        count++;
        if (count == maxNode)
        {
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }
        if (level == height)
            return;
    }
}

// TODO: Implement all AVLTree<K, T> methods here
template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::rotateRight(AVLTree<K, T>::AVLNode *&root)
{
    AVLNode *temp = root->pLeft;
    root->pLeft = temp->pRight;
    temp->pRight = root;
    // root = temp;
    return temp;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::rotateLeft(AVLTree<K, T>::AVLNode *&root)
{
    AVLNode *temp = root->pRight;
    root->pRight = temp->pLeft;
    temp->pLeft = root;
    // root = temp;
    return temp;
}

template <class K, class T>
void AVLTree<K, T>::clearHelper(AVLNode *node)
{
    if (node)
    {
        clearHelper(node->pLeft);
        clearHelper(node->pRight);
        delete node;
    }
}

template <class K, class T>
AVLTree<K, T>::AVLTree()
{
    this->root = nullptr;
    this->count = 0;
}

template <class K, class T>
AVLTree<K, T>::~AVLTree()
{
    clear();
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::insert(AVLTree<K, T>::AVLNode *&root, const K &key, const T &value, bool &taller)
{
    if (!root)
    {
        root = new AVLNode(key, value);
        count++;
        taller = true;
        return root;
    }
    // cout << "Insert (" << key << ", " << value << ")\n";
    // if (key == root->key)
    // {
    //     return root;
    // }
    // else
    if (key == root->key)
    {
        taller = false;
        return root;
    }
    else if (key < root->key)
    {
        root->pLeft = insert(root->pLeft, key, value, taller);
        // cout << "Insert (" << key << ", " << value << ") complete \n";
        // if left subtree is taller
        if (taller)
        {
            if (root->balance == LH)
            {
                // unbalanced, must rebalance
                root = leftBalance(root, taller);
            }
            else if (root->balance == EH)
            {
                root->balance = LH;
            }
            else
            {
                root->balance = EH;
                taller = false;
            }
        }
    }
    else
    {
        root->pRight = insert(root->pRight, key, value, taller);
        // cout << "Insert (" << key << ", " << value << ") complete\n";

        // if right subtree is taller
        if (taller)
        {
            if (root->balance == LH)
            {
                root->balance = EH;
                taller = false;
            }
            else if (root->balance == EH)
            {
                root->balance = RH;
            }
            else
            {
                // unbalanced, must rebalance
                root = rightBalance(root, taller);
            }
        }
    }
    return root;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::leftBalance(AVLTree<K, T>::AVLNode *&root, bool &taller) // taller is true
{
    // cout << "Left balance\n";
    AVLNode *leftTree = root->pLeft;

    // Case 1: Left of Left => right rotate
    if (leftTree->balance == LH)
    {
        root = rotateRight(root);
        root->balance = EH;
        leftTree->balance = EH;
        taller = false;
    }
    else
    // Case 2: Right of Left: LR rotation
    {
        AVLNode *rightTree = leftTree->pRight;
        if (rightTree->balance == EH)
        {
            root->balance = EH;
            leftTree->balance = EH;
        }
        else if (rightTree->balance == LH)
        {
            root->balance = RH;
            leftTree->balance = EH;
        }
        else
        {
            root->balance = EH;
            leftTree->balance = LH;
        }
        rightTree->balance = EH;

        root->pLeft = rotateLeft(leftTree);
        root = rotateRight(root);
        taller = false;
    }
    // cout << "Left balance complete\n";
    return root;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::rightBalance(AVLTree<K, T>::AVLNode *&root, bool &taller) // taller is true
{
    // cout << "Right balance\n";
    AVLNode *rightTree = root->pRight;

    // Case 1: Right of Right => left rotate
    if (rightTree->balance == RH)
    {
        root = rotateLeft(root);
        root->balance = EH;
        rightTree->balance = EH;
        taller = false;
    }
    else
    // Case 2: Left of Right: RL rotation
    {
        AVLNode *leftTree = rightTree->pLeft;
        if (leftTree->balance == EH)
        {
            root->balance = EH;
            rightTree->balance = EH;
        }
        else if (leftTree->balance == RH)
        {
            root->balance = LH;
            rightTree->balance = EH;
        }
        else
        {
            root->balance = EH;
            rightTree->balance = RH;
        }
        leftTree->balance = EH;

        root->pRight = rotateRight(rightTree);
        root = rotateLeft(root);
        taller = false;
    }
    // cout << "Right balance complete\n";
    return root;
}

template <class K, class T>
void AVLTree<K, T>::insert(const K &key, const T &value)
{
    bool taller = false;
    root = insert(root, key, value, taller);
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::remove(AVLTree<K, T>::AVLNode *&root, const K &key, bool &shorter, bool &success)
{
    if (!root)
    {
        shorter = false;
        success = false;
        return root;
    }

    if (key < root->key)
    {
        root->pLeft = remove(root->pLeft, key, shorter, success);
        if (shorter)
        {
            root = deleteRightBalance(root, shorter);
        }
    }
    else if (key > root->key)
    {
        root->pRight = remove(root->pRight, key, shorter, success);
        if (shorter)
        {
            root = deleteLeftBalance(root, shorter);
        }
    }
    else
    {
        // found the node
        AVLNode *deleteNode = root;
        if (!(deleteNode->pRight))
        {
            AVLNode *newRoot = root->pLeft;
            success = true;
            shorter = true;
            delete deleteNode;
            count--;
            return newRoot;
        }
        else if (!(deleteNode->pLeft))
        {
            AVLNode *newRoot = root->pRight;
            success = true;
            shorter = true;
            delete deleteNode;
            count--;
            return newRoot;
        }
        else
        {
            AVLNode *exchPtr = root->pRight;
            while (exchPtr->pLeft != nullptr)
            {
                exchPtr = exchPtr->pLeft;
            }
            root->data = exchPtr->data;
            root->key = exchPtr->key;
            root->pRight = remove(root->pRight, exchPtr->key, shorter, success);
            if (shorter)
            {
                root = deleteLeftBalance(root, shorter);
            }
        }
    }
    return root;
}

// Balance when right subtree is shorter after deleting the right branch
template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::deleteLeftBalance(AVLTree<K, T>::AVLNode *&root, bool &shorter)
{
    if (root->balance == RH)
    {
        root->balance = EH;
    }
    else if (root->balance == EH)
    {
        root->balance = LH;
        shorter = false;
    }
    else
    {
        // imbalance case
        AVLNode *leftTree = root->pLeft;

        // Case 1 : LR
        if (leftTree->balance == RH)
        {
            AVLNode *rightTree = leftTree->pRight;
            if (rightTree->balance == RH)
            {
                leftTree->balance = LH;
                root->balance = EH;
            }
            else if (rightTree->balance == EH)
            {
                root->balance = RH;
                leftTree->balance = EH;
            }
            else
            {
                root->balance = RH;
                leftTree->balance = EH;
            }
            rightTree->balance = EH;
            root->pLeft = rotateLeft(leftTree);
            root = rotateRight(root);
        }
        // Case 2: LL, right rotate
        else
        {
            if (leftTree->balance != EH)
            {
                root->balance = EH;
                leftTree->balance = EH;
            }
            else
            {
                root->balance = LH;
                leftTree->balance = RH;
                shorter = false;
            }
            root = rotateRight(root);
        }
    }
    return root;
}

// Balance when left subtree is shorter after deleting the left branch
template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::deleteRightBalance(AVLTree<K, T>::AVLNode *&root, bool &shorter)
{
    if (root->balance == LH)
    {
        root->balance = EH;
    }
    else if (root->balance == EH)
    {
        root->balance = RH;
        shorter = false;
    }
    else
    {
        // imbalance case
        AVLNode *rightTree = root->pRight;

        // Case 1 : RL
        if (rightTree->balance == LH)
        {
            AVLNode *leftTree = rightTree->pLeft;
            if (leftTree->balance == LH)
            {
                rightTree->balance = RH;
                root->balance = EH;
            }
            else if (leftTree->balance == EH)
            {
                root->balance = LH;
                rightTree->balance = EH;
            }
            else
            {
                root->balance = LH;
                rightTree->balance = EH;
            }
            leftTree->balance = EH;
            root->pRight = rotateRight(rightTree);
            root = rotateLeft(root);
        }
        // Case 2: RR, left rotate
        else
        {
            if (rightTree->balance != EH)
            {
                root->balance = EH;
                rightTree->balance = EH;
            }
            else
            {
                root->balance = RH;
                rightTree->balance = LH;
                shorter = false;
            }
            root = rotateLeft(root);
        }
    }
    return root;
}

template <class K, class T>
void AVLTree<K, T>::remove(const K &key)
{
    bool shorter = false;
    bool success = false;
    this->root = remove(this->root, key, shorter, success);
}

template <class K, class T>
bool AVLTree<K, T>::contains(const K &key) const
{
    if (!root)
        return false;
    AVLNode *current = root;
    while (current)
    {
        if (current->key == key)
        {
            return true;
        }
        else if (key < current->key)
        {
            current = current->pLeft;
        }
        else
        {
            current = current->pRight;
        }
    }
    return false;
}

template <class K, class T>
int AVLTree<K, T>::getHeightHelper(AVLTree<K, T>::AVLNode *root) const
{
    if (!root)
        return 0;
    return max(getHeightHelper(root->pLeft), getHeightHelper(root->pRight)) + 1;
}

template <class K, class T>
int AVLTree<K, T>::getHeight() const
{
    return getHeightHelper(this->root);
}

template <class K, class T>
int AVLTree<K, T>::getSize() const
{
    if (!(this->root))
        return 0;
    queue<AVLNode *> q;
    q.push(root);
    int result = 0;
    while (!q.empty())
    {
        AVLNode *current = q.front();
        q.pop();
        result++;
        if (current->pLeft)
            q.push(current->pLeft);
        if (current->pRight)
            q.push(current->pRight);
    }
    return result;
}

template <class K, class T>
bool AVLTree<K, T>::empty() const
{
    return !root;
}

template <class K, class T>
void AVLTree<K, T>::clear()
{
    clearHelper(this->root);
    this->root = nullptr;
    this->count = 0;
}

template <class K, class T>
void AVLTree<K, T>::inorderHelper(AVLTree<K, T>::AVLNode *root, void (*action)(const T &)) const
{
    if (root)
    {
        inorderHelper(root->pLeft, action);
        action(root->data);
        inorderHelper(root->pRight, action);
    }
}

template <class K, class T>
void AVLTree<K, T>::inorderTraversal(void (*action)(const T &)) const
{
    inorderHelper(this->root, action);
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::getNodeAt(int index)
{
    int count = 0;
    return getNodeAt(root, index, count);
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::getNodeAt(AVLNode *node, int index, int &count)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    AVLNode *leftResult = getNodeAt(node->pLeft, index, count);
    if (leftResult != nullptr)
    {
        return leftResult;
    }

    if (count == index)
    {
        return node;
    }
    count++;
    return getNodeAt(node->pRight, index, count);
}

template <class K, class T>
void AVLTree<K, T>::getNodes(AVLNode *root, vector<AVLNode *> &result)
{
    if (root)
    {
        getNodes(root->pLeft, result);
        result.push_back(root);
        getNodes(root->pRight, result);
    }
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::findMax()
{
    if (!root)
        return nullptr;
    AVLNode *current = root;
    while (current->pRight)
        current = current->pRight;

    return current;
}

template <class K, class T>
int AVLTree<K, T>::size() const
{
    return count;
}

// =====================================
// RedBlackTree<K, T> implementation
// =====================================
template <class K, class T>
RedBlackTree<K, T>::RBTNode::RBTNode(const K &key, const T &value)
{
    this->key = key;
    this->data = value;
    this->color = RED;
    this->left = this->right = this->parent = nullptr;
}

template <class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToRed()
{
    this->color = RED;
}
template <class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToBlack()
{
    this->color = BLACK;
}

template <class K, class T>
void RedBlackTree<K, T>::printTreeStructure() const
{
    if (this->root == nullptr)
    {
        cout << "NULL\n";
        return;
    }

    queue<RBTNode *> q;
    q.push(root);
    RBTNode *temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;

    // Calculate tree height for spacing
    int height = 0;
    queue<RBTNode *> heightQ;
    heightQ.push(root);
    while (!heightQ.empty())
    {
        int size = heightQ.size();
        height++;
        for (int i = 0; i < size; i++)
        {
            RBTNode *node = heightQ.front();
            heightQ.pop();
            if (node->left)
                heightQ.push(node->left);
            if (node->right)
                heightQ.push(node->right);
        }
    }

    int space = pow(2, height);
    printNSpace(space / 2);

    while (!q.empty())
    {
        temp = q.front();
        q.pop();

        if (temp == nullptr)
        {
            cout << "NULL";
            q.push(nullptr);
            q.push(nullptr);
        }
        else
        {
            // Print node data and color as tuple: (data, color)
            cout << "(" << temp->data << ",";
            if (temp->color == RED)
            {
                cout << "R)";
            }
            else
            {
                cout << "B)";
            }

            q.push(temp->left);
            q.push(temp->right);
        }

        printNSpace(space);
        count++;

        if (count == maxNode)
        {
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }

        if (level == height)
        {
            return;
        }
    }
}

// TODO: Implement all other RedBlackTree<K, T> methods here

template <class K, class T>
void RedBlackTree<K, T>::rotateLeft(RBTNode *node)
{
    RBTNode *child = node->right;
    node->right = child->left;
    if (node->right != nullptr)
        (node->right)->parent = node;
    child->parent = node->parent;
    if (node->parent == nullptr)
        root = child;
    else if (node == (node->parent)->left)
        (node->parent)->left = child;
    else
        (node->parent)->right = child;
    child->left = node;
    node->parent = child;
}

template <class K, class T>
void RedBlackTree<K, T>::rotateRight(RBTNode *node)
{
    RBTNode *child = node->left;
    node->left = child->right;
    if (node->left != nullptr)
        (node->left)->parent = node;
    child->parent = node->parent;
    if (node->parent == nullptr)
        root = child;
    else if (node == (node->parent)->right)
        (node->parent)->right = child;
    else
        (node->parent)->left = child;
    child->right = node;
    node->parent = child;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::lowerBoundNode(const K &key) const
{
    RBTNode *node = root;
    while (node != nullptr)
    {
        if (node->key >= key)
            return node;
        else
            node = node->right;
    }
    return nullptr;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::upperBoundNode(const K &key) const
{
    RBTNode *node = root;
    while (node != nullptr)
    {
        if (node->key > key)
            return node;
        else
            node = node->right;
    }
    return nullptr;
}

template <class K, class T>
RedBlackTree<K, T>::RedBlackTree()
{
    this->root = nullptr;
    this->count = 0;
}
template <class K, class T>
RedBlackTree<K, T>::~RedBlackTree()
{
    clear();
}

template <class K, class T>
bool RedBlackTree<K, T>::empty() const
{
    return !root;
}

template <class K, class T>
int RedBlackTree<K, T>::size() const
{
    if (!(this->root))
        return 0;
    queue<RBTNode *> q;
    q.push(root);
    int result = 0;
    while (!q.empty())
    {
        RBTNode *current = q.front();
        q.pop();
        result++;
        if (current->left)
            q.push(current->left);
        if (current->right)
            q.push(current->right);
    }
    return result;
}

template <class K, class T>
void RedBlackTree<K, T>::clear()
{
    clearHelper(this->root);
    this->root = nullptr;
    count = 0;
}

template <class K, class T>
void RedBlackTree<K, T>::clearHelper(RBTNode *node)
{
    if (node)
    {
        clearHelper(node->left);
        clearHelper(node->right);
        delete node;
    }
}

template <class K, class T>
void RedBlackTree<K, T>::insert(const K &key, const T &value)
{
    RBTNode *newNode = new RBTNode(key, value);
    count++;

    RBTNode *parent = nullptr, *current = root;

    while (current != nullptr)
    {
        parent = current;
        if (newNode->key < current->key)
        {
            current = current->left;
        }
        else if (newNode->key > current->key)
        {
            current = current->right;
        }
        else
        {
            // key already exists
            delete newNode;
            count--;
            return;
        }
    }

    newNode->parent = parent;
    if (parent == nullptr)
    {
        root = newNode; // empty tree
    }
    else if (newNode->key < parent->key)
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }

    insertFixup(newNode);
}

template <class K, class T>
void RedBlackTree<K, T>::insertFixup(RBTNode *node)
{
    RBTNode *parent = nullptr, *grandparent = nullptr;
    // Loop until no RED-RED case
    while (node != root && (node->parent)->color == RED)
    {
        parent = node->parent;
        grandparent = parent->parent;

        // Case A: Parent is a LEFT child
        if (parent == grandparent->left)
        {
            RBTNode *uncle = grandparent->right;

            // Case 1: uncle is RED
            // Recolor P U to black, G to red
            if (uncle != nullptr && uncle->color == RED)
            {
                grandparent->recolorToRed();
                parent->recolorToBlack();
                uncle->recolorToBlack();
                node = grandparent;
            }
            // Case 2: uncle is BLACK/nullptr
            else
            {
                // Case 2a: "Triangle" (current is a right child)
                if (node == parent->right)
                {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }

                // Case 2b: "Line" (current is a left child)
                rotateRight(grandparent);
                // swap(parent->color, grandparent->color);
                // auto temp = parent->color;
                // parent->color = grandparent->color;
                // grandparent->color = temp;
                // node = parent;
                parent->recolorToBlack();
                grandparent->recolorToRed();
            }
        }
        // Case B: parent is a RIGHT child
        else
        {
            RBTNode *uncle = grandparent->left;
            // Case 1: Uncle is RED
            if (uncle != nullptr && uncle->color == RED)
            {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                node = grandparent;
            }
            // Case 2: Uncle is BLACK
            else
            {
                // Case 2a: "Triangle" (node is a LEFT child)
                if (node == parent->left)
                {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }
                // Case 2b: "Line" (node is a RIGHT child)
                rotateLeft(grandparent);
                // swap(parent->color, grandparent->color);
                // auto temp = parent->color;
                // parent->color = grandparent->color;
                // grandparent->color = temp;
                // node = parent;
                parent->recolorToBlack();
                grandparent->recolorToRed();
            }
        }
    }
    root->color = BLACK;
}

// Replaces subtree 'u' with 'v'
template <class K, class T>
void RedBlackTree<K, T>::transplant(RBTNode *u, RBTNode *v)
{
    if (u->parent == nullptr)
    {
        root = v;
    }
    else if (u == (u->parent)->left)
    {
        (u->parent)->left = v;
    }
    else
    {
        (u->parent)->right = v;
    }
    if (v != nullptr)
    {
        v->parent = u->parent;
    }
}

// Find min, assumes node is not nullptr
template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::findMax(RBTNode *node)
{
    while (node->right != nullptr)
        node = node->right;
    return node;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::findMin(RBTNode *node) const
{
    while (node->left != nullptr)
        node = node->left;
    return node;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::successor(RBTNode *node) const
{
    // Case 1: has right subtree
    if (node->right != nullptr)
    {
        return findMin(node->right);
    }

    // Case 2: no right subtree
    RBTNode *par = node->parent;
    RBTNode *current = node;
    while (par != nullptr && current == par->right)
    {
        current = par;
        par = par->parent;
    }

    // par is either nullptr (reached root), or current is a left child of par (par is successor)
    return par;
}

template <class K, class T>
vector<typename RedBlackTree<K, T>::RBTNode *> RedBlackTree<K, T>::filter(K min, K max) const
{
    vector<RBTNode *> candidates;
    bool found = false;
    RBTNode *current = lowerBound(min, found);
    RBTNode *end_bound = upperBound(max, found);

    while (current != nullptr && current != end_bound)
    {
        candidates.push_back(current);
        current = successor(current);
    }
    return candidates;
}

template <class K, class T>
void RedBlackTree<K, T>::remove(const K &key)
{
    RBTNode *deleteNode = root;
    while (deleteNode != nullptr)
    {
        if (key < deleteNode->key)
        {
            deleteNode = deleteNode->left;
        }
        else if (key > deleteNode->key)
        {
            deleteNode = deleteNode->right;
        }
        else
        {
            break; // found the key
        }
    }

    if (deleteNode == nullptr)
        return;

    removeNode(deleteNode);
}

template <class K, class T>
void RedBlackTree<K, T>::removeNode(RBTNode *deleteNode)
{
    RBTNode *replacementNode;   // The node to replace the spliced node
    RBTNode *replacementParent; // Parent of the spliced node
    RBTNode *spliceNode = deleteNode;

    Color originalColor = spliceNode->color;

    // Case 1 and 2: 0 or 1 child
    if (deleteNode->left == nullptr)
    {
        replacementNode = deleteNode->right;
        replacementParent = deleteNode->parent;
        transplant(deleteNode, deleteNode->right);
    }
    else if (deleteNode->right == nullptr)
    {
        replacementNode = deleteNode->left;
        replacementParent = deleteNode->parent;
        transplant(deleteNode, deleteNode->left);
    }
    // Case 3: 2 children, use in-order predecessor
    else
    {
        spliceNode = findMax(deleteNode->left);
        originalColor = spliceNode->color;
        replacementNode = spliceNode->left;

        if (spliceNode->parent == deleteNode) // Direct left child
        {
            if (replacementNode != nullptr)
            {
                replacementNode->parent = spliceNode;
            }
            replacementParent = spliceNode;
        }
        else
        {
            // Update parent and child pointers for the spliced node
            replacementParent = spliceNode->parent;
            transplant(spliceNode, spliceNode->left);
            spliceNode->left = deleteNode->left;
            spliceNode->left->parent = spliceNode;
        }
        transplant(deleteNode, spliceNode);
        spliceNode->right = deleteNode->right;
        spliceNode->right->parent = spliceNode;
        spliceNode->color = deleteNode->color;
    }

    delete deleteNode;
    count--;

    // Fix the Red-Black Tree properties if a black node was removed
    if (originalColor == BLACK)
    {
        removeFixup(replacementNode, replacementParent);
    }
}

template <class K, class T>
void RedBlackTree<K, T>::removeFixup(RBTNode *doubleBlackNode, RBTNode *parent)
{
    RBTNode *sibling;
    while (doubleBlackNode != root && (doubleBlackNode == nullptr || doubleBlackNode->color == BLACK))
    {
        if (parent == nullptr)
            break;

        auto nodeColor = [](RBTNode *node) -> Color
        {
            return (node == nullptr) ? BLACK : node->color;
        };

        // Case A: doubleBlack is a LEFT child
        if (doubleBlackNode == parent->left)
        {
            sibling = parent->right;

            // Case 2: Red sibling -> rotate P left, recolor P red, S black
            if (nodeColor(sibling) == RED)
            {
                sibling->recolorToBlack();
                parent->recolorToRed();
                rotateLeft(parent);
                sibling = parent->right; // Get new sibling
            }

            // Sibling is now a BLACK node
            RBTNode *leftSib = sibling ? sibling->left : nullptr;
            RBTNode *rightSib = sibling ? sibling->right : nullptr;

            // Case 3: Both of sibling's children are black -> recolor S to red, push X <- P upwards
            if (nodeColor(leftSib) == BLACK && nodeColor(rightSib) == BLACK)
            {
                sibling->recolorToRed();
                doubleBlackNode = parent;
                parent = doubleBlackNode->parent;
            }
            else
            {
                // Sibling has at least one RED child

                // Case 4: Near child (left) is RED, far child (right) is BLACK -> right rotate S, recolor S red, SL black
                if (nodeColor(leftSib) == RED && nodeColor(rightSib) == BLACK)
                {
                    sibling->recolorToRed();
                    leftSib->recolorToBlack();
                    rotateRight(sibling);
                    sibling = parent->right;
                }
                // Case 5: Far child (right) is RED -> left rotate P, color S with old color P, color P black, color SR black
                sibling->color = parent->color;
                parent->recolorToBlack();
                if (rightSib)
                    rightSib->recolorToBlack();
                rotateLeft(parent);
                doubleBlackNode = root; // Problem resolved
            }
        }
        // Case B: doubleBlack is a RIGHT child
        else
        {
            sibling = parent->left;

            // Case 2: Red sibling -> rotate P right, recolor P red, S black
            if (nodeColor(sibling) == RED)
            {
                sibling->recolorToBlack();
                parent->recolorToRed();
                rotateRight(parent);
                sibling = parent->left; // Get new sibling
            }

            // Sibling is now a BLACK node
            RBTNode *leftSib = sibling ? sibling->left : nullptr;
            RBTNode *rightSib = sibling ? sibling->right : nullptr;

            // Case 3: Both of sibling's children are black -> recolor S to red, push X <- P upwards
            if (nodeColor(leftSib) == BLACK && nodeColor(rightSib) == BLACK)
            {
                sibling->recolorToRed();
                doubleBlackNode = parent;
                parent = doubleBlackNode->parent;
            }
            else
            {
                // Sibling has at least one RED child

                // Case 4: Near child (right) is RED, far child (left) is BLACK -> left rotate S, recolor S red, SR black
                if (nodeColor(rightSib) == RED && nodeColor(leftSib) == BLACK)
                {
                    sibling->recolorToRed();
                    rightSib->recolorToBlack();
                    rotateLeft(sibling);
                    sibling = parent->left;
                }
                // Case 5: Far child (left) is RED -> right rotate P, color S with old color P, color P black, color SL black
                sibling->color = parent->color;
                parent->recolorToBlack();
                if (leftSib)
                    leftSib->recolorToBlack();
                rotateRight(parent);
                doubleBlackNode = root; // Problem resolved
            }
        }
    }
    if (doubleBlackNode)
    {
        doubleBlackNode->recolorToBlack();
    }
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::find(const K &key) const
{
    if (!root)
        return nullptr;

    RBTNode *current = root;
    while (current)
    {
        if (key == current->key)
        {
            return current;
        }
        else if (key < current->key)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return nullptr;
}

template <class K, class T>
bool RedBlackTree<K, T>::contains(const K &key) const
{
    if (!root)
        return false;

    RBTNode *current = root;
    while (current)
    {
        if (key == current->key)
        {
            return true;
        }
        else if (key < current->key)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return false;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::lowerBound(const K &key, bool &found) const
{
    RBTNode *best = nullptr, *current = this->root;
    while (current)
    {
        if (current->key >= key) // valid
        {
            best = current;
            current = current->left; // try a smaller node
        }
        else
        {
            // too low, must go higher
            current = current->right;
        }
    }
    found = (best != nullptr);
    return best;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::upperBound(const K &key, bool &found) const
{
    RBTNode *best = nullptr, *current = this->root;
    while (current)
    {
        if (current->key >= key) // valid
        {
            best = current;
            current = current->left; // try a smaller node
        }
        else
        {
            // too low, must go higher
            current = current->right;
        }
    }
    found = (best != nullptr);
    return best;
}

// =====================================
// VectorRecord implementation
// =====================================

// Overload operator << for VectorRecord - prints only the id
std::ostream &operator<<(std::ostream &os, const VectorRecord &record)
{
    os << record.id;
    return os;
}

// FIXME: Need a way to delete the vector associated in the record

// =====================================
// VectorStore implementation
// =====================================

// TODO: Implement all VectorStore methods here

double VectorStore::norm(const vector<float> &vec) const
{
    double result = 0;
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        result += (double)(*it) * (double)(*it);
    }
    return sqrt(result);
}

VectorStore::VectorStore(int dimension, std::vector<float> *(*embeddingFunction)(const std::string &), const std::vector<float> &referenceVector)
    : dimension(dimension), embeddingFunction(embeddingFunction)
{
    vectorStore = new AVLTree<double, VectorRecord>;
    normIndex = new RedBlackTree<double, VectorRecord>;
    ids = new AVLTree<int, int>;
    this->referenceVector = const_cast<vector<float> *>(&referenceVector);
    rootVector = nullptr;
    count = 0;
    averageDistance = 0;
    maxId = 0;
}

VectorStore::~VectorStore()
{
    // TODO:
    void (*deleteVector)(const VectorRecord &) = [](const VectorRecord &vec) -> void
    { delete vec.vector; };
    vectorStore->inorderTraversal(deleteVector);
    delete vectorStore;
    delete normIndex;
    delete ids;
}

int VectorStore::size()
{
    return count;
}

bool VectorStore::empty()
{
    return count == 0;
}

pair<typename AVLTree<double, VectorRecord>::AVLNode *, int> VectorStore::buildTreeFromArray(vector<AVLTree<double, VectorRecord>::AVLNode *> &array, int start, int end)
{
    if (start > end)
        return {nullptr, 0};

    int mid = start + (end - start) / 2;

    // AVLTree<double, VectorRecord>::AVLNode* node = new AVLTree<double, VectorRecord>::AVLNode(array[mid].distanceFromReference, array[mid]);
    AVLTree<double, VectorRecord>::AVLNode *node = array[mid];
    auto leftData = buildTreeFromArray(array, start, mid - 1);
    auto rightData = buildTreeFromArray(array, mid + 1, end);

    node->pLeft = leftData.first;
    node->pRight = rightData.first;

    int leftH = leftData.second;
    int rightH = rightData.second;
    node->balance = BalanceValue(rightH - leftH);
    return {node, (leftH > rightH ? leftH : rightH) + 1};
}

void VectorStore::rebuildRootIfNeeded()
{
    if (count <= 1)
        return;

    vector<AVLTree<double, VectorRecord>::AVLNode *> nodeList;
    auto avlRoot = vectorStore->getRoot();
    vectorStore->getNodes(avlRoot, nodeList);
    averageDistance = 0;
    for (auto &node : nodeList)
    {
        double distance = l2Distance(*node->data.vector, *referenceVector);
        averageDistance += distance;
        node->key = distance;
        node->data.distanceFromReference = distance;
    }
    averageDistance /= count;
    mergeSort(nodeList, 0, nodeList.size() - 1);
    rebuildRoot(nodeList);
}

void VectorStore::rebuildRoot(vector<AVLTree<double, VectorRecord>::AVLNode *> &nodeList)
{
    double closestToAvg = abs(nodeList[0]->data.distanceFromReference - averageDistance);
    int bestIdx = 0;

    for (int i = 0; i < (int)nodeList.size(); i++)
    {
        double currentToAvg = abs(nodeList[i]->data.distanceFromReference - averageDistance);
        if (currentToAvg < closestToAvg)
        {
            closestToAvg = currentToAvg;
            bestIdx = i;
        }
    }

    vectorStore->root = nodeList[bestIdx];
    auto newRoot = vectorStore->getRoot();
    rootVector = &nodeList[bestIdx]->data;

    auto leftData = buildTreeFromArray(nodeList, 0, bestIdx - 1);
    auto rightData = buildTreeFromArray(nodeList, bestIdx + 1, nodeList.size() - 1);

    newRoot->pLeft = leftData.first;
    newRoot->pRight = rightData.first;
    int leftH = leftData.second;
    int rightH = rightData.second;
    newRoot->balance = BalanceValue(rightH - leftH);
}

void VectorStore::rebuildTreeWithNewRoot(VectorRecord *newRoot)
{
    if (count <= 1)
        return;
    vector<AVLTree<double, VectorRecord>::AVLNode *> nodeList;
    auto avlRoot = vectorStore->getRoot();
    vectorStore->getNodes(avlRoot, nodeList);

    int rootIdx = 0;
    for (int i = 0; i < (int)nodeList.size(); i++)
    {
        if (&nodeList[i]->data == newRoot)
        {
            rootIdx = i;
            break;
        }
    }

    vectorStore->root = nodeList[rootIdx];
    auto root = vectorStore->getRoot();
    rootVector = &nodeList[rootIdx]->data;

    auto leftData = buildTreeFromArray(nodeList, 0, rootIdx - 1);
    auto rightData = buildTreeFromArray(nodeList, rootIdx + 1, nodeList.size() - 1);

    root->pLeft = leftData.first;
    root->pRight = rightData.first;
    int leftH = leftData.second;
    int rightH = rightData.second;
    root->balance = BalanceValue(rightH - leftH);
}

void VectorStore::clear()
{
    void (*deleteRecord)(const VectorRecord &) = [](const VectorRecord &record) -> void
    {
        if (record.vector)
        {
            delete record.vector;
        }
    };
    vectorStore->inorderTraversal(deleteRecord);
    vectorStore->clear();
    normIndex->clear();
    ids->clear();
    rootVector = nullptr;
    count = 0;
    averageDistance = 0;
    maxId = 0;
}

vector<float> *VectorStore::preprocessing(string rawText)
{
    vector<float> *vec = embeddingFunction(rawText);
    if ((int)vec->size() != this->dimension)
    {
        vec->resize(this->dimension, 0);
    }
    return vec;
}

// FIXME: a lot of things are wrong here
void VectorStore::addText(string rawText)
{
    vector<float> *vec = preprocessing(rawText);
    double distanceToRef = l2Distance(*vec, *referenceVector);
    // cout << "Calculated dist\n";
    double normValue = norm(*vec);
    // cout << "Calculated norm\n";
    VectorRecord record(maxId + 1, rawText, vec, distanceToRef, normValue);
    int sizeBefore = vectorStore->size();
    vectorStore->insert(distanceToRef, record);
    int sizeAfter = vectorStore->size();
    // cout << "AVL insetion successfull\n";
    if (sizeBefore == sizeAfter)
    {
        return;
    }
    averageDistance = (averageDistance * (double)count + distanceToRef) / (double)(count + 1);
    // cout << "Updated distance\n";

    if (count == 0)
    {
        rootVector = &vectorStore->root->data;
    }
    vector<AVLTree<double, VectorRecord>::AVLNode *> nodeList;
    auto avlRoot = vectorStore->getRoot();
    rootVector = &avlRoot->data;
    vectorStore->getNodes(avlRoot, nodeList);

    for (auto node : nodeList)
    {
        if (abs(node->data.distanceFromReference - averageDistance) < abs(rootVector->distanceFromReference - averageDistance))
        {
            rebuildRoot(nodeList);
            break;
        }
    }
    normIndex->insert(normValue, record);
    // cout << "Inserted into RBT\n";
    count++;
    maxId++;
    ids->insert(maxId, maxId);
    // cout << "Inserted key\n";
}

void VectorStore::rangeCheck(int index) const
{
    if (index < 0 || index >= count)
    {
        throw out_of_range("Index is invalid!");
    }
}

VectorRecord *VectorStore::getVector(int index)
{
    rangeCheck(index);
    AVLTree<double, VectorRecord>::AVLNode *record = vectorStore->getNodeAt(index);
    return &record->data;
}

string VectorStore::getRawText(int index)
{
    rangeCheck(index);
    AVLTree<double, VectorRecord>::AVLNode *record = vectorStore->getNodeAt(index);
    return record->data.rawText;
}

int VectorStore::getId(int index)
{
    rangeCheck(index);
    AVLTree<double, VectorRecord>::AVLNode *record = vectorStore->getNodeAt(index);
    return record->data.id;
}

bool VectorStore::removeAt(int index)
{
    // TODO:
    rangeCheck(index);
    AVLTree<double, VectorRecord>::AVLNode *record = vectorStore->getNodeAt(index);
    VectorRecord recToRemove = record->data;
    // bool removeRoot = (recToRemove == *rootVector);
    vector<float> *vecToRemove = recToRemove.vector;
    delete vecToRemove;

    double avlKey = record->key;
    double rbtKey = recToRemove.norm;
    int removeId = recToRemove.id;
    averageDistance = (averageDistance * (double)count - avlKey) / (double)(count - 1);

    vectorStore->remove(avlKey);
    normIndex->remove(rbtKey);
    count--;
    if (count == 0) 
    {
        averageDistance = 0;
        rootVector = nullptr;
    }
    ids->remove(removeId);
    if (!ids->empty())
    {
        maxId = ids->findMax()->key;
    }
    else
    {
        maxId = 0;
    }

    if (count <= 1)
    {
        if (count == 0)
            rootVector = nullptr;
        else
            rootVector = &vectorStore->getRoot()->data;
        return true; // nothing to rebalance
    }
    
    vector<AVLTree<double, VectorRecord>::AVLNode *> nodeList;
    auto avlRoot = vectorStore->getRoot();
    rootVector = &avlRoot->data;
    vectorStore->getNodes(avlRoot, nodeList);

    for (auto node : nodeList)
    {
        if (abs(node->data.distanceFromReference - averageDistance) < abs(rootVector->distanceFromReference - averageDistance))
        {
            rebuildRoot(nodeList);
            break;
        }
    }
    return true;
}

void VectorStore::setReferenceVector(const vector<float> &newReference)
{
    // TODO:
    referenceVector = const_cast<vector<float> *>(&newReference);
    rebuildRootIfNeeded();
}

vector<float> *VectorStore::getReferenceVector() const
{
    return referenceVector;
}

VectorRecord *VectorStore::getRootVector() const
{
    return rootVector;
}

double VectorStore::getAverageDistance() const
{
    return averageDistance;
}

void VectorStore::setEmbeddingFunction(vector<float> *(*newEmbeddingFunction)(const string &))
{
    embeddingFunction = newEmbeddingFunction;
}

void VectorStore::forEachAction(AVLTree<double, VectorRecord>::AVLNode *root, void (*action)(vector<float> &, int, string &))
{
    if (root)
    {
        forEachAction(root->pLeft, action);
        action(*root->data.vector, root->data.id, root->data.rawText);
        forEachAction(root->pRight, action);
    }
}

void VectorStore::forEach(void (*action)(vector<float> &, int, string &))
{
    auto *root = vectorStore->getRoot();
    forEachAction(root, action);
}

void VectorStore::getSortedId(AVLTree<double, VectorRecord>::AVLNode *root, vector<int> &ids) const
{
    if (root)
    {
        getSortedId(root->pLeft, ids);
        ids.push_back(root->data.id);
        getSortedId(root->pRight, ids);
    }
}

void VectorStore::getSortedRecords(AVLTree<double, VectorRecord>::AVLNode *root, vector<VectorRecord *> &records) const
{
    if (root)
    {
        getSortedRecords(root->pLeft, records);
        records.push_back(&root->data);
        getSortedRecords(root->pRight, records);
    }
}

vector<int> VectorStore::getAllIdsSortedByDistance() const
{
    vector<int> result;
    auto root = vectorStore->getRoot();
    getSortedId(root, result);
    return result;
}

vector<VectorRecord *> VectorStore::getAllVectorsSortedByDistance() const
{
    vector<VectorRecord *> result;
    auto root = vectorStore->getRoot();
    getSortedRecords(root, result);
    return result;
}

double VectorStore::cosineSimilarity(const vector<float> &v1, const vector<float> &v2)
{
    double prod = 0, normV1 = 0, normV2 = 0;
    for (int i = 0; i < (int)v1.size(); i++)
    {
        double val1 = v1[i], val2 = v2[i];
        prod += val1 * val2;
        normV1 += val1 * val1;
        normV2 += val2 * val2;
    }
    return prod / (sqrt(normV1) * sqrt(normV2));
}

double VectorStore::l1Distance(const vector<float> &v1, const vector<float> &v2)
{
    double result = 0;
    for (int i = 0; i < (int)v1.size(); i++)
    {
        result += abs((double)v1[i] - double(v2[i]));
    }
    return result;
}

double VectorStore::l2Distance(const vector<float> &v1, const vector<float> &v2)
{
    double result = 0;
    for (int i = 0; i < (int)v1.size(); i++)
    {
        double val = (double)v1[i] - double(v2[i]);
        result += pow(val, 2);
    }
    return sqrt(result);
}

double VectorStore::estimateD_Linear(const std::vector<float> &query, int k, double averageDistance,
                                     const std::vector<float> &reference, double c0_bias, double c1_slope)
{
    vector<float> subtraction(query.size());
    for (int i = 0; i < (int)query.size(); i++)
    {
        subtraction[i] = query[i] - reference[i];
    }

    double dr = norm(subtraction);

    double D = abs(dr - averageDistance) + c1_slope * averageDistance * k + c0_bias;
    return D;
}

void VectorStore::metricCheck(string metric) const
{
    if (metric != "cosine" && metric != "manhattan" && metric != "euclidean")
    {
        throw invalid_metric();
    }
}

double VectorStore::distanceByMetric(const std::vector<float> &a, const std::vector<float> &b, const std::string &metric) const
{
    // FIXME:
    metricCheck(metric);
    if (metric == "cosine")
    {
        return const_cast<VectorStore *>(this)->cosineSimilarity(a, b);
    }
    else if (metric == "manhattan")
    {
        return const_cast<VectorStore *>(this)->l1Distance(a, b);
    }
    else
    {
        return const_cast<VectorStore *>(this)->l2Distance(a, b);
    }
}

void VectorStore::findNearestHelper(AVLTree<double, VectorRecord>::AVLNode *root, const std::vector<float> &query, string metric, double &bestDistance, int &bestID)
{
    if (root)
    {
        findNearestHelper(root->pLeft, query, metric, bestDistance, bestID);
        double distance = distanceByMetric(*root->data.vector, query, metric);
        if (distance < bestDistance)
        {
            bestDistance = distance;
            bestID = root->data.id;
        }
        findNearestHelper(root->pRight, query, metric, bestDistance, bestID);
    }
}

int VectorStore::findNearest(const std::vector<float> &query, string metric)
{
    metricCheck(metric);
    // TODO:
    if (count <= 0)
        return -1;
    auto root = vectorStore->getRoot();
    double bestDistance = distanceByMetric(*root->data.vector, query, metric);
    int bestID = root->data.id;
    findNearestHelper(root, query, metric, bestDistance, bestID);
    return bestID;
}

int *VectorStore::topKNearest(const vector<float> &query, int k, string metric)
{
    // TODO:
    metricCheck(metric);
    if (k <= 0 || k > count)
    {
        throw invalid_k_value();
    }
    double normQ = norm(query);
    double D = estimateD_Linear(query, k, averageDistance, *referenceVector);
    auto candidates = normIndex->filter(normQ - D, normQ + D);
    int m = candidates.size();
    if (m < k)
        k = m;
    cout << "Value m: " << m;
    if (m == 0) return nullptr;
    vector<pair<int, double>> distances;
    for (auto candidate : candidates)
    {
        double distance = distanceByMetric(*candidate->data.vector, query, metric);
        if (metric != "cosine")
            distances.push_back({candidate->data.id, distance});
        else
            distances.push_back({candidate->data.id, 1 - distance});
    }

    int *result = new int[k];

    MaxHeap<pair<int, double>> heap([](const pair<int, double> &a, const pair<int, double> &b) -> bool
                                    { return a.second < b.second; });
    for (auto distance : distances)
    {
        if (heap.size() < k)
        {
            heap.push(distance);
        }
        else if (heap.lessThan(distance, heap.peek()))
        {
            heap.pop();
            heap.push(distance);
        }
    }

    int index = 0;
    while (!heap.empty())
    {
        result[index++] = heap.pop().first;
    }

    // Reverse the array
    int left = 0;
    int right = k - 1;
    while (left < right)
    {
        auto temp = result[left];
        result[left] = result[right];
        result[right] = temp;
        left++;
        right--;
    }
    return result;
}

void VectorStore::rangeQueryRootHelper(AVLTree<double, VectorRecord>::AVLNode *root, const double &minDist, const double &maxDist, vector<int> &result) const
{
    if (root)
    {
        if (minDist <= root->key)
            rangeQueryRootHelper(root->pLeft, minDist, maxDist, result);
        if (minDist <= root->key && root->key <= maxDist)
            result.push_back(root->data.id);
        if (root->key <= maxDist)
            rangeQueryRootHelper(root->pRight, minDist, maxDist, result);
    }
}

int *VectorStore::rangeQueryFromRoot(double minDist, double maxDist) const
{
    if (count == 0)
        return nullptr;
    vector<int> result;
    auto avlRoot = vectorStore->getRoot();
    rangeQueryRootHelper(avlRoot, minDist, maxDist, result);
    cout << result.size() << endl;
    if (result.size() == 0) return nullptr;
    int *arr = new int[result.size()];
    for (int i = 0; i < result.size(); i++)
    {
        arr[i] = result[i];
    }
    return arr;
}

void VectorStore::rangeQueryHelper(AVLTree<double, VectorRecord>::AVLNode *root, const vector<float> &query, const double &radius, string metric, vector<pair<int, double>> &result) const
{
    if (root)
    {
        rangeQueryHelper(root->pLeft, query, radius, metric, result);
        double dist = distanceByMetric(*root->data.vector, query, metric);
        if (dist < radius)
        {
            result.push_back({root->data.id, dist});
        }
        rangeQueryHelper(root->pRight, query, radius, metric, result);
    }
}

int *VectorStore::rangeQuery(const vector<float> &query, double radius, std::string metric) const
{
    if (count == 0)
        return nullptr;
    vector<pair<int, double>> results;
    // int size = 0;
    auto avlRoot = vectorStore->getRoot();
    rangeQueryHelper(avlRoot, query, radius, metric, results);
    if (results.size() == 0) return nullptr;

    MaxHeap<pair<int, double>> heap([](const pair<int, double> &a, const pair<int, double> &b) -> bool
                                    { return a.second < b.second; });
    for (auto result : results)
    {
        heap.push(result);
    }

    int *arr = new int[results.size()];
    for (int i = results.size() - 1; i >= 0; i--)
    {
        arr[i] = heap.peek().first;
        heap.pop();
    }
    if (metric == "cosine")
    {
        int left = 0;
        int right = results.size() - 1;
        while (left < right)
        {
            auto temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;
            left++;
            right--;
        }
    }
    return arr;
}

void VectorStore::boundingBoxHelper(AVLTree<double, VectorRecord>::AVLNode *root, const vector<float> &minBound, const vector<float> &maxBound, vector<int> &result) const
{
    if (root)
    {
        boundingBoxHelper(root->pLeft, minBound, maxBound, result);
        vector<float> current = *root->data.vector;
        bool between = true;
        for (int i = 0; i < (int)current.size(); i++)
        {
            if (current[i] <= minBound[i] || current[i] >= maxBound[i])
            {
                between = false;
                break;
            }
        }
        if (between)
        {
            result.push_back(root->data.id);
        }
        boundingBoxHelper(root->pRight, minBound, maxBound, result);
    }
}

int *VectorStore::boundingBoxQuery(const vector<float> &minBound, const vector<float> &maxBound) const
{
    if (count == 0)
        return nullptr;
    vector<int> result;
    // int size = 0;
    auto avlRoot = vectorStore->getRoot();
    boundingBoxHelper(avlRoot, minBound, maxBound, result);
    if (result.size() == 0) return nullptr;
    int *arr = new int[result.size()];
    for (int i = 0; i < result.size(); i++)
    {
        arr[i] = result[i];
    }
    return arr;
}

double VectorStore::getMaxDistance() const
{
    auto current = vectorStore->getRoot();
    while (current->pRight)
    {
        current = current->pRight;
    }
    return current->key;
}

double VectorStore::getMinDistance() const
{
    auto current = vectorStore->getRoot();
    while (current->pLeft)
    {
        current = current->pLeft;
    }
    return current->key;
}

VectorRecord VectorStore::computeCentroid(const vector<VectorRecord *> &records) const
{
    vector<float> *resultVec = new vector<float>;

    int maxDimension = records[0]->vector->size();
    int count = records.size();
    for (int dim = 0; dim < (int)maxDimension; dim++)
    {
        float avg = 0.0;
        for (int i = 0; i < count; i++)
        {
            avg += (*records[i]->vector)[dim];
        }
        avg /= count;
        resultVec->push_back(avg);
    }
    double resultNorm = this->norm(*resultVec);
    double distanceToRef = distanceByMetric(*resultVec, *referenceVector, "euclidean");
    VectorRecord result = VectorRecord(-1, "centroid", resultVec, distanceToRef, resultNorm);
    return result;
}

void VectorStore::findNearestDistanceHelper(AVLTree<double, VectorRecord>::AVLNode *root, double targetDistance, VectorRecord *&best) const
{
    if (root)
    {
        findNearestDistanceHelper(root->pLeft, targetDistance, best);
        if (abs(root->key - targetDistance) < abs(best->distanceFromReference < targetDistance))
        {
            best = &root->data;
        }
        findNearestDistanceHelper(root->pRight, targetDistance, best);
    }
}

VectorRecord *VectorStore::findVectorNearestToDistance(double targetDistance) const
{
    if (count == 0)
        return nullptr;

    auto avlRoot = vectorStore->getRoot();
    VectorRecord *best = &avlRoot->data;
    findNearestDistanceHelper(avlRoot, targetDistance, best);
    return best;
}

// Explicit template instantiation for the type used by VectorStore
template class AVLTree<double, VectorRecord>;
template class AVLTree<double, double>;
template class AVLTree<int, double>;
template class AVLTree<int, int>;
template class AVLTree<double, string>;
template class AVLTree<int, string>;

template class RedBlackTree<double, VectorRecord>;
template class RedBlackTree<double, double>;
template class RedBlackTree<int, double>;
template class RedBlackTree<int, int>;
template class RedBlackTree<double, string>;
template class RedBlackTree<int, string>;
