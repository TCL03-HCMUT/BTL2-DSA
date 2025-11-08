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



int main() {
    // Student can use this main function to do some basic testing
    testcase2();
    return 0;
}