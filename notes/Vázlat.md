
nyelv: c++

2 db binkerfa + stdlib map

cél: keresési idő tesztelése

kulcs: uint64_t

tárolt érték: 2 pointer

## Load gen

paraméterek: beszúrandó elemek száma (std::size_t: n), seed (uint64_t: s)

random engine: rand = Rng(s)

data = std::vector\<uint64_t>()

n = min(n, data.max_size())

data.reserve(n)

for i in 0..n: data.push_back(i)
rand.shuffle()

bint = BinTree(data)
bint_cont = BinTreeCont(data)

map = std::map\<uint64_t,  Node>()

for i in data: map.insert({i, Node(i, 0, 0)})

## Keresés szimulálása, benchmark

paraméterek:  tesztek száma (std::size_t: k), teszt seed (uint64_t: ts)

Keretrendszer: [nanobench](https://nanobench.ankerl.com/)

rand = Rng(ts)
 
for i in 0..k: pick from data, search

## Fák

- a fák nem módosíthatóak

-  Node
	- Node\* left, right
	- uint64_t value

- BinTree:
	- Node:  root
	
	- Constr(&std::vector\<uint64_t> data): sorban beszúrja a számokat a fába
	- Destr(): törli a fát postorder módon

	- find(int64_t value) -> Node

	- egy Node pointerei közv a gyerekekre mutatnak
	- a Node-ok közv a heapen vannak

- BinTreeCont:
	- Node: root
	- std::vector\<Node> tree

	- find(int64_t value) -> Node

	- Constr(&std::vector\<uint64_t> data): sorban beszúrja a számokat a fába és a tree vector-ba
	- Destr(): törléskor a tree vector megoldja

	- egy Node pointer-ei offset a tree vector-ba
	- a Node-ok folytonosan vannak tárolva a tree vector-ban