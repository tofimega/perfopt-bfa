import json
import os


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

test_names = ["map test",
              "unordered map test",
              "deep umap test",
              "simple bintree test",
              "continuous bintree test"]


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
        if branches != 0 and branch_misses !=0:
            self.bm_ratio = branches/branch_misses
        else:
            self.bm_ratio = 1



dir_name = "records"


test_results = {} # key: node_count + test_name

def parse_data():
    dir_id= os.fsencode(dir_name)
    im_data = {} # key: node_count

    for n in node_counts:
        im_data[node_counts[n]] = []

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
                    im_data[node_counts[filename]].append(data)

    for n in im_data: # get average of medians
        avg_elapsed = 0
        avg_instructions = 0
        avg_cycles = 0
        avg_branches = 0
        avg_branch_misses = 0
        for data in im_data[n]:
            avg_elapsed += data.elapsed
            avg_instructions += data.instructions
            avg_cycles += data.cycles
            avg_branches += data.branches
            avg_branch_misses += data.branch_misses
        avg_elapsed /= len(im_data[n])
        avg_instructions /= len(im_data[n])
        avg_cycles /= len(im_data[n])
        avg_branches /= len(im_data[n])
        avg_branch_misses /= len(im_data[n])

        avg_data = TestData(im_data[n][0].test_name,
                            n,
                            avg_elapsed,
                            avg_instructions,
                            avg_cycles,
                            avg_branches,
                            avg_branch_misses)
        
        test_results[n][im_data[n][0].test_name] = avg_data


parse_data()
print(test_results)

# initialize test_results: for each node count a dict
# for each test: (btree, cbtree, map, umap, all dmaps) create TestResult
# for dmaps, add immediately (there is only one)
# for anything else, collect till end in mean_of_median