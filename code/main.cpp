#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "nanobench.h"

#define keytype uint64_t

using namespace std;


struct Node {
    keytype key;
    Node* left = nullptr;
    Node* right = nullptr;
};

class BinTree {
    Node* root;

    void Insert(keytype key){}

public:
    BinTree(vector<keytype> keys){}
    Node Search(){}

};

class BinTreeCont {
    Node* root;
    vector<Node> data;
    void Insert(keytype key){}

public:
    BinTreeCont(vector<keytype> keys){}
    Node Search(){}

};


int main() {

}

