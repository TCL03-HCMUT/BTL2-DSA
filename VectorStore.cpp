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
    root = temp;
    return root;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::rotateLeft(AVLTree<K, T>::AVLNode *&root)
{
    AVLNode *temp = root->pRight;
    root->pRight = temp->pLeft;
    temp->pLeft = root;
    root = temp;
    return root;
}

template <class K, class T>
void AVLTree<K, T>::clearHelper(AVLTree<K, T>::AVLNode *node)
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
    if (key < root->key)
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
        (root->pRight)->balance = EH;
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
        (root->pLeft)->balance = EH;
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
            return newRoot;
        }
        else if (!(deleteNode->pLeft))
        {
            AVLNode *newRoot = root->pRight;
            success = true;
            shorter = true;
            delete deleteNode;
            return newRoot;
        }
        else
        {
            AVLNode* exchPtr = root->pRight;
            while(exchPtr->pLeft != nullptr){
                exchPtr = exchPtr->pLeft;
            }
            root->data =exchPtr->data;
            root->pRight = remove(root->pRight,exchPtr->key, shorter, success);
            if(shorter)
            {
                root = deleteLeftBalance(root, shorter);
            }
        }
    }
    return root;
}

// Balance when right subtree is shorter after deleting the right branch
template<class K, class T>
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
        if(leftTree->balance == RH){
            AVLNode* rightTree = leftTree->pRight;
            if(rightTree->balance == RH){
                leftTree->balance = LH;
                root->balance = EH;
            }
            else if(rightTree->balance == EH){
                root->balance = RH;
                leftTree->balance = EH;
            }
            else{
                root->balance = RH;
                leftTree->balance = EH;
            }
            rightTree->balance = EH;
            root->pLeft = rotateLeft(leftTree);
            root = rotateRight(root);
        }
        // Case 2: LL, right rotate
        else{
            if(leftTree->balance != EH){
                root->balance = EH;
                leftTree->balance = EH;
            }
            else{
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
template<class K, class T>
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
        if(rightTree->balance == LH){
            AVLNode* leftTree = rightTree->pLeft;
            if(leftTree->balance == LH){
                rightTree->balance = RH;
                root->balance = EH;
            }
            else if(leftTree->balance == EH){
                root->balance = LH;
                rightTree->balance = EH;
            }
            else{
                root->balance = LH;
                rightTree->balance = EH;
            }
            leftTree->balance = EH;
            root->pRight = rotateRight(rightTree);
            root = rotateLeft(root);
        }
        // Case 2: RR, left rotate
        else{
            if(rightTree->balance != EH){
                root->balance = EH;
                rightTree->balance = EH;
            }
            else{
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
    // TODO:
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
    // TODO:
    inorderHelper(this->root, action);
}

// =====================================
// RedBlackTree<K, T> implementation
// =====================================

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

// =====================================
// VectorRecord implementation
// =====================================

// Overload operator << for VectorRecord - prints only the id
std::ostream &operator<<(std::ostream &os, const VectorRecord &record)
{
    os << record.id;
    return os;
}

// =====================================
// VectorStore implementation
// =====================================

// TODO: Implement all VectorStore methods here

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
