#ifndef KNAPSACK_H
#define KNAPSACK_H

#include <string>
#include <vector>

#define RANDOM_BOUND 3

using namespace std;

struct Goods{
    int index;
    int weight;
    int value;
    double unit;
    Goods(int i, int w, int v): index(i), weight(w), value(v){
        unit = value*1.0/weight;
    }
    bool operator< (const Goods &g) const{
        return this->unit > g.unit;
    }
};

struct Node{
    int weight;          // total weight of current Node.
    int value;           // total value of current Node.
    double ubound;       // upper bound of current Node.
    int id;              // current goods id.
    bool choose;         // choose this goods or not.
    Node *parent;        // parent Node pointer, used to get solution from leaf Nodes.
    Node *leftchild;     // left child Node pointer, used to release Nodes from root.
    Node *rightchild;    // right child Node pointer, used to release Nodes from root.
    Node(int w, int v, double ub, int i, bool c, Node *p):
        weight(w), value(v), ubound(ub), id(i), choose(c), parent(p), leftchild(nullptr), rightchild(nullptr){}
    // operator overload for priority_queue.
    bool operator< (const Node &node) const{
        return this->ubound < node.ubound;
    }
};

// used to compare for the priority_queue<Node *>
class NodeCompare
{
public:
    bool operator() (const Node *a, const Node *b) const
    {
        return a->ubound < b->ubound;
    }
};

class knapsack
{
public:
    knapsack();
    knapsack(vector<Goods> &goods_list, int max_size);
    ~knapsack();
    // generate random test data.
    void loadTest(int num, int max_size);
    // user input test data.
    void loadTest(vector<Goods> &goods_list, int max_size);
    // default test data.
    void loadTest(string type);
    void calculate(bool show);
    void printGoods();
    void printResult();
private:
    int MAX_SIZE;
    int MAX_VALUE;
    int num;
    vector<Goods> goods_list;
    vector<vector<int>> solutions;
    void release(Node *root);
};

#endif // KNAPSACK_H
