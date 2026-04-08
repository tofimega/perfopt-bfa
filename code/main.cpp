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
    Node* Search(keytype key){ return nullptr;}

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
    Node* Search(keytype key){return nullptr;}

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

    ankerl::nanobench::Rng test_rand_map(test_seed);
  



    Node* res;
    ankerl::nanobench::Bench bench = ankerl::nanobench::Bench().title("Search Time Benchmark").unit("nodes").batch(nodes).name("map test").run(
        [&]{
            ankerl::nanobench::doNotOptimizeAway(res = &map_data[test_rand_map()%nodes]);
        });

        cout << "Map test finished, last node key: "  << ((res == nullptr) ? -1 : res->key) << "\n";
  ankerl::nanobench::Rng test_rand_tree(test_seed);

        bench.name("simple bintree test").run(
        [&]{
             ankerl::nanobench::doNotOptimizeAway(res = tree_data.Search(test_rand_tree()%nodes));
        });

        cout << "Simple binary tree test finished, last node key: "  << ((res == nullptr) ? -1 : res->key)<< "\n";
    ankerl::nanobench::Rng test_rand_cont(test_seed);
        bench.name("continuous bintree test").run(
        [&]{
             ankerl::nanobench::doNotOptimizeAway(res = tree_data.Search(test_rand_tree()%nodes));
        });

        cout << "Continuous binary tree test finished, last node key: "  << ((res == nullptr) ? -1 : res->key)<< "\n";


        vector<ankerl::nanobench::Result> results = bench.results();
        bench.render(ankerl::nanobench::templates::json(), cout);
}


int main(int argc, char* argv[]) {
    cout << "Beginning execution\n";
    
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

    cout << "\nover\n";

}

