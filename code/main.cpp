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
    Node* root = nullptr;

    void Insert(keytype key){
        Node*& current = root;

        while (true)
        {
            if (current == nullptr){
                current = new Node();
                current -> key = key;
                return;
            }

            if (key < current -> key) current = current ->left;
            else if (key > current -> key) current = current -> right;
            else return;
        }
    }

public:
    BinTree(vector<keytype> keys){
        for (keytype k : keys) Insert(k);
    }
    Node Search(){}

};

class BinTreeCont {
    Node* root = nullptr;
    vector<Node> data;
    void Insert(keytype key){
        Node*& current = root;

        while (true)
        {
            if (current == nullptr){
                Node n;
                n.key = key;
                data.push_back(n);
                current = &data[data.size()-1];
                return;
            }

            if (key < current -> key) current = current ->left;
            else if (key > current -> key) current = current -> right;
            else return;
        }
    }

public:
    BinTreeCont(vector<keytype> keys){
        data.reserve(keys.size());
        for (keytype k : keys) Insert(k);
    }
    Node Search(){}

};


void test(size_t nodes, uint64_t gen_seed, uint64_t test_seed){
    vector<uint64_t> keys;
    keys.reserve(nodes);

    for (uint64_t i = 0; i < nodes; ++i) keys.push_back(i);

    ankerl::nanobench::Rng gen_rand(gen_seed);

    gen_rand.shuffle(keys);

    map<uint64_t, Node> map_data;
    for (keytype k : keys) map_data.insert(std::pair(k, Node()));

    BinTree tree_data(keys);
    BinTreeCont cont_data(keys);


}


int main(int argc, char* argv[]) {
if (argc == 1){
    test(10000, 10000, 10000);
}
else if (argc == 4){
    size_t nodes = stoul(argv[1]);
    uint64_t gen_seed = stoul(argv[2]);
    uint64_t test_seed = stoul(argv[3]);

    test(nodes, gen_seed, test_seed);
}
else cerr << "Invalid input\n";

cout << "over\n";

}

