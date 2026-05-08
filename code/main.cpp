#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>

#ifndef ANKERL_NANOBENCH_IMPLEMENT
#define ANKERL_NANOBENCH_IMPLEMENT
#endif

#include "nanobench.h"

#define keytype uint64_t

using namespace std;

struct Node
{
    keytype key;
    Node *left = nullptr;
    Node *right = nullptr;

    Node() {}
    Node(keytype k) { key = k; }
};

class BinTree
{
    Node *root = nullptr;

    void Insert(keytype key)
    {
        if (root == nullptr)
        {
            root = new Node();
            root->key = key;
            return;
        }

        Node *parent;
        Node *current = root;
        bool left = false;
        while (true)
        {
            parent = current;
            if (key < current->key)
            {
                current = current->left;
                left = true;
            }
            else if (key > current->key)
            {
                current = current->right;
                left = false;
            }
            else
                return;

            if (current == nullptr)
            {
                current = new Node();
                current->key = key;
                if (left)
                    parent->left = current;
                else
                    parent->right = current;
                return;
            }
        }
    }

public:
    BinTree(vector<keytype> keys)
    {
        for (keytype k : keys)
            Insert(k);
    }
    Node *Search(keytype key)
    {
        Node *rt = root;

        while (rt != nullptr && rt->key != key)
        {
            if (key < rt->key)
                rt = rt->left;
            else
                rt = rt->right;
        }
        return rt;
    }
};

class BinTreeCont
{
    vector<Node> data;

    void Insert(keytype key)
    {
        if (data.empty())
        {
            data.push_back(Node(key));
            return;
        }

        Node *parent;
        Node *current = &data[0];
        bool left = false;
        while (true)
        {
            parent = current;
            if (key < current->key)
            {
                current = current->left;
                left = true;
            }
            else if (key > current->key)
            {
                current = current->right;
                left = false;
            }
            else
                return;

            if (current == nullptr)
            {
                data.push_back(Node(key));
                if (left)
                    parent->left = &data[data.size() - 1];
                else
                    parent->right = &data[data.size() - 1];
                return;
            }
        }
    }

public:
    BinTreeCont(vector<keytype> keys)
    {
        data.reserve(keys.size());
        for (keytype k : keys)
            Insert(k);
    }

    Node *Search(keytype key)
    {
        Node *rt = &data[0];
        while (rt != nullptr && rt->key != key)
        {
            if (key < rt->key)
                rt = rt->left;
            else
                rt = rt->right;
        }
        return rt;
    }
};

