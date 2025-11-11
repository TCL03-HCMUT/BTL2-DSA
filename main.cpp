#include "VectorStore.h"

void testcase1()
{
    AVLTree<int, int> tree;
    for (int i = 0; i < 10; i++)
    {
        tree.insert(i, i);
        tree.insert(i, i);
    }
    tree.printTreeStructure();
    tree.inorderTraversal([](const int &val) -> void{cout << val << " ";});
    cout << "\n";
}

void testcase2()
{
    AVLTree<int, int> tree;
    for (int i = 0; i < 10; i++)
    {
        tree.insert(i, i);
        tree.insert(i, i);
    }
    tree.printTreeStructure();
    tree.remove(3);
    tree.printTreeStructure();
    cout << "\n";
}

void testcase3()
{
    RedBlackTree<int, int> tree;
    for (int i = 0; i < 5; i++)
    {
        tree.insert(i, i);
    }
    tree.printTreeStructure();
    cout << "\n";

    tree.remove(1);
    tree.printTreeStructure();
}

int main() {
    // Student can use this main function to do some basic testing
    testcase3();
    return 0;
}