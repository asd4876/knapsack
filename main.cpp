#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cstdlib>
#include <limits>
#include "knapsack.h"

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif // INT_MAX

using namespace std;

int getInput(int min, int max){
    string tmp;
    cin>>tmp;
    int a = atoi(tmp.c_str());
    while(cin.fail()||a<min||a>max){
        cout<<"Illegal input, please input again: ";
        cin>>tmp;
        a = atoi(tmp.c_str());
    }
    return a;
}

int main()
{
    knapsack k;
    cout<<"------------------Knapsack01 Problem using Branch and Bound------------------"<<endl;
    cout<<"This program is used to solve knapsack01 problem using branch and bound,";
    cout<<"you can choose following types to test this program:"<<endl;
    cout<<"\tType 1: insert data yourself."<<endl;
    cout<<"\tType 2: load default test data."<<endl;
    cout<<"\tType 3: generate test data randomly."<<endl;
    cout<<"Please input your choice: ";
    int type = getInput(1, 3);
    switch(type){
    case 1:{
        cout<<"Please input knapsack size: ";
        int ksize = getInput(1, INT_MAX);
        cout<<"Please input goods num: ";
        int num = getInput(1, INT_MAX);
        vector<Goods> glist;
        for(int i=0;i<num;i++){
            cout<<"Goods ["<<i+1<<"].weight: ";
            int w = getInput(1, INT_MAX);
            cout<<"Goods ["<<i+1<<"].value: ";
            int v = getInput(1, INT_MAX);
            glist.push_back(Goods(i+1, w, v));
        }
        k.loadTest(glist, ksize);
        break;
    }
    case 2:{
        cout<<"There are four default data set: "<<endl;
        cout<<"\tType 1: test data same as PPT."<<endl;
        cout<<"\tType 2: test data with no solution."<<endl;
        cout<<"\tType 3: test data with one solution."<<endl;
        cout<<"\tType 4: test data with more than one solution."<<endl;
        cout<<"Please input your choice: ";
        int t = getInput(1, 4);
        switch(t){
        case 1:{
            k.loadTest("ppt");
            break;
        }
        case 2:{
            k.loadTest("test0");
            break;
        }
        case 3:{
            k.loadTest("test1");
            break;
        }
        case 4:{
            k.loadTest("test2");
            break;
        }
        default:{
            cout<<"Unknown input type"<<endl;
            break;
        }
        }
        break;
    }
    case 3:{
        cout<<"Please input knapsack size: ";
        int ksize = getInput(1, INT_MAX);
        cout<<"Please input goods num: ";
        int num = getInput(1, INT_MAX);
        k.loadTest(num, ksize);
        break;
    }
    default:
        cout<<"Unknown input type"<<endl;
        break;
    }
    k.printGoods();
    k.calculate(true);
    k.printResult();
    return 0;
}
