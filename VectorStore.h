#ifndef VECTORSTORE_H
#define VECTORSTORE_H

#pragma once
// NOTE: Per assignment rules, only this single include is allowed here.
#include "main.h"

// ------------------------------
// AVL balance enum
// ------------------------------
enum BalanceValue
{
    LH = -1, // Left Higher
    EH = 0,  // Equal Height
    RH = 1   // Right Higher
};

// ------------------------------
// Generic AVL Tree (template)
// ------------------------------
template <class K, class T>
class AVLTree
{
    friend class VectorStore; // Allow VectorStore to access protected/private members

public:
    class AVLNode
    {
    public:
        K key;
        T data;
        AVLNode *pLeft;
        AVLNode *pRight;
        BalanceValue balance;

        AVLNode(const K &key, const T &value)
            : key(key), data(value), pLeft(nullptr), pRight(nullptr), balance(EH) {}

        friend class VectorStore; // Allow VectorStore to access AVLNode members
    };

protected:
    AVLNode *root;
    int count;

    AVLNode *rotateRight(AVLNode *&node);
    AVLNode *rotateLeft(AVLNode *&node);
    void clearHelper(AVLNode *node);
    int getHeightHelper(AVLNode *root) const;
    void inorderHelper(AVLNode *root, void (*action)(const T &)) const;
    AVLNode *insert(AVLNode *&root, const K &key, const T &value, bool &taller);
    AVLNode *leftBalance(AVLNode *&root, bool &taller);
    AVLNode *rightBalance(AVLNode *&root, bool &taller);

    AVLNode *remove(AVLNode *&root, const K &key, bool &shorter, bool &success);
    AVLNode *deleteLeftBalance(AVLNode *&root, bool &shorter);
    AVLNode *deleteRightBalance(AVLNode *&root, bool &shorter);

public:
    AVLTree();
    ~AVLTree();

    void insert(const K &key, const T &value);
    void remove(const K &key);
    bool contains(const K &key) const;

    int getHeight() const;
    int getSize() const;
    int size() const;
    bool empty() const;
    void clear();

    void printTreeStructure() const;

    void inorderTraversal(void (*action)(const T &)) const;

    AVLNode *getRoot() const { return root; }

    AVLNode *getNodeAt(int index);
    AVLNode *getNodeAt(AVLNode *node, int index, int &count);
    void getNodes(AVLNode *root, vector<AVLNode *> &result);
    AVLNode *findMax();
};

enum Color
{
    RED,
    BLACK
};

// RedBlackTree class
template <class K, class T>
class RedBlackTree
{
    friend class VectorStore; // Allow VectorStore to access protected/private members

public:
    // RBTNode class

    class RBTNode
    {
    public:
        K key;
        T data;
        Color color;
        RBTNode *parent;
        RBTNode *left;
        RBTNode *right;

        // Constructor
        RBTNode(const K &key, const T &value);

        void recolorToRed();
        void recolorToBlack();

        friend class VectorStore; // Allow VectorStore to access RBTNode members
    };

private:
    RBTNode *root;
    int count;

protected:
    void rotateLeft(RBTNode *node);
    void rotateRight(RBTNode *node);

    RBTNode *lowerBoundNode(const K &key) const;
    RBTNode *upperBoundNode(const K &key) const;

    void clearHelper(RBTNode *root);

    // Insert helpers
    void insertFixup(RBTNode *node);

    // Delete helpers
    void transplant(RBTNode *u, RBTNode *v);
    RBTNode *findMax(RBTNode *node);
    RBTNode *findMin(RBTNode *node) const;
    void removeNode(RBTNode *deleteNode);
    void removeFixup(RBTNode *doubleBlackNode, RBTNode *doubleBlackParent);
    RBTNode *successor(RBTNode *node) const;

public:
    RedBlackTree();
    ~RedBlackTree();

    bool empty() const;
    int size() const;
    void clear();
    void insert(const K &key, const T &value);
    void remove(const K &key);
    RBTNode *find(const K &key) const;
    bool contains(const K &key) const;

    RBTNode *lowerBound(const K &key, bool &found) const;
    RBTNode *upperBound(const K &key, bool &found) const;

    void printTreeStructure() const;

    RBTNode *getRoot() const { return root; }
    vector<RBTNode *> filter(K min, K max) const;
};

// ------------------------------
// VectorRecord
// ------------------------------
class VectorRecord
{
public:
    int id;
    std::string rawText;
    int rawLength;
    std::vector<float> *vector;
    double distanceFromReference;
    double norm;

