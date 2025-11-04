#include "VectorStore.h"

void testcase1()
{
    AVLTree<int, int> tree;
    for (int i = -9; i < 10; i++)
    {
        tree.insert(i, i);
    }
    tree.printTreeStructure();
}



int main() {
    // Student can use this main function to do some basic testing
    testcase1();
    return 0;
}