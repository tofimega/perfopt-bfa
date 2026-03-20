
nyelv: c++

2 db binkerfa + stdlib map

cél: keresési idő tesztelése

kulcs: size_t

tárolt érték: 2 pointer

## Load gen

paraméterek: beszúrandó elemek száma (std::size_t: n), seed (uint64_t: s), tesztek száma (std::size_t: k)


random engine: rand = Rng(s)

data = std::vector\<int64_t>()

n = min(n, data.max_size())

data.reserve(n)

for i in 0..n: data.push_back(i)

data = rand.shuffle(data)

bint = BinTree(data)
bint_cont = BinTreeCont(data)

map = std::map\<int64_t,  Node>()

for i in data: map.insert({i, Node(0, 0)})

## Keresés szimulálása, benchmark

Keretrendszer: [nanobench](https://nanobench.ankerl.com/)

tests =  vector\<int64_t>

tests.reseve(k)
 
for i in k: tests.push_back(data\[rand()%data.size()])
 
for t in tests: measure and record search times

## Fák

-  Node
	- left, right pointers

- IBinTree: közös iface
	- TODO

- BinTree:
	- Node:  root
	
	- Constr(&std::vector\<int64_t> data): sorban beszúrja a számokat a fába
	- Destr(): törli a fát postorder módon

	- egy Node pointerei közv a gyerekekre mutatnak
	- a Node-ok közv a heapen vannak

- BinTreeCont:
	- Node: root
	- std::vector\<Node> tree

	- Constr(&std::vector\<int64_t> data): sorban beszúrja a számokat a fába és a tree vector-ba
	- Destr(): törléskor a tree vector megoldja

	- egy Node pointer-ei offset a tree vector-ba
	- a Node-ok folytonosan vannak tárolva a tree vector-ban