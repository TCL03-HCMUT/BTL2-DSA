// NOTE: Per assignment rules, only this single include is allowed here.
#include "VectorStore.h"

// =====================================
// Helper functions
// =====================================

// Helper function to print n spaces for tree visualization
void printNSpace(int n) {
    for (int i = 0; i < n; i++) {
        cout << " ";
    }
}

// =====================================
// AVLTree<K, T> implementation
// =====================================

template <class K, class T>
void AVLTree<K, T>::printTreeStructure() const {
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


//TODO: Implement all AVLTree<K, T> methods here
template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::rotateRight(AVLTree<K, T>::AVLNode*& root)
{
    AVLNode* temp = root->pLeft;
    root->pLeft = temp->pRight;
    temp->pRight = root;
    root = temp;
    return root;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode *AVLTree<K, T>::rotateLeft(AVLTree<K, T>::AVLNode*& root)
{
    AVLNode* temp = root->pRight;
    root->pRight = temp->pLeft;
    temp->pLeft = root;
    root = temp;
    return root;
}

template <class K, class T>
void AVLTree<K, T>::clearHelper(AVLTree<K, T>::AVLNode* node)
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
    AVLNode* leftTree = root->pLeft;

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
    AVLNode* rightTree = root->pRight;

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
    // TODO:
    bool taller = false;
    root = insert(root, key, value, taller);
}

template <class K, class T>
void AVLTree<K, T>::remove(const K &key)
{
    // TODO:
}

template <class K, class T>
bool AVLTree<K, T>::contains(const K &key) const
{
    // TODO:
    if (!root) return false;
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
int AVLTree<K, T>::getHeightHelper(AVLTree<K, T>::AVLNode* root) const
{
    if (!root) return 0;
    return max(getHeightHelper(root->pLeft), getHeightHelper(root->pRight)) + 1;
}

template <class K, class T>
int AVLTree<K, T>::getHeight() const
{
    // TODO: 
    return getHeightHelper(this->root);
}

template <class K, class T>
int AVLTree<K, T>::getSize() const
{
    // TODO:
    if (!(this->root)) return 0;
    queue<AVLNode*> q;
    q.push(root);
    int result = 0;
    while (!q.empty())
    {
        AVLNode* current = q.front();
        q.pop();
        result++;
        if (current->pLeft) q.push(current->pLeft);
        if (current->pRight) q.push(current->pRight);
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
}

template <class K, class T>
void AVLTree<K, T>::inorderHelper(AVLTree<K, T>::AVLNode* root, void (*action)(const T&)) const
{
    if (root)
    {
        action(root->data);
        inorderHelper(root->pLeft, action);
        inorderHelper(root->pRight, action);
    }
}

template <class K, class T>
void AVLTree<K, T>::inorderTraversal(void (*action)(const T&)) const
{
    // TODO:
    inorderHelper(this->root, action);
}

// =====================================
// RedBlackTree<K, T> implementation
// =====================================

template <class K, class T>
void RedBlackTree<K, T>::printTreeStructure() const {
    if (this->root == nullptr) {
        cout << "NULL\n";
        return;
    }
    
    queue<RBTNode*> q;
    q.push(root);
    RBTNode* temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;
    
    // Calculate tree height for spacing
    int height = 0;
    queue<RBTNode*> heightQ;
    heightQ.push(root);
    while (!heightQ.empty()) {
        int size = heightQ.size();
        height++;
        for (int i = 0; i < size; i++) {
            RBTNode* node = heightQ.front();
            heightQ.pop();
            if (node->left) heightQ.push(node->left);
            if (node->right) heightQ.push(node->right);
        }
    }
    
    int space = pow(2, height);
    printNSpace(space / 2);
    
    while (!q.empty()) {
        temp = q.front();
        q.pop();
        
        if (temp == nullptr) {
            cout << "NULL";
            q.push(nullptr);
            q.push(nullptr);
        } else {
            // Print node data and color as tuple: (data, color)
            cout << "(" << temp->data << ",";
            if (temp->color == RED) {
                cout << "R)";
            } else {
                cout << "B)";
            }
            
            q.push(temp->left);
            q.push(temp->right);
        }
        
        printNSpace(space);
        count++;
        
        if (count == maxNode) {
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }
        
        if (level == height) {
            return;
        }
    }
}

//TODO: Implement all other RedBlackTree<K, T> methods here




// =====================================
// VectorRecord implementation
// =====================================

// Overload operator << for VectorRecord - prints only the id
std::ostream& operator<<(std::ostream& os, const VectorRecord& record) {
    os << record.id;
    return os;
}

// =====================================
// VectorStore implementation
// =====================================

//TODO: Implement all VectorStore methods here

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



