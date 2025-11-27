#include "VectorStore.h"
#include <cassert>
#include <iomanip>

std::vector<float> *simpleEmbedding(const std::string &text)
{
    std::vector<float> *vec = new std::vector<float>();
    std::stringstream ss(text);
    float value;
    while (ss >> value)
    {
        vec->push_back(value);
    }
    return vec;
}

std::string printVectorRecord(const VectorRecord *record)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    oss << "ID=" << record->id
        << ", RawText=" << record->rawText
        << ", RawLength=" << record->rawLength
        << ", Vector=";

    if (record->vector)
    {
        oss << "[";
        for (int i = 0; i < record->vector->size(); ++i)
        {
            oss << (*record->vector)[i];
            if (i + 1 < record->vector->size())
                oss << ", ";
        }
        oss << "]";
    }

    oss << ", Distance=" << record->distanceFromReference
        << ", Norm=" << record->norm;

    return oss.str();
}


void test_207()
{
    std::vector<float> refVec = {-1.0f};
    VectorStore store(1, simpleEmbedding, refVec);
    store.addText("0");
    store.addText("1");
    store.addText("2");
    store.addText("3");
    store.addText("4");

    cout << endl;
    cout << "Original:\n";
    cout << printVectorRecord(store.getRootVector()) << endl;
    cout << setprecision(10) << fixed << store.getAverageDistance() << endl;
    {
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 5; ++i)
        {
            cout << (printVectorRecord(sorted[i])) << endl;
        }
    }
    cout << endl;

    cout << "Remove at index 0\n";
    store.removeAt(0);
    cout << printVectorRecord(store.getRootVector()) << endl;
    cout << setprecision(10) << fixed << store.getAverageDistance() << endl;

    {
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 4; ++i)
        {
            cout << (printVectorRecord(sorted[i])) << endl;
        }
    }
    cout << endl;

    cout << "Remove at index 3\n";
    store.removeAt(3);
    cout << printVectorRecord(store.getRootVector()) << endl;
    cout << setprecision(10) << fixed << store.getAverageDistance() << endl;

    {
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 3; ++i)
        {
            cout << (printVectorRecord(sorted[i])) << endl;
        }
    }
    cout << endl;

    cout << "AddText \"-10\"\n";
    store.addText("-10");
    cout << printVectorRecord(store.getRootVector()) << endl;
    cout << setprecision(10) << fixed << store.getAverageDistance() << endl;

    {
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 4; ++i)
        {
            cout << (printVectorRecord(sorted[i])) << endl;
        }
    }
    cout << endl;

    cout << "Remove at index 0\n";
    store.removeAt(0);
    cout << printVectorRecord(store.getRootVector()) << endl;
    cout << setprecision(10) << fixed << store.getAverageDistance() << endl;
    {
        std::vector<VectorRecord *> sorted = store.getAllVectorsSortedByDistance();
        for (int i = 0; i < 3; ++i)
        {
            cout << (printVectorRecord(sorted[i])) << endl;
        }
    }

    store.removeAt(0);
    store.removeAt(0);
    store.removeAt(0);

    cout << boolalpha << (store.getRootVector() == nullptr) << endl;
    cout << boolalpha << (store.getAverageDistance() == 0.0) << endl;

    store.addText("-1");
    cout << printVectorRecord(store.getRootVector());

    cout << endl;
}

void test_001()
{
    AVLTree<int, int> tree;
    for (int i = 1; i <= 5; i++)
    {
        tree.insert(i, i);
    }

    tree.remove(1);
    tree.remove(5);
    tree.printTreeStructure();
    tree.insert(5, 5);
    tree.printTreeStructure();
}

void test_002()
{
    RedBlackTree<int, string> rbt;
    rbt.insert(8, "8");
    rbt.insert(18, "18");
    rbt.insert(5, "5");
    rbt.insert(15, "15");
    rbt.insert(17, "17");
    rbt.insert(25, "25");
    rbt.insert(40, "40");
    // rbt.insert(80, "80");

    rbt.printTreeStructure();
}

void test_301()
{
    std::vector<float> refVec = {0.0f};
    VectorStore store(1, simpleEmbedding, refVec);

    
    for (int i = 0; i <= 10; ++i)
    {
        store.addText(std::to_string(i));
    }
    store.getAVLTree()->printTreeStructure();
    {
        int *rawResult = store.rangeQueryFromRoot(2, 8);
        std::vector<int> result;
        if (rawResult)
        {
            result.assign(rawResult, rawResult + 7);
            delete[] rawResult;
        }
        // std::sort(result.begin(), result.end());
        const std::vector<int> expected = {3, 4, 5, 6, 7, 8, 9};
        for (int i = 0; i < 7; i++)
            cout << result[i] << " " << expected[i] << endl;
    }

    cout << endl;

    {
        int *rawResult = store.rangeQuery({5}, 2, "euclidean");
        std::vector<int> result;
        if (rawResult)
        {
            result.assign(rawResult, rawResult + 5);
            delete[] rawResult;
        }
        // std::sort(result.begin(), result.end());
        const std::vector<int> expected = {4, 5, 6, 7, 8};
        for (int i = 0; i < 5; i++)
            cout << result[i] << " " << expected[i] << endl;
    }
}

void test_heap()
{
    int arr[] = {7, 4, 9, 2, 15, 1, 0};
    MaxHeap<int> heap;
    for (int i = 0; i < 7; i++)
    {
        heap.push(arr[i]);
    }

    for (int i = 6; i >= 0; i--)
    {
        arr[i] = heap.peek();
        heap.pop();
    }
    for (int i = 0; i < 7; i++)
    {
        cout << arr[i] << " ";
    }
}
int main() {
    // Student can use this main function to do some basic testing
    test_heap();
    return 0;
}