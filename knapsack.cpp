#include "knapsack.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <typeinfo>
#include <cstdlib>
#include <ctime>

using namespace std;

knapsack::knapsack(){
}

knapsack::knapsack(vector<Goods> &input, int max_size){
    MAX_SIZE = max_size;
    goods_list.assign(input.begin(),input.end());
    num = input.size();
}

knapsack::~knapsack(){

}

void knapsack::printGoods(){
    cout<<"---------------------print input info---------------------"<<endl;
    cout<<"Knapsack size: "<<MAX_SIZE<<endl;
    cout<<"Total goods number: "<<num<<endl;
    for(int i=0;i<num;i++){
        cout<<"Goods id: "<<goods_list[i].index<<" ";
        cout<<"weight: "<<goods_list[i].weight<<" ";
        cout<<"value: "<<goods_list[i].value<<" ";
        cout<<"unit value: "<<goods_list[i].unit<<endl;
    }
}

void knapsack::printResult(){
    cout<<"---------------------print solutions---------------------"<<endl;
    if(solutions.size() == 0){
        cout<<"Max value: 0, no goods can be put into knapsack!"<<endl;
        return;
    }
    int n = solutions.size();
    cout<<"Max value: "<<MAX_VALUE<<endl;
    cout<<"Total solution number: "<<n<<endl;
    for(int i=0;i<n;i++){
        cout<<"Solution "<<(i+1)<<": "<<endl;
        int tw = 0;
        int s = solutions[i].size();
        for(int j=0;j<s;j++){
            int id = solutions[i][j];
            tw += goods_list[id].weight;
            cout<<"\tGoods id: "<<goods_list[id].index<<" ";
            cout<<"weight: "<<goods_list[id].weight<<" ";
            cout<<"value: "<<goods_list[id].value<<endl;
        }
        cout<<"\ttotal weight: "<<tw<<endl;
    }
}


void knapsack::loadTest(int num, int max_size){
    cout<<"Generate random test data"<<endl;
    this->num = num;
    this->MAX_SIZE = max_size;
    int w_b = (max_size/num)*RANDOM_BOUND;
    int u_b = num*RANDOM_BOUND;
    srand((unsigned)time(NULL));
    for(int i=0;i<num;i++){
        int w = rand()%w_b+1;
        int v = w*(rand()%u_b+1);
        goods_list.push_back(Goods(i+1, w, v));
    }
}

void knapsack::loadTest(vector<Goods> &input, int max_size){
    MAX_SIZE = max_size;
    goods_list.assign(input.begin(),input.end());
    num = input.size();
}

void knapsack::loadTest(string type){
    if(type == "ppt"){
        cout<<"Load samples from ppt."<<endl;
        num = 4;
        MAX_SIZE = 10;
        goods_list.push_back(Goods(1, 5, 25));
        goods_list.push_back(Goods(2, 7, 42));
        goods_list.push_back(Goods(3, 4, 40));
        goods_list.push_back(Goods(4, 3, 12));
    }
    else if(type == "test2"){
        cout<<"Load samples with more than one solutions."<<endl;
        num = 4;
        MAX_SIZE = 10;
        goods_list.push_back(Goods(1, 5, 25));
        goods_list.push_back(Goods(2, 7, 42));
        goods_list.push_back(Goods(3, 4, 40));
        goods_list.push_back(Goods(4, 5, 25));
    }
    else if(type == "test0"){
        cout<<"Load samples with no solution."<<endl;
        num = 2;
        MAX_SIZE = 5;
        goods_list.push_back(Goods(1, 6, 30));
        goods_list.push_back(Goods(2, 7, 42));
    }
    else if(type == "test1"){
        cout<<"Load samples with one solution."<<endl;
        num = 4;
        MAX_SIZE = 10;
        goods_list.push_back(Goods(1, 5, 25));
        goods_list.push_back(Goods(2, 7, 42));
        goods_list.push_back(Goods(3, 4, 40));
        goods_list.push_back(Goods(4, 3, 12));
    }

}

void knapsack::calculate(bool show){
    if(show){
        cout<<"---------------------start calculate-------------------"<<endl;
    }
    // sort input goods list.
    sort(goods_list.begin(),goods_list.end());

    // initialize node list and root node.
    priority_queue<Node*, vector<Node*>, NodeCompare> pqueue;
    double ub = goods_list[0].unit*MAX_SIZE;
    Node *root = new Node(0, 0, ub, -1, false, nullptr);
    pqueue.push(root);

    // iteratively fetch data and calculate.
    while(!pqueue.empty()){

        // fetch data with largest upper bound.
        Node *n = pqueue.top();
        pqueue.pop();
        int nid = n->id;
        if(show){
            cout<<"pop node: goods["<<nid<<"], weight: "<<n->weight<<", value: "<<n->value<<", upper bound: "<<n->ubound<<endl;
        }

        // if current node is the leaf node, then get the first solution.
        if(nid == num-1){
            cout<<"\tfind result!"<<endl;
            // record its value as final result.
            MAX_VALUE = n->value;
            // maybe there are not only one solution, those nodes with same value as current node are also solutions.
            while(MAX_VALUE > 0&&n->value == MAX_VALUE){
                // record solution.
                vector<int> sol;
                if(n->choose) sol.push_back(n->id);
                Node *p = n->parent;
                while(p->id > -1){
                    if(p->choose) sol.push_back(p->id);
                    p = p->parent;
                }
                solutions.push_back(sol);
                if(!pqueue.empty()){
                     n = pqueue.top();
                     pqueue.pop();
                }
                else break;
            }
            break;
        }
        else{
            // generate left/choose node.
            Goods g = goods_list[nid+1];
            int v_c = n->value + g.value;
            int w_c = n->weight + g.weight;
            double ub_c = v_c;
            if(w_c <= MAX_SIZE){
                if(nid+2 < num) ub_c += (MAX_SIZE - w_c)*goods_list[nid+2].unit;
                Node *c = new Node(w_c, v_c, ub_c, nid+1, true, n);
                pqueue.push(c);
                n->leftchild = c;
                if(show){
                    cout<<"\tpush node: choose goods["<<nid+1<<"], weight: "<<w_c<<", value: "<<v_c<<", upper bound: "<<ub_c<<endl;
                }
            }
            else{
                if(show){
                    cout<<"\tignore node: choose goods["<<nid+1<<"]"<<endl;
                }
            }

            // generate right/not choose node.
            double ub_nc = n->value;
            if(nid+2 < num) ub_nc += (MAX_SIZE - n->weight)*goods_list[nid+2].unit;
            Node *nc = new Node(n->weight, n->value, ub_nc, nid+1, false, n);
            pqueue.push(nc);
            n->rightchild = nc;
            if(show){
                cout<<"\tpush node: not choose goods["<<nid+1<<"], weight: "<<n->weight<<", value: "<<n->value<<", upper bound: "<<ub_nc<<endl;
            }
        }
    }
    cout<<"calculate successfully!"<<endl;
    release(root);
    return;
}

void knapsack::release(Node *root){
    if(root->leftchild!=nullptr) release(root->leftchild);
    if(root->rightchild!=nullptr) release(root->rightchild);
    delete root;
}