void test(size_t nodes, uint64_t gen_seed, uint64_t test_seed, uint64_t test_count, uint64_t deep_map_bucket_count, string outfile)
{
    cout << "tree size: " << nodes
         << "\nseed for generating nodes: " << gen_seed
         << "\nseed for random searches: " << test_seed
         << "\nnumber of tests: " << test_count
         << "\ndeep umap bucket count: " << deep_map_bucket_count
         << "\n\n outputting to: " << outfile << std::endl;

    vector<uint64_t> keys;
    keys.reserve(nodes);

    for (uint64_t i = 0; i < nodes; ++i)
        keys.push_back(i);

    ankerl::nanobench::Rng gen_rand(gen_seed);

    gen_rand.shuffle(keys);

    map<uint64_t, Node> map_data;
    for (keytype k : keys)
        map_data.insert(std::pair(k, Node(k)));

    unordered_map<uint64_t, Node> umap_data;
    for (keytype k : keys)
        umap_data.insert(std::pair(k, Node(k)));

    BinTree tree_data(keys);
    BinTreeCont cont_data(keys);

    ankerl::nanobench::Rng test_rand_map(test_seed);

    cout << "\n beginning map test..." << std::endl;

    uint64_t x = 0;
    Node *res;
    keytype search;

    ankerl::nanobench::Bench bench = ankerl::nanobench::Bench()
                                         .title("Search Time Benchmark")
                                         .warmup(1000)
                                         .minEpochIterations(test_count)
                                         .unit("search")
                                         .batch(1)
                                         .name("map test")
                                         .run(
                                             [&]
                                             {
                                                 // search = test_rand_map()%nodes;res = &map_data[search]; x+=res->key;

                                                 search = test_rand_map() % nodes;
                                                 res = &map_data[search];
                                                 x += res->key;
                                                 ankerl::nanobench::doNotOptimizeAway(test_rand_map);
                                                 ankerl::nanobench::doNotOptimizeAway(res);
                                                 ankerl::nanobench::doNotOptimizeAway(search);
                                                 ankerl::nanobench::doNotOptimizeAway(x);
                                                 //   cout << "search key: " << search << " node key: "  << ((res == nullptr) ? -1 : res->key);
                                             });
    cout << "\nMap test finished, last search key: " << search << " last node key: " << ((res == nullptr) ? -1 : res->key) << "\nx: " << x << std::endl;

    ankerl::nanobench::Rng test_rand_umap(test_seed);
    x = 0;

    cout << " beginning umap test..." << std::endl;
    bench.name("unordered map test")
        .run(
            [&]
            {
                search = test_rand_umap() % nodes;
                res = &umap_data[search];
                x += res->key;
                ankerl::nanobench::doNotOptimizeAway(test_rand_umap);
                ankerl::nanobench::doNotOptimizeAway(res);
                ankerl::nanobench::doNotOptimizeAway(search);
                ankerl::nanobench::doNotOptimizeAway(x);
                // ankerl::nanobench::doNotOptimizeAway([&]{search = test_rand_umap()%nodes;res = &umap_data[search];x+=res->key;});
            });

    cout << "\nUnordered map test finished, last search key: " << search << " last node key: " << ((res == nullptr) ? -1 : res->key) << "\nx: " << x << std::endl;

    cout << " rehashing map..." << std::endl;
    umap_data.max_load_factor(nodes);
    umap_data.rehash(deep_map_bucket_count);
    cout << "\n beginning deep umap test..." << std::endl;
    ankerl::nanobench::Rng test_rand_dmap(test_seed);

    x = 0;
    bench.name("deep umap test")
        .run(
            [&]
            {
                search = test_rand_dmap() % nodes;
                res = &umap_data[search];
                x += res->key;
                ankerl::nanobench::doNotOptimizeAway(test_rand_dmap);
                ankerl::nanobench::doNotOptimizeAway(res);
                ankerl::nanobench::doNotOptimizeAway(search);
                ankerl::nanobench::doNotOptimizeAway(x);
                // ankerl::nanobench::doNotOptimizeAway([&]{search = test_rand_dmap()%nodes;res = &umap_data[search];x+=res->key;});
            });

    cout << "\nDeep unordered map test finished, last search key: " << search << " last node key: " << ((res == nullptr) ? -1 : res->key) << "\nx: " << x << std::endl;

    ankerl::nanobench::Rng test_rand_tree(test_seed);
    x = 0;

    cout << " beginning tree test..." << std::endl;

    bench.name("simple bintree test")
        .run(
            [&]
            {
                search = test_rand_tree() % nodes;
                res = tree_data.Search(search);
                x += res->key;
                ankerl::nanobench::doNotOptimizeAway(test_rand_tree);
                ankerl::nanobench::doNotOptimizeAway(res);
                ankerl::nanobench::doNotOptimizeAway(search);
                ankerl::nanobench::doNotOptimizeAway(x);
                // ankerl::nanobench::doNotOptimizeAway([&]{search = test_rand_tree()%nodes;res = tree_data.Search(search);x+=res->key;});
            });

    cout << "\nSimple binary tree test finished, last search key: " << search << " last node key: " << ((res == nullptr) ? -1 : res->key) << "\nx: " << x << std::endl;

    x = 0;

    cout << " beginning contree test..." << std::endl;
    ankerl::nanobench::Rng test_rand_cont(test_seed);
    bench.name("continuous bintree test")
        .run(
            [&]
            {
                search = test_rand_cont() % nodes;
                res = tree_data.Search(search);
                x += res->key;
                ankerl::nanobench::doNotOptimizeAway(test_rand_cont);
                ankerl::nanobench::doNotOptimizeAway(res);
                ankerl::nanobench::doNotOptimizeAway(search);
                ankerl::nanobench::doNotOptimizeAway(x);
                // ankerl::nanobench::doNotOptimizeAway([&]{search = test_rand_cont()%nodes;res = cont_data.Search(search);x+=res->key;});
            });

    cout << "Continuous binary tree test finished, last search key: " << search << " last node key: " << ((res == nullptr) ? -1 : res->key) << "\nx: " << x << std::endl;

    ofstream res_out(outfile);
    vector<ankerl::nanobench::Result> results = bench.results();
    bench.render(ankerl::nanobench::templates::json(), res_out);
    res_out.close();
}

int main(int argc, char *argv[])
{
    cout << "Beginning execution\n";

    if (argc == 1)
    {
        test(100000, 10000, 10000, 1000000, 1000, "results_default.json");
    }
    else if (argc == 7)
    {

        size_t nodes = stoul(argv[1]);
        uint64_t gen_seed = stoul(argv[2]);
        uint64_t test_seed = stoul(argv[3]);
        uint64_t test_count = stoul(argv[4]);
        uint64_t dmp_buckets = stoul(argv[5]);
        test(nodes, gen_seed, test_seed, test_count, dmp_buckets, argv[6]);
    }
    else
        cerr << "Invalid input\n";

    cout << "\nover\n";
}
