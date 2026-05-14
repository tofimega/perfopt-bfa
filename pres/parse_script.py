import json
import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

node_counts = {
    "test1.json" : 1000,
    "test2.json" : 10000,
    "test3.json" : 100000,
    "test4.json" : 1000000,
    "test5.json" : 10000000,
}

bucket_counts = {
    "100" : 100,
    "10k" : 10000,
    "1k" : 1000,
    "5k" : 5000,
    "20k" : 20000,
    "40k" : 40000,
    "70k" : 70000
}

test_names = [
    "simple bintree test",
    "continuous bintree test",
    "map test",
    "unordered map test",
     "deep umap test (100 buckets)",
     "deep umap test (1000 buckets)",
     "deep umap test (5000 buckets)",
     "deep umap test (10000 buckets)",
     "deep umap test (20000 buckets)",
     "deep umap test (40000 buckets)",
     "deep umap test (70000 buckets)"
    ]

class TestData:
    def __init__(self, test_name, node_count, elapsed, instructions, cycles, branches, branch_misses):
        self.warmup = 1000
        self.test_name = test_name
        self.node_count = node_count
        self.elapsed = elapsed
        self.instructions = instructions
        self.cycles = cycles
        self.branches = branches
        self.branch_misses = branch_misses
        self.per_second = 1/elapsed
        self.ci_ratio = cycles/instructions
        if branches != 0 and branch_misses !=0:
            self.bm_ratio = branches/branch_misses
        else:
            self.bm_ratio = 1

    def copy(other):
        return TestData(other.test_name, other.node_count, other.elapsed, other.instructions, other.cycles, other.branches, other.branch_misses)


    def get_value(self, value_string):
        match value_string:
            case "cycles": return self.cycles
            case "elapsed": return self.elapsed
            case "instructions": return self.instructions
            case "branches": return self.branches
            case "branch misses": return self.branch_misses
            case "search/s": return self.per_second
            case "cycles/ins": return self.ci_ratio
            case "misses/branch" : return self.bm_ratio
        raise ValueError("Bad param: "+value_string)

dir_name = "records"

valid_attributes = [
    "cycles", "elapsed", "instructions", "branches", "branch misses", "search/s", "cycles/ins", "misses/branch"
]

test_results = {} # key: node_count + test_name

def parse_data():
    dir_id= os.fsencode(dir_name)
    im_data = {} # key: node_count + test_name

    for n in node_counts:
        im_data[node_counts[n]] = {}
        for name in test_names:
            im_data[node_counts[n]][name] = []

    for n in node_counts:
        test_results[node_counts[n]] = {}
    
    for buckets in os.listdir(dir_id):
        str_buckets = buckets.decode()
        dmap_bcount = bucket_counts[str_buckets]
        bucket_dir = os.path.join(dir_name, buckets.decode())
        bucket_id = os.fsencode(bucket_dir)
        for son_of_bucket_named_jay in os.listdir(bucket_id):
            filename = son_of_bucket_named_jay.decode()
            fpath = os.path.join(bucket_dir, filename)
            parsed = json.loads(open(fpath).read())["results"]
            for r in parsed:
                if r["name"] == "deep umap test": # there is only one per deep map
                    testname = r["name"]+" (" +str(dmap_bcount)+" buckets)"
                    data = TestData(testname,
                                    node_counts[filename],
                                    r["median(elapsed)"],
                                    r["median(instructions)"],
                                    r["median(cpucycles)"],
                                    r["median(branchinstructions)"],
                                    r["median(branchmisses)"])
                    test_results[node_counts[filename]][testname] = data
                else: # store others for averaging
                    testname = r["name"]
                    data = TestData(testname,
                                    node_counts[filename],
                                    r["median(elapsed)"],
                                    r["median(instructions)"],
                                    r["median(cpucycles)"],
                                    r["median(branchinstructions)"],
                                    r["median(branchmisses)"])
                    im_data[node_counts[filename]][testname].append(data)
                
    for n in im_data: # get average of medians
        avg_elapsed = 0
        avg_instructions = 0
        avg_cycles = 0
        avg_branches = 0
        avg_branch_misses = 0
        nm=""
        for name in im_data[n]:
            nm = name
            name = im_data[n][name]
            if len(name) == 0:
                continue
            for d in name:
                data = d
                avg_elapsed += data.elapsed
                avg_instructions += data.instructions
                avg_cycles += data.cycles
                avg_branches += data.branches
                avg_branch_misses += data.branch_misses
            
            avg_elapsed /= len(name)
            avg_instructions /= len(name)
            avg_cycles /= len(name)
            avg_branches /= len(name)
            avg_branch_misses /= len(name)

            avg_data = TestData(nm,
                                n,
                                max(avg_elapsed, 0),
                                max(avg_instructions, 0),
                                max(avg_cycles, 0),
                                max(avg_branches, 0),
                                max(avg_branch_misses, 0))

            test_results[n][nm] = avg_data


parse_data()


def show_plot(node_count, value_string):
    collections = ["Simple Binary Tree",
        "Contiguous Binary Tree",
        'std::map',
        'std::unordered_map Scaled',
        "std::unordered map With 100 Buckets",
        "std::unordered map With 1000 Buckets",
        "std::unordered map With 5000 Buckets",
        "std::unordered map With 10000 Buckets",
        "std::unordered map With 20000 Buckets",
        "std::unordered map With 40000 Buckets",
        "std::unordered map With 70000 Buckets"
        ]
    speed = [-1] * len(collections)
    for d in test_results[node_count]:
        d = test_results[node_count][d]
        speed[test_names.index(d.test_name)] = d.get_value(value_string)
    plt.figure(figsize=(10, 10), layout="tight")
    plt.bar(collections, speed)
    plt.xticks(rotation=75)
    plt.title(value_string)
    plt.show()

for a in node_counts:
    for b in valid_attributes:
        show_plot(node_counts[a], b)
        break
    break