    VectorRecord()
        : id(-1), rawLength(0), vector(nullptr), distanceFromReference(0.0), norm(0.0) {}

    VectorRecord(int _id,
                 const std::string &_rawText,
                 std::vector<float> *_vec,
                 double _dist, double _norm = 0.0)
        : id(_id),
          rawText(_rawText),
          rawLength(static_cast<int>(_rawText.size())),
          vector(_vec),
          distanceFromReference(_dist),
          norm(_norm) {}
    VectorRecord(const VectorRecord &other)
    {
        this->id = other.id;
        this->rawText = other.rawText;
        this->rawLength = other.rawLength;
        this->vector = other.vector;
        this->distanceFromReference = other.distanceFromReference;
        this->norm = other.norm;
    }

    VectorRecord &operator=(const VectorRecord &other)
    {
        this->id = other.id;
        this->rawText = other.rawText;
        this->rawLength = other.rawLength;
        this->vector = other.vector;
        this->distanceFromReference = other.distanceFromReference;
        this->norm = other.norm;
        return *this;
    }

    bool operator==(const VectorRecord &other)
    {
        return (this->id == other.id) && (this->rawText == other.rawText) && (this->rawLength == other.rawLength) && ((this->vector == other.vector) || (*this->vector == *other.vector)) && (this->distanceFromReference == other.distanceFromReference) && (this->norm == other.norm);
    }

    // Overload operator << to print only the id
    friend std::ostream &operator<<(std::ostream &os, const VectorRecord &record);
};

// ------------------------------
// VectorStore
// ------------------------------
class VectorStore
{
private:
    AVLTree<double, VectorRecord> *vectorStore;
    RedBlackTree<double, VectorRecord> *normIndex;

    std::vector<float> *referenceVector;
    VectorRecord *rootVector;

    int dimension;
    int count;
    double averageDistance;
    int maxId;

    vector<VectorRecord> recordList;

    std::vector<float> *(*embeddingFunction)(const std::string &);

    double distanceByMetric(const std::vector<float> &a,
                            const std::vector<float> &b,
                            const std::string &metric) const;

    void rebuildRootIfNeeded();
    void rebuildTreeWithNewRoot(VectorRecord *newRoot);

    VectorRecord *findVectorNearestToDistance(double targetDistance) const;

    void rangeCheck(int index) const;
    void metricCheck(string metric) const;

    void findNearestHelper(AVLTree<double, VectorRecord>::AVLNode *root, const std::vector<float> &query, string metric, double &bestDistance, int &bestID);

    void rangeQueryRootHelper(AVLTree<double, VectorRecord>::AVLNode *root, const double &minDist, const double &maxDist, vector<int> &result) const;
    void rangeQueryHelper(AVLTree<double, VectorRecord>::AVLNode *root, const vector<float> &query, const double &radius, string metric, vector<pair<int, double>> &result) const;
    void boundingBoxHelper(AVLTree<double, VectorRecord>::AVLNode *root, const vector<float> &minBound, const vector<float> &maxBound, vector<int> &result) const;
    void findNearestDistanceHelper(AVLTree<double, VectorRecord>::AVLNode *root, double targetDistance, VectorRecord *&best) const;
    void merge(vector<AVLTree<double, VectorRecord>::AVLNode *> &arr, int left, int mid, int right)
    {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<AVLTree<double, VectorRecord>::AVLNode *> L(n1);
        std::vector<AVLTree<double, VectorRecord>::AVLNode *> R(n2);

        for (int i = 0; i < n1; i++)
            L[i] = arr[left + i];
        for (int j = 0; j < n2; j++)
            R[j] = arr[mid + 1 + j];

        int i = 0;
        int j = 0;
        int k = left;

        while (i < n1 && j < n2)
        {
            if (L[i]->data.distanceFromReference <= R[j]->data.distanceFromReference)
            {
                arr[k] = L[i];
                i++;
            }
            else
            {
                arr[k] = R[j];
                j++;
            }
            k++;
        }

        while (i < n1)
        {
            arr[k] = L[i];
            i++;
            k++;
        }

        while (j < n2)
        {
            arr[k] = R[j];
            j++;
            k++;
        }
    }
    void mergeSort(vector<AVLTree<double, VectorRecord>::AVLNode *> &arr, int left, int right)
    {
        if (left >= right)
        {
            return;
        }

        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }

    void rebuildRoot(vector<AVLTree<double, VectorRecord>::AVLNode *> &nodeList);

    pair<AVLTree<double, VectorRecord>::AVLNode *, int> buildTreeFromArray(vector<AVLTree<double, VectorRecord>::AVLNode *> &array, int start, int end);

public:
    AVLTree<int, int> *ids;
    AVLTree<double, VectorRecord> *getAVLTree() {return vectorStore;}
    double norm(const std::vector<float> &vec) const;
    void forEachAction(AVLTree<double, VectorRecord>::AVLNode *root, void (*action)(vector<float> &, int, string &));
    void getSortedId(AVLTree<double, VectorRecord>::AVLNode *root, vector<int> &ids) const;
    void getSortedRecords(AVLTree<double, VectorRecord>::AVLNode *root, vector<VectorRecord *> &records) const;
    VectorStore(int dimension,
                std::vector<float> *(*embeddingFunction)(const std::string &),
                const std::vector<float> &referenceVector);
    ~VectorStore();

    int size();
    bool empty();
    void clear();

    std::vector<float> *preprocessing(std::string rawText);
    void addText(std::string rawText);

    VectorRecord *getVector(int index);
    std::string getRawText(int index);
    int getId(int index);

    bool removeAt(int index);

    void setReferenceVector(const std::vector<float> &newReference);
    std::vector<float> *getReferenceVector() const;
    VectorRecord *getRootVector() const;
    double getAverageDistance() const;
    void setEmbeddingFunction(std::vector<float> *(*newEmbeddingFunction)(const std::string &));

    void forEach(void (*action)(std::vector<float> &, int, std::string &));
    std::vector<int> getAllIdsSortedByDistance() const;
    std::vector<VectorRecord *> getAllVectorsSortedByDistance() const;

    double cosineSimilarity(const std::vector<float> &v1, const std::vector<float> &v2);
    double l1Distance(const std::vector<float> &v1, const std::vector<float> &v2);
    double l2Distance(const std::vector<float> &v1, const std::vector<float> &v2);

    double estimateD_Linear(const std::vector<float> &query, int k, double averageDistance, const std::vector<float> &reference, double c0_bias = 1e-9, double c1_slope = 0.05);

    int findNearest(const std::vector<float> &query, std::string metric = "cosine");
    int *topKNearest(const std::vector<float> &query, int k, std::string metric = "cosine");

    int *rangeQueryFromRoot(double minDist, double maxDist) const;
    int *rangeQuery(const std::vector<float> &query, double radius, std::string metric = "cosine") const;
    int *boundingBoxQuery(const std::vector<float> &minBound, const std::vector<float> &maxBound) const;

    double getMaxDistance() const;
    double getMinDistance() const;
    VectorRecord computeCentroid(const std::vector<VectorRecord *> &records) const;
};

template <class T>
class MaxHeap
{
public:
    vector<T> data;
    bool (*lessThan)(const T &, const T &);
    MaxHeap(bool (*lessThan)(const T &, const T &) = [](const T &a, const T &b) -> bool
            { return a < b; })
    {
        this->lessThan = lessThan;
    }

    MaxHeap(vector<T> array, bool (*lessThan)(const T &, const T &) = [](const T &a, const T &b) -> bool
            { return a < b; })
    {
        data = array;
        heapify();
        this->lessThan = lessThan;
    }

    void heapUp(int index)
    {
        while (index > 0)
        {
            int parent = (index - 1) / 2;
            if (lessThan(data[parent], data[index]))
            {
                // swap(data[parent], data[index]);
                auto temp = data[parent];
                data[parent] = data[index];
                data[index] = temp;
                index = parent;
            }
            else
                break;
        }
    }

    void heapDown(int index)
    {
        int size = data.size();
        while (2 * index + 1 < size)
        {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < size && lessThan(data[largest], data[left]))
            {
                largest = left;
            }

            if (right < size && lessThan(data[largest], data[right]))
            {
                largest = right;
            }

            if (largest != index)
            {
                // swap(data[largest], data[index]);
                auto temp = data[largest];
                data[largest] = data[index];
                data[index] = temp;
                index = largest;
            }
            else
                break;
        }
    }

    void heapify()
    {
        for (int i = data.size() / 2 - 1; i > -1; i--)
        {
            heapDown(i);
        }
    }

    T peek()
    {
        return data[0];
    }

    int size()
    {
        return data.size();
    }

    void push(T val)
    {
        data.push_back(val);
        heapUp(data.size() - 1);
    }

    T pop()
    {
        T result = data[0];

        T last = data.back();
        data[0] = last;
        data.pop_back();
        heapDown(0);
        return result;
    }

    bool empty()
    {
        return data.size() == 0;
    }
};

#endif // VECTORSTORE_H
